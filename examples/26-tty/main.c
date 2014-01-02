#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "../libuv/include/uv.h"

#define ERROR(msg, code) do {                                                         \
  fprintf(stderr, "%s: [%s: %s]\n", msg, uv_err_name((code)), uv_strerror((code)));   \
  assert(0);                                                                          \
} while(0);

#define STDIN   0
#define STDOUT  1
#define STDERR  2

#define READABLE 1

#define NORMAL 0
#define RAW    1

uv_loop_t *loop;
uv_tty_t tty;

void write_to_tty(char* s) {
  uv_buf_t buf;
  uv_write_t req;
  buf.base = s;
  buf.len = strlen(buf.base);
  uv_write(&req, (uv_stream_t*) &tty, &buf, 1 /*nbufs*/, NULL);
}

// https://github.com/thlorenz/libuv-dox/blob/master/methods.md#tty
int main() {
  int r;

  loop = uv_default_loop();

  r = uv_tty_init(loop, &tty, STDOUT, READABLE);
  if (r) ERROR("initing tty", r);

  // enable TYY formatting, flow-control, etc.
  r = uv_tty_set_mode(&tty, NORMAL);
  if (r) ERROR("setting tty mode", r);

  // guesses what type of stream to use with the given file
  // https://github.com/thlorenz/libuv-dox/blob/master/methods.md#uv_guess_handle
  if (uv_guess_handle(STDOUT) == UV_TTY) {
    write_to_tty("\033[41;37m");
  }

  write_to_tty("Hello TTY\n");

  // restore state of terminal
  uv_tty_reset_mode();

  // NOTE: this hangs for me on DARWIN after printing Hello TTY on red background
  return uv_run(loop, UV_RUN_DEFAULT);
}
