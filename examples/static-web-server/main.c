#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dgb.h"
#include "../libuv/include/uv.h"
#include "request-parser.h"
#include "resolve-resource.h"
#include "pipe-file.h"

#define CHECK(r, msg) if (r) {                                                 \
  log_err("%s: [%s(%d): %s]\n", msg, uv_err_name((r)), r, uv_strerror((r)));   \
  exit(1);                                                                     \
}

#define UVERR(r, msg) log_err("%s: [%s(%d): %s]\n", msg, uv_err_name((r)), r, uv_strerror((r)));

#define PORT    3000
#define BACKLOG 128

/* extends uv_tcp_t (handle) */
struct sws_handle_req_s {
  uv_tcp_t handle;
  int id;
  sws_parse_req_t parse_req;
  sws_resource_info_t resource_info;
};

typedef struct sws_handle_req_s sws_handle_req_t;

static uv_loop_t *loop;
static uv_tcp_t server;
static int id;

static void alloc_cb            ( uv_handle_t *handle, size_t size, uv_buf_t *buf);

static void on_connect          ( uv_stream_t *server, int status);
static void sws_handle_req_init ( sws_handle_req_t* handle_req, int req_id);
static void on_req_read         ( uv_stream_t *req, ssize_t nread, const uv_buf_t *buf);

static void on_parse_complete      ( sws_parse_req_t* parse_result);
static void on_resolve_resource    ( sws_resource_info_t* info);
static void on_pipe_file_complete  ( uv_stream_t* tcp, int status);
static void on_res_end             ( uv_handle_t *handle);
static void sws_cleanup_handle_req ( sws_handle_req_t *handle_req);

static void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  buf->base = malloc(size);
  buf->len = size;
}

static void on_connect(uv_stream_t *server, int status) {
  int r;

  CHECK(status, "connecting");
  debug("connecting req");

  // the tcp handle points to our sws_handle_req_t which can store a bit extra info
  sws_handle_req_t *handle_req = malloc(sizeof(sws_handle_req_t));
  sws_handle_req_init(handle_req, id++);

  r = uv_accept(server, (uv_stream_t*) &handle_req->handle);
  if (r) {
    log_err("error accepting connection %d", r);
    uv_close((uv_handle_t*) handle_req, NULL);
  } else {
    // read the req into the tcp socket to cause it to get parsed
    // once the headers are in we'll get called back the first time (see on_headers_complete)
    // for now we assume no body since this is just a static webserver
    uv_read_start((uv_stream_t*) handle_req, alloc_cb, on_req_read);
  }
}

static void sws_handle_req_init(sws_handle_req_t* handle_req, int req_id) {
  sws_req_parser_init(&handle_req->parse_req, req_id, on_parse_complete);
  uv_tcp_init(loop, &handle_req->handle);
  handle_req->handle.data = NULL;

  handle_req->id = req_id;

  // All contained types point to parent type, except for the tcp type since its data property needs
  // to be used for other pointers, i.e. inside pipe_file.
  // However since handle_req and tcp point to same address we can upcast tcp to handle_req when needed.
  handle_req->parse_req.data     = (void*) handle_req;
  handle_req->resource_info.data = (void*) handle_req;
}

static void on_req_read(uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf) {
  size_t parsed;
  sws_handle_req_t *handle_req = (sws_handle_req_t*) tcp;

  if (nread == UV_EOF) {
    uv_close((uv_handle_t*) tcp, NULL);

    debug("closed req tcp connection due to unexpected EOF");
  } else if (nread > 0) {
    log_info("[ %3d ] req (len %ld)", handle_req->id, nread);

    parsed = sws_req_parser_execute(&handle_req->parse_req, buf->base, nread);
    if (parsed < nread) {
      log_err("parsing http req");
      uv_close((uv_handle_t*) &handle_req->handle, on_res_end);
    }
  } else {
    UVERR((int) nread, "reading req req");
  }
  if (buf->base) free(buf->base);
}

static void on_parse_complete(sws_parse_req_t* parse_req) {
  sws_handle_req_t *handle_req = (sws_handle_req_t*) parse_req->data;
  debug("%s\n", sws_req_parser_result_str((sws_parse_result_t*)parse_req));

  sws_resolve_resource_init(loop, &handle_req->resource_info);

  sws_resolve_resource_start(&handle_req->resource_info, handle_req->parse_req.url, on_resolve_resource);
}

static void on_resolve_resource(sws_resource_info_t* info) {
  int r;
  sws_handle_req_t *handle_req = (sws_handle_req_t*) info->data;

  debug("resolved %s", sws_resource_info_str(info));
  if (info->result) {
    UVERR(info->result, "resolve resource");
    // TODO: return 404
    sws_resolve_resource_init(loop, &handle_req->resource_info);
    sws_resolve_resource_start(&handle_req->resource_info, "/404.html", on_resolve_resource);
  } else {
    r = sws_pipe_file(loop, (uv_stream_t*)&handle_req->handle, info->full_path, info->size, on_pipe_file_complete);
    CHECK(r, "pipe file");
  }
}

static void on_pipe_file_complete(uv_stream_t* tcp, int status) {
  CHECK(status, "pipe file");
  uv_close((uv_handle_t*) tcp, on_res_end);
}

static void on_res_end(uv_handle_t *handle) {
  sws_handle_req_t* handle_req = (sws_handle_req_t*) handle;
  log_info("[ %3d ] connection closed", handle_req->id);
  sws_cleanup_handle_req(handle_req);
  free(handle_req);
}

static void sws_cleanup_handle_req(sws_handle_req_t *handle_req) {
  sws_cleanup_parse_req(&handle_req->parse_req);
}

int main() {
  int r;

  id = 0;
  loop = uv_default_loop();

  struct sockaddr_in bind_addr;
  r = uv_ip4_addr("0.0.0.0", PORT, &bind_addr);
  CHECK(r, "get bind addr");

  r = uv_tcp_init(loop, &server);
  CHECK(r, "init server");

  r = uv_tcp_bind(&server, (const struct sockaddr*) &bind_addr);
  CHECK(r, "bind");

  r = uv_listen((uv_stream_t*) &server, BACKLOG, on_connect);
  CHECK(r, "listen");
  log_info("listening on http://localhost:%d", PORT);

  uv_run(loop, UV_RUN_DEFAULT);
  return 0;
}
