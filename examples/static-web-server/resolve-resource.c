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

#define INDEX_HTML      "/index.html"
#define INDEX_HTML_LEN  11
#define STATIC          "/static"
#define STATIC_LEN      7

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

char* sws_fileinfo_str(sws_fileinfo_t *info) {
  char *str = malloc(sizeof(char) * (PATH_MAX + 30 /* size */));
  sprintf(str, "%s (%ld)", info->full_path, info->size);
  return str;
}

void fs_stat_cb(uv_fs_t* req) {
  sws_fileinfo_t *fileinfo = (sws_fileinfo_t*) req->data;

  if (req->result) {
    fileinfo->result =req->result;
  } else {
    fileinfo->size = req->statbuf.st_size;
  }

  fileinfo->resolve_resource_cb(fileinfo);
  uv_fs_req_cleanup(req);
}

void sws_resolve_resource(uv_loop_t* loop, const char* url_path, sws_resolve_resource_cb resolve_resource_cb) {
  sws_fileinfo_t *fileinfo;
  int r;

  /* only resolve root the very first time since it shouldn't change  */
  /* and yes, we are assuming for now that all files are stored in ./static */
  if (!*root) {
    r = uv_cwd(root, PATH_MAX);
    CHECK(r, "getting cwd");
    strcat(root, "/static");
  }

  fileinfo = (sws_fileinfo_t*) malloc(sizeof(sws_fileinfo_t));
  fileinfo->resolve_resource_cb = resolve_resource_cb;
  resolve_url(url_path, fileinfo->full_path);

  uv_fs_t *req = (uv_fs_t*) malloc(sizeof(uv_fs_t));
  req->data = (void*) fileinfo;
  r = uv_fs_stat(loop, req, fileinfo->full_path, fs_stat_cb);
  CHECK(r, "fs_stat");
}

#undef INDEX_HTML
#undef INDEX_HTML_LEN
#undef STATIC
#undef STATIC_LEN


/*  Example:
 *
 *  void on_resolved_resource(sws_fileinfo_t *info) {
 *    debug("resolved %s", sws_fileinfo_str(info));
 *  }
 *
 *  int main(void) {
 *    uv_loop_t *loop;
 *    loop = uv_default_loop();
 *    sws_resolve_resource(loop, "/index.html", on_resolved_resource);
 *    uv_run(loop, UV_RUN_DEFAULT);
 *    return 0;
 *  }
 */
