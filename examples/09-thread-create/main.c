#include <stdio.h>
#include <unistd.h>
#include "../libuv/include/uv.h"

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
    sleep(1);
    fprintf(stderr, "tortoise ran one more step\n");
  }
  fprintf(stderr, "tortoise done running!\n");
}

// https://github.com/thlorenz/libuv-dox/blob/master/methods.md#thread-local-storage
int main() {
  int tracklen = 5;

  uv_thread_t hare_id;
  uv_thread_t tortoise_id;

  uv_thread_create(&hare_id, hare_entry, &tracklen);
  uv_thread_create(&tortoise_id, tortoise_entry, &tracklen);

  // no second param (ala pthread_join) which allows target thread passing back value to calling thread
  // instead send values via http://nikhilm.github.io/uvbook/threads.html#inter-thread-communication
  uv_thread_join(&hare_id);
  uv_thread_join(&tortoise_id);

  return 0;
}
