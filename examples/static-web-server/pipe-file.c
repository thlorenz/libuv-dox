#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../libuv/include/uv.h"
#include "dgb.h"

#include "pipe-file.h"

#define CHECK(r, msg) if (r) {                                                          \
  fprintf(stderr, "%s: [%s(%d): %s]\n", msg, uv_err_name((r)), r, uv_strerror((r)));    \
  exit(1);                                                                              \
}

#define UVERR(r, msg) fprintf(stderr, "%s: [%s(%d): %s]\n", msg, uv_err_name((r)), r, uv_strerror((r)));

#define SUCCES_HDRS               \
  "HTTP/1.1 200 OK\r\n"           \
  "Content-Type: text/plain\r\n"  \
  "Transfer-Coding: chunked\r\n"  \
  "\r\n"

#define NOIPC   0

#define DONT_CALLBACK 0
#define CALLBACK      1

typedef struct write_req_s write_req_t;
typedef struct file_tcp_pipe_s file_tcp_pipe_t;

struct write_req_s {
  uv_write_t req;
  uv_buf_t buf;
};

struct file_tcp_pipe_s {
  uv_loop_t*         loop;
  ssize_t            nread;
  ssize_t            nwritten;
  ssize_t            size;
  uv_pipe_t          *file_pipe;
  uv_stream_t        *tcp;
  sws_pipe_file_cb   pipe_file_cb;
};

static int file_tcp_pipe_init(
    uv_loop_t* loop
  , file_tcp_pipe_t* file_tcp_pipe
  , uv_stream_t* tcp
  , const char* file_path
  , ssize_t size
  , sws_pipe_file_cb pipe_file_cb
  );

static int file_tcp_pipe_start (file_tcp_pipe_t* file_tcp_pipe);

static void on_file_read (uv_stream_t *file_pipe, ssize_t nread, const uv_buf_t *buf);
static void write_data   (uv_stream_t *tcp, size_t size, const uv_buf_t *buf, uv_write_cb cb);
static void on_write_res (uv_write_t* write_req, int status);

static void file_tcp_pipe_end(file_tcp_pipe_t *file_tcp_pipe, int status, int callback);

int sws_pipe_file(
    uv_loop_t* loop
  , uv_stream_t* tcp
  , const char* file_path
  , ssize_t size
  , sws_pipe_file_cb pipe_file_cb
  ) {

  int r;

  // we are using it to keep track of stuff, so we need to make sure that the caller isn't using it as well
  assert(tcp->data == NULL);

  file_tcp_pipe_t *file_tcp_pipe =  (file_tcp_pipe_t*) malloc(sizeof(file_tcp_pipe_t));

  r = file_tcp_pipe_init(loop, file_tcp_pipe, tcp, file_path, size, pipe_file_cb);
  if (r) {
    file_tcp_pipe_end(file_tcp_pipe, r, DONT_CALLBACK);
    return r;
  }

  // allow us to get back to all info regarding this pipe via tcp->data or file_pipe->data
  file_tcp_pipe->tcp->data       =  (void*) file_tcp_pipe;
  file_tcp_pipe->file_pipe->data =  (void*) file_tcp_pipe;

  r = file_tcp_pipe_start(file_tcp_pipe);
  if (r) return r;

  return 0;
}

static void free_write_req(write_req_t *req) {
  free(req->buf.base);
  free(req);
}

static void free_file_tcp_pipe(file_tcp_pipe_t* file_tcp_pipe) {
  free(file_tcp_pipe->file_pipe);
  // freeing the tcp stream is responsibility of whoever called pipe_file since we can't be sure
  // if it was allocated with malloc or not and/or if the connection is still needed after the file was sent
  free(file_tcp_pipe);
}

static void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  buf->base = malloc(size);
  buf->len = size;
}

