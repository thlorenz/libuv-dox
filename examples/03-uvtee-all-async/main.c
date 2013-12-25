#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

#define STDIN 0
#define STDOUT 1

#define NOIPC 0

typedef struct {
  uv_write_t req;
  uv_buf_t buf;
} write_req_t;

void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf);
void read_cb(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);

uv_pipe_t stdin_pipe;
uv_pipe_t stdout_pipe;
uv_pipe_t file_pipe;

uv_fs_t file_open_req;

void open_cb(uv_fs_t* req) {
  assert(req == &file_open_req);
  assert(req->fs_type == UV_FS_OPEN);
  if (req->result < 0) ERROR("async open error", req->result);

  fprintf(stderr, "opened %s\n", req->path);

  uv_file fd = req->result;

  fprintf(stderr, "file: %d\n", fd);

  uv_pipe_init(uv_default_loop(), &file_pipe, 0);
  uv_pipe_open(&file_pipe, fd);

  // uv_pipe_t subclasses uv_stream_t
  uv_read_start((uv_stream_t*)&stdin_pipe, alloc_cb, read_cb);

  uv_fs_req_cleanup(req);
  assert(req->path == NULL);
}

void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  fprintf(stderr, "allocating %ld bytes\n", size);
  buf->base = malloc(size);
  buf->len = size;

  assert(buf->base != NULL);
}

void free_write_req(uv_write_t *req) {
  write_req_t *wreq = (write_req_t*) req;
  free(wreq->buf.base);
  free(wreq);
}

void write_data(uv_stream_t *dest, size_t size, const uv_buf_t *buf, uv_write_cb cb) {
  write_req_t *req = malloc(sizeof(write_req_t));
  req->buf = uv_buf_init((char*) malloc(size), size);
  // pass copy of the buf thru to free_write_req because:
  //  - buf de-allocates once we lose scope
  //  - therefore we need to copy it's contents, but also need to free the copy later
  memcpy(req->buf.base, buf->base, size);
  uv_write((uv_write_t*)req, (uv_stream_t*)dest, &req->buf, 1 /* n bufs */, cb);
}

void write_cb(uv_write_t* req, int status) {
  assert(status == 0);
  assert(req->type == UV_WRITE);
  free_write_req(req);
}

void read_cb(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
  if (nread == UV_EOF) {
    uv_close((uv_handle_t*)&stdin_pipe, NULL);
    uv_close((uv_handle_t*)&stdout_pipe, NULL);
    uv_close((uv_handle_t*)&file_pipe, NULL);
  } else if (nread > 0) {
    fprintf(stderr, "%ld bytes read\n", nread);
    write_data((uv_stream_t*)&stdout_pipe, nread, buf, write_cb);
    write_data((uv_stream_t*)&file_pipe, nread, buf, write_cb);
  }
  if (buf->base) free(buf->base);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Please provide output file.\n");
    fprintf(stderr, "Example: echo \"hello world\" | ./main test.txt\n");
    exit(1);
  }

  const char *filename = argv[1];

  uv_loop_t *loop = uv_default_loop();

  uv_pipe_init(loop, &stdin_pipe, NOIPC);
  uv_pipe_open(&stdin_pipe, STDIN);

  uv_pipe_init(loop, &stdout_pipe, NOIPC);
  uv_pipe_open(&stdout_pipe, STDOUT);

  uv_fs_open(loop, &file_open_req, filename, O_CREAT | O_RDWR, 0644, open_cb);

  uv_run(loop, UV_RUN_DEFAULT);

  return 0;
}
