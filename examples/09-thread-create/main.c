#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

void hare_entry(void *args) {
  int tracklen = *((int*) args);
  while(tracklen--) {
    sleep(1);
    fprintf(stderr, "hare ran one more step\n");
  }
  fprintf(stderr, "hare done running!\n");
}

void tortoise_entry(void *args) {
  int tracklen = *((int*) args);
  while(tracklen--) {
    sleep(3);
    fprintf(stderr, "tortoise ran one more step\n");
  }
  fprintf(stderr, "tortoise done running!\n");
}

// https://github.com/thlorenz/libuv-dox/blob/master/methods.md#thread-local-storage
int main() {
  int r;
  int tracklen = 5;

  uv_thread_t hare_id;
  uv_thread_t tortoise_id;

  r = uv_thread_create(&hare_id, hare_entry, &tracklen);
  if (r) ERROR("thread_create", r);

  r = uv_thread_create(&tortoise_id, tortoise_entry, &tracklen);
  if (r) ERROR("thread_create", r);

  // no second param (ala pthread_join) which allows target thread passing back value to calling thread
  // instead send values via http://nikhilm.github.io/uvbook/threads.html#inter-thread-communication
  r = uv_thread_join(&hare_id);
  if (r) ERROR("thread_join", r);

  r = uv_thread_join(&tortoise_id);
  if (r) ERROR("thread_join", r);

  return 0;
}
