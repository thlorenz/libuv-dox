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

uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;

char buffer[1024];

void open_cb(uv_fs_t* req) {
  assert(req->fs_type == UV_FS_OPEN);
  if (req->result < 0) ERROR("async open error", req->result);

  printf("opened %s\n", req->path);


  uv_file fd = req->result;
  size_t length = sizeof(buffer);
  int64_t offset = -1;

  uv_fs_read(uv_default_loop(), &read_req, fd, buffer, length, offset, read_cb);
  uv_fs_req_cleanup(req);
}

void read_cb(uv_fs_t *req) {
  uv_fs_req_cleanup(req);

  assert(req->fs_type == UV_FS_READ);

  if (req->result < 0) ERROR("aync read error", req->result);
  if (req->result == 0) {
    uv_fs_t close_req;
    uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
  } else {
//    uv_fs_write(uv_default_loop(), &write_req, STDOUT, buffer, req->result, -1, write_cb);
  }

  printf("%ld bytes read\n", req->result);
}

void write_cb(uv_fs_t *req) {
  uv_fs_req_cleanup(req);
  if (req->result < 0) ERROR("async write error", req->result);

  printf("%ld bytes written\n", req->result);
  uv_fs_read(uv_default_loop(), &read_req, open_req.result, buffer, sizeof(buffer), -1, read_cb);
}

int main() {

  const char *filename = "main.c";

  // always returns 0 for async requests
  int r = uv_fs_open(uv_default_loop(), &open_req, filename, O_RDONLY, 0, open_cb);
  assert(r == 0);

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return 0;
}
