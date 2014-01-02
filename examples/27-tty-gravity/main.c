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

#define NOTREADABLE 0
#define READABLE    1

#define NORMAL 0
#define RAW    1

uv_loop_t *loop;
uv_tty_t tty;

uv_timer_t tick;
uv_write_t write_req;

int width, height;
int pos = 0;

char *msg = "  Hello TTY  ";

void timer_cb(uv_timer_t* req, int status) {
  if (status) ERROR("in timer callback", status);

  char data[500];

  uv_buf_t buf;
  buf.base = data;
  buf.len = sprintf(
      data
    , "\033[2J\033[H\033[%dB\033[%luC\033[42;37m%s"
    , pos
    , (unsigned long) (width - strlen(msg)) / 2
    , msg
  );

  uv_write(&write_req, (uv_stream_t*) &tty, &buf, 1, NULL);

  pos++;
  if (pos > height) {
    uv_tty_reset_mode();
    uv_timer_stop(&tick);
  }
}

int main() {
  int r;

  loop = uv_default_loop();

  r = uv_tty_init(loop, &tty, STDOUT, NOTREADABLE);
  if (r) ERROR("initing tty", r);

  // enable TYY formatting, flow-control, etc.
  r = uv_tty_set_mode(&tty, NORMAL);
  if (r) ERROR("setting tty mode", r);

  // https://github.com/thlorenz/libuv-dox/blob/master/methods.md#uv_tty_get_winsize
  r = uv_tty_get_winsize(&tty, &width, &height);
  if (r) {
    fprintf(stderr, "Unable to get TTY information\n");
    uv_tty_reset_mode();
    return 1;
  }

  fprintf(stderr, "Width %d, height %d\n", width, height);

  r = uv_timer_init(loop, &tick);
  if (r) ERROR("initing timer", r);

  r = uv_timer_start(&tick, timer_cb, 200, 200);
  if (r) ERROR("starting timer", r);

  return uv_run(loop, UV_RUN_DEFAULT);
}
