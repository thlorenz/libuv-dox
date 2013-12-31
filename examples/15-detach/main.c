#include <stdio.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

uv_loop_t *loop;

// https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_process_t--uv_handle_t
uv_process_t child_req;

uv_process_options_t options;

void on_exit(uv_process_t* child_process, int64_t exit_status, int term_signal) {
  fprintf(stderr, "Process exited with status %lld, signal %d\n", exit_status, term_signal);
  uv_close((uv_handle_t*) child_process, NULL);
}

// https://github.com/thlorenz/libuv-dox/blob/master/methods.md#process
int main() {
  int r;
  loop = uv_default_loop();

  const char* exepath = "sleep";
  char *args[3] = { (char*) exepath, "100", NULL };

  // https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_process_options_t
  options.exit_cb = on_exit;
  options.file = exepath;
  options.args = args;
  options.flags = UV_PROCESS_DETACHED;


  fprintf(stderr, "spawning detached: %s %s\n", exepath, args[1]);
  r = uv_spawn(loop, &child_req, &options);
  if (r) ERROR("spawning child process", r);

  fprintf(stderr, "Launched sleep with PID %d\n", child_req.pid);
  fprintf(stderr, "See it runnig via: ps aux | grep %d", child_req.pid);

  assert(uv_has_ref((uv_handle_t*) &child_req));

  // manually decrease event loop's ref count to allow our process to exit, making this a fire and forget
  // https://github.com/thlorenz/libuv-dox/blob/master/methods.md#reference-count
  uv_unref((uv_handle_t*) &child_req);

  assert(!uv_has_ref((uv_handle_t*) &child_req));

  return uv_run(loop, UV_RUN_DEFAULT);
}
