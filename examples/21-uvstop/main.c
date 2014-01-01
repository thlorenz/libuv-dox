#include <stdio.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

int64_t count = 0;

uv_loop_t *loop;

void idle_cb(uv_idle_t *handle, int status) {
  if (status) ERROR("when idling", status);
  fprintf(stderr, "idle callback\n");

  if (count++ >= 5) {
    uv_stop(loop);
    fprintf(stderr, "uv_stop() called\n");
  }
}

void prepare_cb(uv_prepare_t *handle, int status) {
  if (status) ERROR("when preparing", status);
  fprintf(stderr, "prepare callback\n");
}

// uv_stop is useful to shut down a loop w/out having to ensure that all handles are stopped
// https://github.com/thlorenz/libuv-dox/blob/master/methods.md#uv_stop
int main() {
  int r;
  loop = uv_default_loop();

  // https://github.com/thlorenz/libuv-dox/blob/master/methods.md#idle
  // https://github.com/thlorenz/libuv-dox/blob/master/types.md#idle
  uv_idle_t idler;

  r = uv_idle_init(loop, &idler);
  if (r) ERROR("idle init", r);

  r = uv_idle_start(&idler, idle_cb);
  if (r) ERROR("idle start", r);


  // https://github.com/thlorenz/libuv-dox/blob/master/methods.md#prepare
  // https://github.com/thlorenz/libuv-dox/blob/master/types.md#prepare
  uv_prepare_t prep;

  uv_prepare_init(loop, &prep);
  if (r) ERROR("prepare init", r);

  uv_prepare_start(&prep, prepare_cb);
  if (r) ERROR("prepare start", r);

  // loop calls handlers in the following order:
  // - run_timers
  // - run_idle
  // - run_prepare
  // - run_pending

  r = uv_run(loop, UV_RUN_DEFAULT);
  fprintf(stderr, "uv_run returned %d because it was stopped", r);

  return 0;
}
