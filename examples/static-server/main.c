#include <stdio.h>
#include <stdlib.h>
#include "dgb.h"
#include "../libuv/include/uv.h"
#include "../http-parser/http_parser.h"

#define CHECK(r, msg) if (r) {                                                 \
  log_err("%s: [%s(%d): %s]\n", msg, uv_err_name((r)), r, uv_strerror((r)));   \
  exit(1);                                                                     \
}

#define UVERR(r, msg) log_err("%s: [%s(%d): %s]\n", msg, uv_err_name((r)), r, uv_strerror((r)));

#define PORT    3000
#define BACKLOG 128

#define DEFAULT_RESPONSE \
  "HTTP/1.1 200 OK\r\n" \
  "Content-Type: text/plain\r\n" \
  "Content-Length: 12\r\n" \
  "\r\n" \
  "hello world\n"

typedef struct {
  uv_tcp_t handle;
  http_parser parser;
  uv_write_t write_req;
  int request_id;
} client_t;

static uv_loop_t *loop;
static uv_tcp_t server;
static int request_id;
static http_parser_settings parser_settings;
static uv_buf_t default_response;

static void on_connect(uv_stream_t *server, int status);
static void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf);
static void on_client_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
static int on_headers_complete(http_parser* parser);
static void on_res_end(uv_handle_t *handle);

static void on_connect(uv_stream_t *server, int status) {
  int r;

  CHECK(status, "connecting");
  debug("connecting client");

  client_t *client = malloc(sizeof(client_t));
  client->request_id = request_id++;

  uv_tcp_init(loop, &client->handle);
  http_parser_init(&client->parser, HTTP_REQUEST);

  // see: on_headers_complete
  client->parser.data = client;

  // see: on_client_read
  client->handle.data = client;

  r = uv_accept(server, (uv_stream_t*) &client->handle);
  if (r) {
    log_err("error accepting connection %d", r);
    uv_close((uv_handle_t*) client, NULL);
  } else {
    uv_read_start((uv_stream_t*) client, alloc_cb, on_client_read);
  }
}

static void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  buf->base = malloc(size);
  buf->len = size;
}

static void on_client_read(uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf) {
  size_t parsed;

  if (nread == UV_EOF) {
    uv_close((uv_handle_t*) tcp, NULL);

    debug("closed client tcp connection due to unexpected EOF");
  } else if (nread > 0) {
    client_t *client = (client_t*) tcp->data;

    log_info("[ %3d ] request (len %ld)\n%s", client->request_id, nread, buf->base);

    parsed = http_parser_execute(&client->parser, &parser_settings, buf->base, nread);
    if (parsed < nread) {
      log_err("parsing http request");
      uv_close((uv_handle_t*) &client->handle, on_res_end);
    }
  } else {
    UVERR((int) nread, "reading client request");
  }
  if (buf->base) free(buf->base);
}

static void on_res_write(uv_write_t* req, int status) {
  CHECK(status, "on res write");
  uv_close((uv_handle_t*) req->handle, on_res_end);
}

static void on_res_end(uv_handle_t *handle) {
  client_t* client = (client_t*) handle->data;
  log_info("[ %3d ] connection closed", client->request_id);
}

static int on_headers_complete(http_parser* parser) {
  client_t *client = (client_t*) parser->data;
  log_info("[ %3d ] http message parsed", client->request_id);
  uv_write(&client->write_req, (uv_stream_t*) &client->handle, &default_response, 1, on_res_write);
  return 1;
}

int main() {
  int r;
  default_response.base = DEFAULT_RESPONSE;
  default_response.len = strlen(default_response.base);

  parser_settings.on_headers_complete  = on_headers_complete;

  request_id = 0;
  loop = uv_default_loop();

  struct sockaddr_in bind_addr;
  r = uv_ip4_addr("0.0.0.0", PORT, &bind_addr);
  CHECK(r, "get bind addr");

  r = uv_tcp_init(loop, &server);
  CHECK(r, "init server");

  r = uv_tcp_bind(&server, (const struct sockaddr*) &bind_addr);
  CHECK(r, "bind");

  r = uv_listen((uv_stream_t*) &server, BACKLOG, on_connect);
  log_info("listening on http://localhost:%d", PORT);

  uv_run(loop, UV_RUN_DEFAULT);
  return 0;
}
