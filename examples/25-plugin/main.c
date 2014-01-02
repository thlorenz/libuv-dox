#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../libuv/include/uv.h"

#include "plugin.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);


typedef void (*init_plugin_function)();


int main(int argc, const char *argv[]) {
  int r;

  if (argc == 1) {
    fprintf(stderr, "Usage: %s [plugin1] [plugin2] ...\n", argv[0]);
    return 0;
  }

  uv_lib_t *lib = (uv_lib_t*) malloc(sizeof(uv_lib_t));
  while(--argc) {
    fprintf(stderr, "Loading %s\n", argv[argc]);

    r = uv_dlopen(argv[argc], lib);
    if (r) {
      fprintf(stderr, "Error: %s\n", uv_dlerror(lib));
      continue;
    }

    init_plugin_function init_plugin;
    r = uv_dlsym(lib, "initialize", (void**) &init_plugin);
    if (r) {
      fprintf(stderr, "dlsym error: %s\n", uv_dlerror(lib));
      continue;
    }

    init_plugin();
  }

  return 0;
}
