#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

#define MIN_PERSEC 200
#define MAX_PERSEC 1000

uv_loop_t *loop;
uv_async_t async;

void progress_cb(uv_async_t* handle, int status) {
  if (status) ERROR("async cb", status);

  // this could cause problems since the data field is not thread safe
  // ideally we'd use a mutex or rwlock to ensure correctly ordered accesses
  double percentage = *(double*) handle->data;
  fprintf(stderr, "Downloaded %.2f%%\n", percentage);
}

void work_cb(uv_work_t* req) {
 int size = *(int*) req->data;
 int downloaded = 0;
 double percentage;

 while (downloaded < size) {
   percentage = (100 * downloaded) / size;
   async.data = (void*) &percentage;
   uv_async_send(&async);

   sleep(1);
   downloaded += (MIN_PERSEC + random()) % MAX_PERSEC;
 }
}

void after_work_cb(uv_work_t* req, int status) {
  if (status) ERROR("after work", status);
  fprintf(stderr, "Download complete\n");

  // cleanup progress watcher otherwise process won't exit since we still have a handle pending
  uv_close((uv_handle_t*) &async, NULL);
}

int main() {
  loop = uv_default_loop();

  uv_work_t work_req;
  int size = 1024;
  work_req.data = (void*) &size;

  // https://github.com/thlorenz/libuv-dox/blob/master/methods.md#async
  uv_async_init(loop, &async, progress_cb);
  uv_queue_work(loop, &work_req, work_cb, after_work_cb);

  return uv_run(loop, UV_RUN_DEFAULT);
}
