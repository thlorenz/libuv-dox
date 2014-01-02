#include <stdio.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

uv_loop_t *loop;
uv_timer_t gc_req;
uv_timer_t fake_job_req;

void gc(uv_timer_t *handle, int status) {
  if (status) ERROR("calling gc", status);
  fprintf(stderr, "freeing unused objects\n");
}

void fake_job(uv_timer_t *handle, int status) {
  if (status) ERROR("calling fake_job", status);
  fprintf(stderr, "fake job done\n");
}

// Event loop runs as long as there are active handles/watchers (registered callbacks).
// By manually decreasing ref count we can allow loop to exit although watchers are still active.
// By manually increasing ref count we can keep loop alive even when no more watchers are active.
// https://github.com/thlorenz/libuv-dox/blob/master/methods.md#reference-count
int main() {
  int r;
  loop = uv_default_loop();

  r = uv_timer_init(loop, &gc_req);
  if (r) ERROR("initing gc timer", r);

  // program will exit even when gc timer is still registered
  uv_unref((uv_handle_t*) &gc_req);

  r = uv_timer_start(&gc_req, gc, 0 /*timeout*/, 1000 /*repeat*/);
  if (r) ERROR("starting gc timer", r);


  r = uv_timer_init(loop, &fake_job_req);
  if (r) ERROR("initing fake_job timer", r);

  r = uv_timer_start(&fake_job_req, fake_job, 5000 /*timeout*/, 0 /*repeat*/);
  if (r) ERROR("starting fake_job timer", r);

  return uv_run(loop, UV_RUN_DEFAULT);
}
