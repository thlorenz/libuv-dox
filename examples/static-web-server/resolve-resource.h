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

  /* the client that requested the resource */
  uv_tcp_t *client;

  /* private */
  sws_resolve_resource_cb resolve_resource_cb;
};

char* sws_resource_info_str(sws_resource_info_t *info);
void sws_resolve_resource(
    uv_loop_t* loop
  , uv_tcp_t* client
  , const char* url_path
  , sws_resolve_resource_cb resolve_resource_cb
);
#endif
