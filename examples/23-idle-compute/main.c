#include <stdio.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

#define STDIN   0
#define STDOUT  1
#define STDERR  2

void wait_for_next_input(void);

uv_loop_t *loop;
uv_fs_t stdin_watcher;
uv_idle_t idler;
char buffer[1024];

void stdin_fs_cb(uv_fs_t* req) {
  if (stdin_watcher.result > 0) {
    buffer[stdin_watcher.result] = '\0';
    fprintf(stderr, "Typed %s\n", buffer);
    wait_for_next_input();
  } else {
    fprintf(stderr, "error opening file: [%s: %s]\n", uv_err_name((req->result)), uv_strerror((req->result)));
  }
}

void idle_cb(uv_idle_t* handle, int status) {
  fprintf(stderr, "Performing long running computation ...\n");
  uv_idle_stop(handle);
}

void wait_for_next_input() {
  int r;
  // Note: the example in the uvbook contains a small mistake  IMO since it's reading from fd: 1 which is stdout,
  //       however it works either way
  r = uv_fs_read(loop, &stdin_watcher, STDIN, buffer, 1024, -1, stdin_fs_cb);
  if (r) ERROR("reading stdin", r);

  r = uv_idle_start(&idler, idle_cb);
  if (r) ERROR("starting idler", r);
}

// idle-watcher callbacks are low priority, as they are only invoked for each iteration when the event loop has no other pending events
int main() {
  int r;

  loop = uv_default_loop();

  r = uv_idle_init(loop, &idler);
  if (r) ERROR("initing idler", r);

  wait_for_next_input();

  return uv_run(loop, UV_RUN_DEFAULT);
}
