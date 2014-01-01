#include <stdio.h>
#include <stdlib.h>
#include <sys/syslimits.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);


#define STDIN   0
#define STDOUT  1
#define STDERR  2

uv_loop_t *loop;
uv_process_t child_req;
uv_process_options_t options;

char exepath[PATH_MAX];

void on_exit(uv_process_t* req, int64_t exit_status, int term_signal) {
  fprintf(stderr, "Process exited with status %lld, signal %d\n", exit_status, term_signal);
  uv_close((uv_handle_t*) req->data, NULL);
  uv_close((uv_handle_t*) req, NULL);
}

void exepath_for_tick() {
  int r;
  size_t size = PATH_MAX;

  r = uv_exepath(exepath, &size);
  if (r) ERROR("getting executable path", r);
  strcpy(exepath + (strlen(exepath) - strlen("main")), "tick");
}

void invoke_cgi_script(uv_tcp_t *client) {
  int r;
  exepath_for_tick();
  char *args[2] = { exepath, NULL };

  // https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_stdio_container_t
  // https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_stdio_flags
  uv_stdio_container_t child_stdio[3];
  child_stdio[STDIN].flags    =  UV_IGNORE;
  child_stdio[STDOUT].flags   =  UV_INHERIT_STREAM;
  child_stdio[STDERR].flags   =  UV_IGNORE;

  // pipe tick stdout into the tcp stream connected to the client
  child_stdio[STDOUT].data.stream = (uv_stream_t*) client;

  // https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_process_options_t
  options.stdio_count =  3;
  options.stdio       =  child_stdio;
  options.exit_cb     =  on_exit;
  options.file        =  exepath;
  options.args        =  args;

  // pass this handle along so we can close it on_exit
  child_req.data = (void*) client;

  fprintf(stderr, "spawning: %s %s\n", exepath, args[1]);
  r = uv_spawn(loop, &child_req, &options);
  if (r) ERROR("spawning child process", r);
}

void connection_cb(uv_stream_t *server, int status) {
  int r;
  if (status) {
    fprintf(stderr, "connection error %d", status);
    return;
  }

  uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
  uv_tcp_init(loop, client);
  r = uv_accept(server, (uv_stream_t*) client);
  if (r == 0) {
    invoke_cgi_script(client);
  } else {
    uv_close((uv_handle_t*) client, NULL);
  }
}

int main() {
  int r;

  loop = uv_default_loop();

  struct sockaddr_in bind_addr;
  r = uv_ip4_addr("0.0.0.0", 7000, &bind_addr);
  if (r) ERROR("obtaining ipv4 address", r);

  uv_tcp_t server;
  uv_tcp_init(loop, &server);

  r = uv_tcp_bind(&server, (struct sockaddr*) &bind_addr);
  if (r) ERROR("binding server to sockaddr", r);

  r = uv_listen((uv_stream_t*) &server, 128/*backlog*/, connection_cb);
  if (r) ERROR("listening for connections", r);


  return uv_run(loop, UV_RUN_DEFAULT);
}
