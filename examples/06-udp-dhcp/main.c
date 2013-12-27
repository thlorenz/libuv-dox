#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

uv_loop_t *loop;
uv_udp_t send_socket;
uv_udp_t recv_socket;

uv_buf_t* make_discover_msg(uv_udp_send_t *req) {
  uv_buf_t *buf = malloc(sizeof(uv_buf_t));

  int buf_size = 256;
  buf->base = malloc(sizeof(char*) * buf_size);
  buf->len = buf_size;

  memset(buf->base, 0, buf->len);

  // BOOTREQUEST
  buf->base[0] = 0x1;
  // HTYPE ethernet
  buf->base[1] = 0x1;
  // HLEN
  buf->base[2] = 0x6;
  // HOPS
  buf->base[3] = 0x0;
  // XID 4 bytes
  buf->base[4] = (unsigned int) random();
  // SECS
  buf->base[8] = 0x0;
  // FLAGS
  buf->base[10] = 0x80;
  // CIADDR 12-15 is all zeros
  // YIADDR 16-19 is all zeros
  // SIADDR 20-23 is all zeros
  // GIADDR 24-27 is all zeros
  // CHADDR 28-43 is the MAC address, use your own
  buf->base[28] = 0xe4;
  buf->base[29] = 0xce;
  buf->base[30] = 0x8f;
  buf->base[31] = 0x13;
  buf->base[32] = 0xf6;
  buf->base[33] = 0xd4;
  // SNAME 64 bytes zero
  // FILE 128 bytes zero
  // OPTIONS
  // - magic cookie
  buf->base[236] = 99;
  buf->base[237] = 130;
  buf->base[238] = 83;
  buf->base[239] = 99;

  // DHCP Message type
  buf->base[240] = 53;
  buf->base[241] = 1;
  buf->base[242] = 1; // DHCPDISCOVER

  // DHCP Parameter request list
  buf->base[243] = 55;
  buf->base[244] = 4;
  buf->base[245] = 1;
  buf->base[246] = 3;
  buf->base[247] = 15;
  buf->base[248] = 6;

  return buf;
}

void print_dhcp_info(const uv_buf_t *buf) {
  unsigned int *as_integer = (unsigned int*)buf->base;
  unsigned int ipbin = ntohl(as_integer[4]);
  unsigned char ip[4] = {0};

  for (int i = 0; i < 4; i++) ip[i] = (ipbin >> i*8) & 0xff;

  fprintf(stderr, "Offered IP %d.%d.%d.%d\n", ip[3], ip[2], ip[1], ip[0]);
}

void recv_cb(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned int flags) {

  if (nread < 0) {
    // there seems to be no way to get an error code here (none of the udp tests do)
    fprintf(stderr, "recv error unexpected\n");
    uv_close((uv_handle_t*) req, NULL);
    free(buf->base);
    return;
  }

  char sender[17] = { 0 };
  uv_ip4_name((struct sockaddr_in*) addr, sender, 16);
  fprintf(stderr, "recv from %s\n", sender);

  print_dhcp_info(buf);

  free(buf->base);
  uv_udp_recv_stop(req);
}

void send_cb(uv_udp_send_t *req, int status) {
  if (status) fprintf(stderr, "async send %s %s\n", uv_err_name(status), uv_strerror(status));
}

void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  buf->base = malloc(size);
  buf->len = size;

  assert(buf->base != NULL);
}

int main() {
  loop = uv_default_loop();

  uv_udp_init(loop, &recv_socket);

  // recv
  //  addr
  struct sockaddr_in recv_addr;
  int r = uv_ip4_addr("0.0.0.0", 68, &recv_addr);
  if (r) ERROR("ip4_addr", r);

  //  bind
  r = uv_udp_bind(&recv_socket, (const struct sockaddr*)&recv_addr, 0 /*unused*/);
  if (r) ERROR("udp bind", r);

  //  start
  r = uv_udp_recv_start(&recv_socket, alloc_cb, recv_cb);
  if (r) ERROR("recv start", r);

  // broadcast
  //  addr
  struct sockaddr_in broadcast_addr;
  r = uv_ip4_addr("0.0.0.0", 0, &broadcast_addr);
  if (r) ERROR("ip4_addr", r);

  //  init, bind, broadcast
  uv_udp_init(loop, &send_socket);
  uv_udp_bind(&send_socket, (const struct sockaddr*)&broadcast_addr, 0);
  uv_udp_set_broadcast(&send_socket, 1);

  // send
  uv_udp_send_t send_req;

  //  addr
  struct sockaddr_in send_addr;
  r = uv_ip4_addr("255.255.255.255", 67, &send_addr);
  if (r) ERROR("ip4_addr", r);

  //  msg
  const uv_buf_t *discover_msg = make_discover_msg(&send_req);

  //  send
  uv_udp_send(&send_req, &send_socket, discover_msg, 1 /*nbufs*/, (const struct sockaddr*) &send_addr, send_cb);

  return uv_run(loop, UV_RUN_DEFAULT);
}
