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

void on_exit(uv_process_t* process, int64_t exit_status, int term_signal) {
  fprintf(stderr, "Process exited with status %lld, signal %d\n", exit_status, term_signal);
  uv_close((uv_handle_t*) process, NULL);
}

// https://github.com/thlorenz/libuv-dox/blob/master/methods.md#process
int main() {
  int r;
  loop = uv_default_loop();

  const char* exepath = "mkdir";
  char *args[3] = { (char*) exepath, "test-dir", NULL };

  // https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_process_options_t
  options.exit_cb = on_exit;
  options.file = exepath;
  options.args = args;


  fprintf(stderr, "spawning: %s %s\n", exepath, args[1]);
  r = uv_spawn(loop, &child_req, &options);
  if (r) ERROR("spawning child process", r);


  return uv_run(loop, UV_RUN_DEFAULT);
}
