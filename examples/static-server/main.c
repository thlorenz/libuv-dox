#include <stdio.h>
#include <stdlib.h>
#include "dgb.h"
#include "../libuv/include/uv.h"

#define CHECK(r, msg) if (r) {                                                 \
  log_err("%s: [%s(%d): %s]\n", msg, uv_err_name((r)), r, uv_strerror((r)));   \
  exit(1);                                                                     \
}

#define PORT    3000
#define BACKLOG 128

static uv_loop_t *loop;
static uv_tcp_t server;
static int client_id;

static void on_connection(uv_stream_t *server, int status);
static void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf);
static void on_client_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);

static void on_connection(uv_stream_t *server, int status) {
  int r;

  CHECK(status, "connecting");
  debug("connecting client");

  uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
  uv_tcp_init(loop, client);
  int *id = malloc(sizeof(int));
  *id = client_id++;
  client->data = id;

  r = uv_accept(server, (uv_stream_t*) client);
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

static void on_client_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
  if (nread == UV_EOF) {
    uv_close((uv_handle_t*) client, NULL);

    debug("closed client connection");
  } else if (nread > 0) {
    debug("%ld bytes read from client %d", nread, *(int*)client->data);
    uv_close((uv_handle_t*) client, NULL);
  }
  if (buf->base) free(buf->base);
}

int main() {
  int r;

  client_id = 0;
  loop = uv_default_loop();

  struct sockaddr_in bind_addr;
  r = uv_ip4_addr("0.0.0.0", PORT, &bind_addr);
  CHECK(r, "get bind addr");

  r = uv_tcp_init(loop, &server);
  CHECK(r, "init server");

  r = uv_tcp_bind(&server, (const struct sockaddr*) &bind_addr);
  CHECK(r, "bind");

  r = uv_listen((uv_stream_t*) &server, BACKLOG, on_connection);
  log_info("listening on http://localhost:%d", PORT);

  uv_run(loop, UV_RUN_DEFAULT);
  return 0;
}
