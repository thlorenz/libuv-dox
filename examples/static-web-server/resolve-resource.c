#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dgb.h"
#include "../libuv/include/uv.h"
#include "resolve-resource.h"

#define CHECK(r, msg) if (r) {                                                 \
  log_err("%s: [%s(%d): %s]\n", msg, uv_err_name((r)), r, uv_strerror((r)));   \
  exit(1);                                                                     \
}

#define UVERR(r, msg) log_err("%s: [%s(%d): %s]\n", msg, uv_err_name((r)), r, uv_strerror((r)));

#define INDEX_HTML  "/index.html"
#define STATIC      "/static"

static char root[PATH_MAX];

static void resolve_url(const char* url_path, char *full_path) {
  char *no_query;
  const char *cp;
  int len;

  // very naive way to trim the query string
  cp = url_path;
  while(*cp && *cp != '?') cp++;

  len = cp - url_path;
  no_query = (char*) malloc(len + 1);
  strncpy(no_query, url_path, len + 1);
  no_query[len] = '\0';

  // resolve / to index.html
  if (strcmp(no_query, "/") == 0) {
    sprintf(full_path, "%s%s", root, INDEX_HTML);
  } else {
    sprintf(full_path, "%s%s", root, no_query);
  }

  free(no_query);
}

char* sws_resource_info_str(sws_resource_info_t *info) {
  char *str = malloc(sizeof(char) * (PATH_MAX + 30 /* size */));
  sprintf(str, "%s (%ld)", info->full_path, info->size);
  return str;
}

void fs_stat_cb(uv_fs_t* req) {
  sws_resource_info_t *resource_info = (sws_resource_info_t*) req->data;

  if (req->result) {
    resource_info->result =req->result;
  } else {
    resource_info->size = req->statbuf.st_size;
  }

  uv_fs_req_cleanup(req);
  resource_info->resolve_resource_cb(resource_info);
}

int sws_resolve_resource_init(
    sws_resource_info_t* resource_info
  , uv_loop_t* loop
  ) {
  resource_info->loop = loop;
  return 0;
}

void sws_resolve_resource_start(
    sws_resource_info_t* resource_info
  , const char* url_path
  , sws_resolve_resource_cb resolve_resource_cb
  ) {

  int r;

  /* only resolve root the very first time since it shouldn't change  */
  /* and yes, we are assuming for now that all files are stored in ./static */
  if (!*root) {
    r = uv_cwd(root, PATH_MAX);
    CHECK(r, "getting cwd");
    strcat(root, STATIC);
  }

  resource_info->resolve_resource_cb = resolve_resource_cb;
  resolve_url(url_path, resource_info->full_path);

  uv_fs_t *fs_req = (uv_fs_t*) malloc(sizeof(uv_fs_t));
  fs_req->data = (void*) resource_info;
  r = uv_fs_stat(resource_info->loop, fs_req, resource_info->full_path, fs_stat_cb);
  CHECK(r, "fs_stat");
}

#undef INDEX_HTML
#undef STATIC
