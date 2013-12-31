#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);


// process stays alive until each handle calls back here when it received the signal
// useful to cleanly shutdown a server that is using multiple event loops
void signal_cb(uv_signal_t* req, int signum) {
  fprintf(stderr, "Received signal: %d\n", signum);
  uv_signal_stop(req);
}

// https://github.com/thlorenz/libuv-dox/blob/master/methods.md#signal
// http://ph7spot.com/musings/introduction-to-unix-signals-and-system-calls
void thread1_entry(void *arg) {
  // one loop for both signal handlers
  uv_loop_t *loop = uv_loop_new();

  uv_signal_t sig1a, sig1b;

  uv_signal_init(loop, &sig1a);
  uv_signal_start(&sig1a, signal_cb, SIGUSR1);

  uv_signal_init(loop, &sig1b);
  uv_signal_start(&sig1b, signal_cb, SIGUSR1);

  uv_run(loop, UV_RUN_DEFAULT);
}

void thread2_entry(void *arg) {
  // one loop for for each signal handler
  uv_loop_t *loop1 = uv_loop_new();
  uv_loop_t *loop2 = uv_loop_new();

  uv_signal_t sig1a, sig1b;

  uv_signal_init(loop1, &sig1a);
  uv_signal_start(&sig1a, signal_cb, SIGUSR1);

  uv_signal_init(loop2, &sig1b);
  uv_signal_start(&sig1b, signal_cb, SIGUSR1);

  // https://github.com/thlorenz/libuv-dox/blob/master/methods.md#uv_run
  // UV_RUN_NOWAIT is like UV_RUN_ONCE except that it doesn't block if there are no pending events
  while (uv_run(loop1, UV_RUN_NOWAIT) || uv_run(loop2, UV_RUN_NOWAIT)) {}
}

int main() {

  fprintf(stderr, "2 threads listening for SIGUSR1 on 3 event loops\n");
  fprintf(stderr, "send it via: kill -SIGUSR1 %d\n", getpid());

  // https://github.com/thlorenz/libuv-dox/blob/master/methods.md#thread-local-storage
  uv_thread_t thread1, thread2;

  uv_thread_create(&thread1, thread1_entry, 0);
  uv_thread_create(&thread2, thread2_entry, 0);

  uv_thread_join(&thread1);
  uv_thread_join(&thread2);


  return 0;
}
