#ifndef SWS_RESOLVE_RESOURCE_H
#define SWS_RESOLVE_RESOURCE_H

#include <sys/syslimits.h>
#include "../libuv/include/uv.h"

typedef struct sws_resource_info_s sws_resource_info_t;
typedef void (*sws_resolve_resource_cb)(sws_resource_info_t*);

struct sws_resource_info_s {
  char full_path[PATH_MAX];
  size_t size;
  unsigned int result;

  /* for now this will point to sws_parse_result_t, but we don't want to know about that type here */
  void* data;

  /* private */
  sws_resolve_resource_cb resolve_resource_cb;
  uv_loop_t *loop;
};

char* sws_resource_info_str(sws_resource_info_t *info);

int sws_resolve_resource_init(
    uv_loop_t* loop
  , sws_resource_info_t* resource_info
  );

void sws_resolve_resource_start(
    sws_resource_info_t* resource_info
  , const char* url_path
  , sws_resolve_resource_cb resolve_resource_cb
);
#endif