static int file_tcp_pipe_init(
    uv_loop_t* loop
  , file_tcp_pipe_t* file_tcp_pipe
  , uv_stream_t* tcp
  , const char* file_path
  , ssize_t size
  , sws_pipe_file_cb pipe_file_cb
  ) {
  int r;
  uv_fs_t file_open_req;
  uv_pipe_t *file_pipe;
  file_pipe = (uv_pipe_t*) malloc(sizeof(uv_pipe_t));

  file_tcp_pipe->loop            =  loop;
  file_tcp_pipe->nread           =  0;
  file_tcp_pipe->nwritten        =  0;
  file_tcp_pipe->size            =  size;
  file_tcp_pipe->tcp             =  tcp;
  file_tcp_pipe->file_pipe       =  file_pipe;
  file_tcp_pipe->pipe_file_cb    =  pipe_file_cb;

  // XXX Not handling case of file not existing -- assuming whoever called as resolved it already
  int fd = uv_fs_open(loop, &file_open_req, file_path, O_RDONLY, 0644, NULL);

  r = uv_pipe_init(loop, file_pipe, NOIPC);
  if (r) return r;

  r = uv_pipe_open(file_pipe, fd);
  if (r) return r;

  return 0;
}

static int file_tcp_pipe_start(file_tcp_pipe_t* file_tcp_pipe) {
  dbg("piping file into response\n");

  return uv_read_start((uv_stream_t*) file_tcp_pipe->file_pipe, alloc_cb, on_file_read);
}

static void on_file_read(uv_stream_t *file_pipe, ssize_t nread, const uv_buf_t *buf) {
  file_tcp_pipe_t *file_tcp_pipe = (file_tcp_pipe_t*) file_pipe->data;
  uv_stream_t *tcp = file_tcp_pipe->tcp;

  if (nread == UV_EOF) {
    file_tcp_pipe_end(file_tcp_pipe, nread, CALLBACK);
  } else if (nread > 0) {
    file_tcp_pipe->nread += nread;
    write_data((uv_stream_t*)tcp, nread, buf, on_write_res);
  } else {
    file_tcp_pipe_end(file_tcp_pipe, nread, CALLBACK);
  }

  if (buf->base) free(buf->base);
}

static void write_data(uv_stream_t *tcp, size_t size, const uv_buf_t *buf, uv_write_cb cb) {
  write_req_t *write_req = malloc(sizeof(write_req_t));
  write_req->buf = uv_buf_init((char*) malloc(size), size);

  // keep file_tcp_pipe_t around
  write_req->req.data = (void*) tcp->data;
  memcpy(write_req->buf.base, buf->base, size);
  uv_write((uv_write_t*)write_req, (uv_stream_t*)tcp, &write_req->buf, 1 /* n bufs */, cb);
}

static void on_write_res(uv_write_t* write_req, int status) {
  CHECK(status, "writing file data to response");

  file_tcp_pipe_t *file_tcp_pipe = (file_tcp_pipe_t*) write_req->data;
  write_req_t *write_buf_req = (write_req_t*) write_req;

  file_tcp_pipe->nwritten += write_buf_req->buf.len;

  dbg("sent %ld of %ld bytes\n", file_tcp_pipe->nwritten, file_tcp_pipe->nread);
  free_write_req(write_buf_req);

  if (file_tcp_pipe->nwritten == file_tcp_pipe->nread) {
    assert(file_tcp_pipe->size == file_tcp_pipe->nread);
    file_tcp_pipe_end(file_tcp_pipe, status, CALLBACK);
  }
}

static void file_tcp_pipe_end(file_tcp_pipe_t *file_tcp_pipe, int status, int callback) {
  sws_pipe_file_cb pipe_file_cb;
  uv_stream_t *tcp;

  // TODO: close the file
  if (callback) {
    pipe_file_cb = file_tcp_pipe->pipe_file_cb;
    tcp = file_tcp_pipe->tcp;
    pipe_file_cb(tcp, status);
  }
  free_file_tcp_pipe(file_tcp_pipe);
}
