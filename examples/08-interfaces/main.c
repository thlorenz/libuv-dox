#include <stdio.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

void print_interface_info(uv_interface_address_t interface) {
  char buf[512];
  size_t buf_size = sizeof(buf);
  int r;

  fprintf(stderr, "%s (%s)\n", interface.name, interface.is_internal ? "internal" : "external");

  if (interface.address.address4.sin_family == AF_INET) {

    r = uv_ip4_name(&interface.address.address4, buf, buf_size);
    if (r) ERROR("ip4_name", r);
    fprintf(stderr, "  IPv4 address: %s\n", buf);

  } else if (interface.address.address4.sin_family == AF_INET6) {

    r = uv_ip6_name(&interface.address.address6, buf, buf_size);
    if (r) ERROR("ip6_name", r);
    fprintf(stderr, "  IPv6 address: %s\n", buf);

  }

  fprintf(stderr, "\n");
}

int main() {
  uv_interface_address_t *info;
  int count, i, r;

  r = uv_interface_addresses(&info, &count);
  if (r) ERROR("obtaining interface addresses", r);

  fprintf(stderr, "Number of interfaces: %d\n\n", count);

  for (i = count - 1; i >= 0; print_interface_info(info[i--]));

  return 0;
}
