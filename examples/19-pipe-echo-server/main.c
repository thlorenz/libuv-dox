#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/syslimits.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);


#define STDIN   0
#define STDOUT  1
#define STDERR  2

#define NOIPC 0
#define IPC   1

uv_loop_t *loop;


void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  buf->base = malloc(size);
  buf->len = size;
}


void write_cb(uv_write_t* req, int status) {
  if (status < 0) ERROR("async write", status);
  char *base = (char*) req->data;
  free(base);
  free(req);
}

void read_cb(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
  if (nread < 0) {
    fprintf(stderr, "read error: [%s: %s]\n", uv_err_name((nread)), uv_strerror((nread)));
    uv_close((uv_handle_t*) client, NULL);
    return;
  }

  uv_write_t *req = (uv_write_t*) malloc(sizeof(uv_write_t));
  req->data = (void*) buf->base;

  uv_write(req, client, buf, 1, write_cb);
}

void connect_cb(uv_stream_t *server, int status) {
  int r;
  if (status) {
    fprintf(stderr, "connection error: [%s: %s]\n", uv_err_name((status)), uv_strerror((status)));
    return;
  }

  uv_pipe_t *client = (uv_pipe_t*) malloc(sizeof(uv_pipe_t));
  r = uv_pipe_init(loop, client, NOIPC);;
  if (r) ERROR("initializing client pipe", r);

  r = uv_accept(server, (uv_stream_t*) client);
  if (r == 0) {
    uv_read_start((uv_stream_t*) client, alloc_cb, read_cb);
  } else {
    uv_close((uv_handle_t*) client, NULL);
  }
}

void sigint_cb(int sig) {
  int r;
  uv_fs_t req;
  r = uv_fs_unlink(loop, &req, "echo.sock", NULL);
  if (r) ERROR("unlinking echo.sock", r);
  exit(0);
}

int main() {
  int r;
  loop = uv_default_loop();

  signal(SIGINT, sigint_cb);

  uv_pipe_t server;
  uv_pipe_init(loop, &server, NOIPC);

  r = uv_pipe_bind(&server, "echo.sock");
  if (r) ERROR("binding to echo.sock", r);

  r = uv_listen((uv_stream_t*) &server, 128, connect_cb);
  if (r) ERROR("listening on socket", r);

  return uv_run(loop, UV_RUN_DEFAULT);
}
