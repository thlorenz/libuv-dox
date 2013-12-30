#include <stdio.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

uv_barrier_t blocker;
uv_rwlock_t numlock;
int shared_num;

#define ROUNDS 20

// https://github.com/thlorenz/libuv-dox/blob/master/methods.md#uv_rwlock_rdlock
void reader_entry(void *n) {
  int r;
  int num = *(int*)n;
  for (int i = 0; i < ROUNDS; i++) {
    do {
      r = uv_rwlock_tryrdlock(&numlock);

      // implementation ./src/unix/thread.c aborts the process unless return value was EBUSY or EAGAIN
      // so checking here again is redundant and mainly done for documentation
      // fact is that the only r we'd ever get would be EBUSY or EAGAIN (negated)
      if (r && r != -EBUSY && r != -EAGAIN) ERROR("obtaining read lock", r);
    } while(r);

    fprintf(stderr, "Reader %d: aquired lock and shared num = %d\n", num, shared_num);

    uv_rwlock_rdunlock(&numlock);
    fprintf(stderr, "Reader %d: released lock\n", num);
  }
  uv_barrier_wait(&blocker);
}

// https://github.com/thlorenz/libuv-dox/blob/master/methods.md#uv_rwlock_wrlock
void writer_entry(void *n) {
  int r;
  int num = *(int*)n;
  for (int i = 0; i < ROUNDS; i++) {
    do {
      r = uv_rwlock_trywrlock(&numlock);

      // see: reader_entry
      if (r && r != -EBUSY && r != -EAGAIN) ERROR("obtaining write lock", r);
    } while(r);

    fprintf(stderr, "Writer %d: aquired lock\n", num);
    shared_num++;
    fprintf(stderr, "Writer %d: incremented shared num = %d\n", num, shared_num);
    uv_rwlock_wrunlock(&numlock);
    fprintf(stderr, "Writer %d: released lock\n", num);
  }
  uv_barrier_wait(&blocker);
}

// same as ./10-locks, but using try* functions when obtaining read and write locks
//
int main() {
  int r;
  const int count = 4;

  fprintf(stderr, "barrier: init\n");
  uv_barrier_init(&blocker, count);

  shared_num = 0;

  // https://github.com/thlorenz/libuv-dox/blob/master/methods.md#rwlock
  fprintf(stderr, "rwlock: init\n");
  r = uv_rwlock_init(&numlock);
  if (r) ERROR("rwlock_init", r);

  uv_thread_t threads[3];

  int thread_nums[] = { 1, 2, 1 };
  r = uv_thread_create(&threads[0], reader_entry, &thread_nums[0]);
  if (r) ERROR("thread_create", r);

  r = uv_thread_create(&threads[1], reader_entry, &thread_nums[1]);
  if (r) ERROR("thread_create", r);

  r = uv_thread_create(&threads[2], writer_entry, &thread_nums[2]);
  if (r) ERROR("thread_create", r);

  // https://github.com/thlorenz/libuv-dox/blob/master/methods.md#barrier
  fprintf(stderr, "barrier: wait\n");
  uv_barrier_wait(&blocker);

  fprintf(stderr, "barrier: destroy\n");
  uv_barrier_destroy(&blocker);

  fprintf(stderr, "rwlock: destroy\n");
  uv_rwlock_destroy(&numlock);
  if (r) ERROR("rwlock_destroy", r);

  return 0;
}
