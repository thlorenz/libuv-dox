#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../libuv/include/uv.h"

uv_loop_t *loop;
static const char *command;

void fs_event_cb(uv_fs_event_t *handle, const char *filename, int events, int status) {
  assert(status == 0);
  const char *type =
      events == UV_RENAME ? "renamed"
    : events == UV_CHANGE ? "changed"
    : "unknown";

  fprintf(stderr, "Change detected in %s: ", handle->filename);
  fprintf(stderr, "%s", type);
  fprintf(stderr, " %s\n", filename ? filename : "");

  system(command);
}

int main(int argc, const char *argv[]) {
  const char *path;
  uv_fs_event_t *fs_event;

  if (argc <= 2) {
    fprintf(stderr, "Usage: %s <command> <file1> [file2 ...]\n", argv[0]);
    return 1;
  }

  loop = uv_default_loop();


  command = argv[1];
  while (argc-- > 2) {
    path = argv[argc];
    fprintf(stderr, "Adding watch on %s\n", path);
    fs_event = (uv_fs_event_t*) malloc(sizeof(uv_fs_event_t));
    uv_fs_event_init(loop, fs_event);
    uv_fs_event_start(fs_event, fs_event_cb, path, 0);
  }

  return uv_run(loop, UV_RUN_DEFAULT);
}
