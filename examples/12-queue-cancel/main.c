#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

#define FIB_UNTIL 25

uv_loop_t *loop;
uv_work_t fib_reqs[FIB_UNTIL];

long fib(long n) {
  return n == 0 || n == 1 ? 1 : fib(n - 1) + fib(n - 2);
}

void work_cb(uv_work_t* req) {
  int n = *(int*) req->data;

  fprintf(stderr, "calculating %dth fibonacci number\n", n);

  // simulate long running task
  if (random() % 2) sleep(1);
  else sleep(3);

  long result = fib(n);
  fprintf(stderr, "%dth fibonacci is %lu\n", n, result);

}

void after_work_cb(uv_work_t* req, int status) {
  if (status == UV_ECANCELED) {
    fprintf(stderr, "Calculation of %d cancelled\n", *(int*)req->data);
    return;
  }

  if (status) ERROR("after work", status);

  fprintf(stderr, "Finished calculating %dth fibonacci number\n", *(int*) req->data);
}

void signal_cb(uv_signal_t* req, int signum) {
  int i;
  fprintf(stderr, "Received signal: %d\n", signum);
  for (i = 0; i < FIB_UNTIL; i++) {
    uv_cancel((uv_req_t*) &fib_reqs[i]);
  }
  uv_signal_stop(req);
}


// https://github.com/thlorenz/libuv-dox/blob/master/methods.md#signal
// http://ph7spot.com/musings/introduction-to-unix-signals-and-system-calls
void listen_for_sigint() {
  uv_signal_t signal;
  uv_signal_init(loop, &signal);
  uv_signal_start(&signal, signal_cb, SIGINT);
}

// https://github.com/thlorenz/libuv-dox/blob/master/methods.md#work-queue
int main() {
  loop = uv_default_loop();

  int data[FIB_UNTIL];

  int i, r;
  for (i = 0; i < FIB_UNTIL; i++) {
    data[i] = i;
    fib_reqs[i].data = (void*) &data[i];
    r = uv_queue_work(loop, &fib_reqs[i], work_cb, after_work_cb);
    if (r) ERROR("queuing work", r);
  }

  listen_for_sigint();

  return uv_run(loop, UV_RUN_DEFAULT);
}
