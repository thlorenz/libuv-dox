#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/syslimits.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);


#define STDIN   0
#define STDOUT  1
#define STDERR  2

#define NOIPC 0
#define IPC   1

uv_loop_t *loop;

struct child_worker {
  uv_process_t req;
  uv_process_options_t options;
  uv_pipe_t pipe;
} *workers;

int round_robin_counter;
int child_worker_count;

char exepath[PATH_MAX];

uv_buf_t dummy_buf;

void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  buf->base = malloc(size);
  buf->len = size;
}

void on_exit(uv_process_t* req, int64_t exit_status, int term_signal) {
  fprintf(stderr, "Process exited with status %lld, signal %d\n", exit_status, term_signal);
  uv_close((uv_handle_t*) req, NULL);
}

void connection_cb(uv_stream_t *server, int status) {
  int r;
  if (status) {
    fprintf(stderr, "connection error %d", status);
    return;
  }

  // main diff between uv_tcp_t and uv_pipe_t is the ipc flag and the pipe_fname pointing to local sock file
  uv_pipe_t *client = (uv_pipe_t*) malloc(sizeof(uv_pipe_t));
  uv_pipe_init(loop, client, NOIPC);
  r = uv_accept(server, (uv_stream_t*) client);

  if (r == 0) {
    uv_write_t *write_req = (uv_write_t*) malloc(sizeof(uv_write_t));
    dummy_buf = uv_buf_init(".", 1);

    struct child_worker *worker = &workers[round_robin_counter];

    // Extended write function for sending handles over a pipe
    // https://github.com/thlorenz/libuv-dox/blob/master/methods.md#uv_write2
    uv_write2(write_req, (uv_stream_t*) &worker->pipe, &dummy_buf, 1 /*nbufs*/, (uv_stream_t*) client, NULL);

    round_robin_counter = (round_robin_counter + 1) % child_worker_count;
  } else {
    uv_close((uv_handle_t*) client, NULL);
  }
}

char* exepath_for_worker() {
  int r;
  size_t size = PATH_MAX;

  r = uv_exepath(exepath, &size);
  if (r) ERROR("getting executable path", r);
  strcpy(exepath + (strlen(exepath) - strlen("main")), "worker");
  return exepath;
}

int get_cpu_count() {
  int r;
  uv_cpu_info_t *info;
  int cpu_count;
  r = uv_cpu_info(&info, &cpu_count);
  if (r) ERROR("obtaining cpu info", r);

  uv_free_cpu_info(info, cpu_count);
  return cpu_count;
}

void setup_workers() {
  int r;
  exepath_for_worker();

  char* args[2];
  args[0] = exepath;
  args[1] = NULL;

  round_robin_counter = 0;
  int cpu_count = get_cpu_count();
  child_worker_count = cpu_count;

  workers = calloc(sizeof(struct child_worker), cpu_count);
  while (cpu_count--) {
    struct child_worker *worker = &workers[cpu_count];
    // pipe is acting as IPC channel
    uv_pipe_init(loop, &worker->pipe, IPC);

    // https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_stdio_container_t
    // https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_stdio_flags
    uv_stdio_container_t child_stdio[3];
    child_stdio[STDIN].flags       =  UV_CREATE_PIPE | UV_READABLE_PIPE;
    child_stdio[STDIN].data.stream =  (uv_stream_t*) &worker->pipe;
    child_stdio[STDOUT].flags      =  UV_IGNORE;
    child_stdio[STDERR].flags      =  UV_INHERIT_FD;
    child_stdio[STDERR].data.fd    =  STDERR;

    // https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_process_options_t
    worker->options.stdio_count =  3;
    worker->options.stdio       =  child_stdio;
    worker->options.exit_cb     =  on_exit;
    worker->options.file        =  exepath;
    worker->options.args        =  args;

    r = uv_spawn(loop, &worker->req, &worker->options);
    if (r) ERROR("spawning worker", r);

    fprintf(stderr, "Started worker %d\n", worker->req.pid);
  }
}

int main() {
  int r;
  loop = uv_default_loop();

  setup_workers();

  struct sockaddr_in bind_addr;
  r = uv_ip4_addr("0.0.0.0", 7000, &bind_addr);
  if (r) ERROR("obtaining ipv4 address", r);

  uv_tcp_t server;
  uv_tcp_init(loop, &server);
  r = uv_tcp_bind(&server, (const struct sockaddr*) &bind_addr);
  if (r) ERROR("binding server to socket", r);

  r = uv_listen((uv_stream_t*) &server, 128/*backlog*/, connection_cb);
  if (r) ERROR("listening for connections", r);

  return uv_run(loop, UV_RUN_DEFAULT);
}
