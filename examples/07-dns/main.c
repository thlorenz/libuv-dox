#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

void connect_cb(uv_connect_t *req, int status);
void read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);

uv_loop_t *loop;

void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  buf->base = malloc(size);
  buf->len = size;
}

char* terminated_cpy(char* orig, ssize_t len) {
  char *cpy = (char*) malloc(sizeof(char*) * (len + 1));
  cpy[len] = '\0';
  strncpy(cpy, orig, len);
  return cpy;
}

void getaddrinfo_cb(uv_getaddrinfo_t *req, int status, struct addrinfo *res) {
  if (status < 0) ERROR("async getaddrinfo", status);

  char addr[17] = { '\0' };
  uv_ip4_name((struct sockaddr_in*) res->ai_addr, addr, 16);
  fprintf(stderr, "%s\n", addr);

  uv_tcp_t *socket = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
  uv_tcp_init(loop, socket);

  uv_connect_t *connect_req = (uv_connect_t*) malloc(sizeof(uv_connect_t));
  // pass along socket so we can start reading on it once we are connected
  connect_req->data = (void*) socket;
  uv_tcp_connect(connect_req, socket, (const struct sockaddr*) res->ai_addr, connect_cb);

  uv_freeaddrinfo(res);
}

void connect_cb(uv_connect_t *req, int status) {
  if (status < 0) ERROR("async connect", status);

  // req->data points to tcp socket which at this point we'll just treat like any other stream
  uv_read_start((uv_stream_t*) req->data, alloc_cb, read_cb);
  free(req);
}

void read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
  if (nread == UV_EOF) {
    fprintf(stderr, "Got EOF, closing connection");
    uv_close((uv_handle_t*) client, NULL);
    free(client);
    return;
  }

  char *cpy = terminated_cpy(buf->base, nread);

  fprintf(stderr, "%s", cpy);
  free(cpy);
  free(buf->base);
}

int main() {
  int r;
  loop = uv_default_loop();

  struct addrinfo hints;
  hints.ai_family = PF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = 0;

  char *node = "irc.freenode.net";
  char *service = "6667";

  fprintf(stderr, "%s is ... ", node);

  uv_getaddrinfo_t addrinfo_req;
  r = uv_getaddrinfo(loop, &addrinfo_req, getaddrinfo_cb, node, service, &hints);
  if (r) ERROR("getaddrinfo", r);


  return uv_run(loop, UV_RUN_DEFAULT);
}
