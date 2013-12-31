#include <stdio.h>
#include <stdlib.h>
#include <sys/syslimits.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);


#define STDIN   0
#define STDOUT  1
#define STDERR  2

uv_loop_t *loop;
uv_process_t child_req;
uv_process_options_t options;

void on_exit(uv_process_t* child_process, int64_t exit_status, int term_signal) {
  fprintf(stderr, "Process exited with status %lld, signal %d\n", exit_status, term_signal);
  uv_close((uv_handle_t*) child_process, NULL);
}

char* exepath_for_test() {
  int r;
  size_t size = PATH_MAX;
  char *exepath = (char*) malloc(sizeof(char*) * size);;

  r = uv_exepath(exepath, &size);
  if (r) ERROR("getting executable path", r);
  strcpy(exepath + (strlen(exepath) - strlen("main")), "test");
  return exepath;
}

int main() {
  int r;

  loop = uv_default_loop();

  char* exepath = exepath_for_test();
  char *args[2] = { exepath, NULL };

  // https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_stdio_container_t
  // https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_stdio_flags
  uv_stdio_container_t child_stdio[3];
  child_stdio[STDIN].flags    =  UV_IGNORE;
  child_stdio[STDOUT].flags   =  UV_IGNORE;
  child_stdio[STDERR].flags   =  UV_INHERIT_FD;
  child_stdio[STDERR].data.fd =  STDERR;
  // only stderr will be piped to the output, so "This is stderr" is all we'll see

  // https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_process_options_t
  options.stdio_count =  3;
  options.stdio       =  child_stdio;
  options.exit_cb     =  on_exit;
  options.file        =  exepath;
  options.args        =  args;

  fprintf(stderr, "spawning: %s %s\n", exepath, args[1]);
  r = uv_spawn(loop, &child_req, &options);
  if (r) ERROR("spawning child process", r);

  uv_run(loop, UV_RUN_DEFAULT);

  free(exepath);
  return 0;
}
