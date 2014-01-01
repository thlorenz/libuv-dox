#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
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
uv_pipe_t queue;

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

  uv_write(req, (uv_stream_t*)client, buf, 1, write_cb);
}

void connect_cb(uv_pipe_t* queue_, ssize_t nread, const uv_buf_t* buf, uv_handle_type pending) {
  int r;
  if (pending == UV_UNKNOWN_HANDLE) {
    fprintf(stderr, "attempt to connect with unknown handle");
    return;
  }

  uv_pipe_t *client = (uv_pipe_t*) malloc(sizeof(uv_pipe_t));
  r = uv_pipe_init(loop, client, NOIPC);;
  if (r) ERROR("initializing client pipe", r);

  // get fd (client) from another fd (queue_ - listening socket == server)
  r = uv_accept((uv_stream_t*) queue_, (uv_stream_t*) client);
  if (r == 0) {
    fprintf(stderr, "Worker %d: Accepted fd %d\n", getpid(), client->io_watcher.fd);
    uv_read_start((uv_stream_t*) client, alloc_cb, read_cb);
  } else {
    uv_close((uv_handle_t*) client, NULL);
  }
}

int main(void) {
  loop = uv_default_loop();

  uv_pipe_init(loop, &queue, IPC);
  uv_pipe_open(&queue, STDIN);

  // same as read, but allows passing handles over pipes
  // https://github.com/thlorenz/libuv-dox/blob/master/methods.md#uv_read2_start
  uv_read2_start((uv_stream_t*)&queue, alloc_cb, connect_cb);
  return uv_run(loop, UV_RUN_DEFAULT);
}
