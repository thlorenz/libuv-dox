#ifndef SWS_RESOLVE_RESOURCE_H
#define SWS_RESOLVE_RESOURCE_H

#include <sys/syslimits.h>
#include "../libuv/include/uv.h"

typedef struct sws_fileinfo_s sws_fileinfo_t;
typedef void (*sws_resolve_resource_cb)(sws_fileinfo_t*);

struct sws_fileinfo_s {
  char full_path[PATH_MAX];
  size_t size;
  unsigned int result;

  /* private */
  sws_resolve_resource_cb resolve_resource_cb;
};

char* sws_fileinfo_str(sws_fileinfo_t *info);
void sws_resolve_resource(uv_loop_t* loop, const char* url_path, sws_resolve_resource_cb resolve_resource_cb);
#endif
