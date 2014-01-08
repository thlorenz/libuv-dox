#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define CHECK(r, msg) if (r) {                                                 \
  fprintf(stderr, "%s: [%s(%d): %s]\n", msg, uv_err_name((r)), r, uv_strerror((r)));   \
  exit(1);                                                                     \
}

#define UVERR(r, msg) fprintf(stderr, "%s: [%s(%d): %s]\n", msg, uv_err_name((r)), r, uv_strerror((r)));

#define PORT    3000
#define BACKLOG 128

#define NOIPC   0
#define BUFSIZE 1024

typedef struct {
  uv_write_t req;
  uv_buf_t buf;
} write_req_t;

typedef struct {
  ssize_t nread;
  ssize_t nwritten;
  uv_pipe_t *file_pipe;
  uv_stream_t *tcp;
} file_tcp_pipe_t;

static uv_loop_t *loop;
static uv_tcp_t server;

static void on_req_read(uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf);
static void on_file_read(uv_stream_t *file_pipe, ssize_t nread, const uv_buf_t *buf);
static void write_data(uv_stream_t *tcp, size_t size, const uv_buf_t *buf, uv_write_cb cb);
static void on_write_res(uv_write_t* write_req, int status);
static void on_res_end(uv_handle_t *handle);
static void pipe_file(uv_stream_t *tcp);

static void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  // forcing smaller size to get multiple chunks even for a small file like this
  size_t len = BUFSIZE ? BUFSIZE : size;
  buf->base = malloc(len);
  buf->len = len;
}

static void on_connect(uv_stream_t *server, int status) {
  int r;

  CHECK(status, "connecting");

  uv_tcp_t *handle = malloc(sizeof(uv_tcp_t));
  uv_tcp_init(loop, handle);

  r = uv_accept(server, (uv_stream_t*) handle);
  if (r) {
    fprintf(stderr, "error accepting connection %d\n", r);
    uv_close((uv_handle_t*) handle, NULL);
  } else {
    // read the req into the tcp socket to cause it to get parsed
    // once the headers are in we'll get called back the first time (see on_headers_complete)
    // for now we assume no body since this is just a static webserver
    uv_read_start((uv_stream_t*) handle, alloc_cb, on_req_read);
  }
}

static void on_req_read(uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf) {

  if (nread == UV_EOF) {
    uv_close((uv_handle_t*) tcp, NULL);
    fprintf(stderr, "closed tcp connection due to unexpected EOF");
  } else if (nread > 0) {
    pipe_file(tcp);
  } else {
    UVERR((int) nread, "reading req");
  }
  if (buf->base) free(buf->base);
}

static void pipe_file(uv_stream_t *tcp) {
  const char *filename = "main.c";

  uv_pipe_t *file_pipe;

  uv_fs_t file_open_req;
  int fd = uv_fs_open(loop, &file_open_req, filename, O_RDONLY, 0644, NULL);

  file_pipe = (uv_pipe_t*) malloc(sizeof(uv_pipe_t));
  uv_pipe_init(loop, file_pipe, NOIPC);
  uv_pipe_open(file_pipe, fd);

  // allow us to get back to all info regarding this pipe via tcp->data or file_pipe->data
  file_tcp_pipe_t *file_tcp_pipe =  (file_tcp_pipe_t*) malloc(sizeof(file_tcp_pipe_t));
  file_tcp_pipe->nread           =  0;
  file_tcp_pipe->nwritten        =  0;
  file_tcp_pipe->file_pipe       =  file_pipe;
  file_tcp_pipe->tcp             =  tcp;
  file_pipe->data                =  (void*) file_tcp_pipe;
  tcp->data                      =  (void*) file_tcp_pipe;

  // TODO: write headers first
  fprintf(stderr, "piping file into response\n");
  uv_read_start((uv_stream_t*) file_pipe, alloc_cb, on_file_read);
}

static void on_file_read(uv_stream_t *file_pipe, ssize_t nread, const uv_buf_t *buf) {
  file_tcp_pipe_t *file_tcp_pipe = (file_tcp_pipe_t*) file_pipe->data;
  uv_stream_t *tcp = file_tcp_pipe->tcp;

  if (nread == UV_EOF) {
    // oddly this never hits
    fprintf(stderr, "EOF\n");
    uv_close((uv_handle_t*)&file_pipe, NULL);
  } else if (nread > 0) {
    file_tcp_pipe->nread += nread;
    write_data((uv_stream_t*)tcp, nread, buf, on_write_res);
  } else {
    UVERR((int)nread, "reading file");
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

static void free_write_req(write_req_t *req) {
  free(req->buf.base);
  free(req);
}

static void free_file_tcp_pipe(file_tcp_pipe_t* file_tcp_pipe) {
  free(file_tcp_pipe->file_pipe);
  free(file_tcp_pipe->tcp);
  free(file_tcp_pipe);
}

static void on_write_res(uv_write_t* write_req, int status) {
  CHECK(status, "writing file data to response");

  file_tcp_pipe_t *file_tcp_pipe = (file_tcp_pipe_t*) write_req->data;
  write_req_t *write_buf_req = (write_req_t*) write_req;

  file_tcp_pipe->nwritten += write_buf_req->buf.len;

  fprintf(stderr, "%ld of %ld\n", file_tcp_pipe->nwritten, file_tcp_pipe->nread);
  free_write_req(write_buf_req);

  if (file_tcp_pipe->nwritten == file_tcp_pipe->nread) {
    uv_close((uv_handle_t*) file_tcp_pipe->tcp, on_res_end);
  }
}

static void on_res_end(uv_handle_t *handle) {
  fprintf(stderr, "connection closed, cleaning up\n");
  uv_stream_t *tcp = (uv_stream_t*) handle;
  file_tcp_pipe_t *file_tcp_pipe = (file_tcp_pipe_t*) tcp->data;
  free_file_tcp_pipe(file_tcp_pipe);
}

int main() {
  int r;

  loop = uv_default_loop();

  struct sockaddr_in bind_addr;
  r = uv_ip4_addr("0.0.0.0", PORT, &bind_addr);
  CHECK(r, "get bind addr");

  r = uv_tcp_init(loop, &server);
  CHECK(r, "init server");

  r = uv_tcp_bind(&server, (const struct sockaddr*) &bind_addr);
  CHECK(r, "bind");

  r = uv_listen((uv_stream_t*) &server, BACKLOG, on_connect);
  fprintf(stderr, "listening on http://localhost:%d\n", PORT);

  uv_run(loop, UV_RUN_DEFAULT);
  return 0;
}
