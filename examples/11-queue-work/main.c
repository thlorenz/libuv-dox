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
  if (status) ERROR("after work", status);
  fprintf(stderr, "Finished calculating %dth fibonacci number\n", *(int*) req->data);
}

// https://github.com/thlorenz/libuv-dox/blob/master/methods.md#work-queue
int main() {
  loop = uv_default_loop();

  int data[FIB_UNTIL];
  uv_work_t req[FIB_UNTIL];

  int i, r;
  for (i = 0; i < FIB_UNTIL; i++) {
    data[i] = i;
    req[i].data = (void*) &data[i];
    r = uv_queue_work(loop, &req[i], work_cb, after_work_cb);
    if (r) ERROR("queuing work", r);
  }

  return uv_run(loop, UV_RUN_DEFAULT);
}
