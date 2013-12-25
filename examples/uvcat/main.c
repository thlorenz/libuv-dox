#include <stdio.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

#define STDOUT 1

void read_cb(uv_fs_t *req);
void write_cb(uv_fs_t *req);
void close_cb(uv_fs_t *req);

// these need to be defined at module level in order to prevent them from being deallocated after we leave the function body
uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;
uv_fs_t close_req;

// shared across open/read/write
char buffer[1024];

void open_cb(uv_fs_t* req) {
  assert(req == &open_req);
  assert(req->fs_type == UV_FS_OPEN);
  if (req->result < 0) ERROR("async open error", req->result);

  fprintf(stderr, "opened %s\n", req->path);

  uv_file fd = req->result;

  fprintf(stderr, "file: %d\n", fd);
  uv_fs_read(uv_default_loop(), &read_req, fd, buffer, sizeof(buffer), -1 /*offset*/, read_cb);

  uv_fs_req_cleanup(req);
  assert(req->path == NULL);
}

void read_cb(uv_fs_t *req) {
  assert(req == &read_req);
  assert(req->fs_type == UV_FS_READ);
  if (req->result < 0) ERROR("aync read error", req->result);

  uv_fs_req_cleanup(req);

  if (req->result == 0) {
    uv_fs_close(uv_default_loop(), &close_req, open_req.result, close_cb);
  } else {
    uv_fs_write(uv_default_loop(), &write_req, STDOUT, buffer, req->result, -1 /*offset*/, write_cb);
  }

  fprintf(stderr, "%ld bytes read\n", req->result);
}

void write_cb(uv_fs_t *req) {
  assert(req == &write_req);
  assert(req->fs_type == UV_FS_WRITE);
  if (req->result < 0) ERROR("async write error", req->result);

  uv_fs_req_cleanup(req);

  fprintf(stderr, "%ld bytes written\n", req->result);
  uv_fs_read(uv_default_loop(), &read_req, open_req.result, buffer, sizeof(buffer), -1, read_cb);
}

void close_cb(uv_fs_t *req) {
  uv_fs_req_cleanup(req);

  assert(req == &close_req);
  assert(req->fs_type == UV_FS_CLOSE);
  if (req->result < 0) ERROR("aync close error", req->result);

  fprintf(stderr, "closed\n");
}

int main() {
  const char *filename = "main.c";

  uv_fs_open(uv_default_loop(), &open_req, filename, O_RDONLY, S_IRUSR, open_cb);

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return 0;
}
