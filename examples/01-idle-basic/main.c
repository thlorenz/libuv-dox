#include <stdio.h>
#include "../libuv/include/uv.h"

// idle callbacks are processed after all other types of callbacks are processed
void idle_cb(uv_idle_t* handle, int status) {
  static int64_t count = -1;
  count++;
  // probably better to write to stdout stream to have those show up immediately
  if ((count % 10000) == 0) printf(".");
  if (count >= 10e6) uv_idle_stop(handle);
}

int main() {
  uv_idle_t idle_handle;

  uv_loop_t *loop = uv_default_loop();
  uv_idle_init(loop, &idle_handle);
  uv_idle_start(&idle_handle, idle_cb);

  printf("\nidling ...\n");
  uv_run(loop, UV_RUN_DEFAULT);

  return 0;
}
