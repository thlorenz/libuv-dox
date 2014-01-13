#ifndef SWS_PIPE_FILE_H
#define SWS_PIPE_FILE_H

#include "../libuv/include/uv.h"

typedef void (*sws_pipe_file_cb)(uv_stream_t* tcp, int status);

/* Assumes that correct headers were sent.
 * Pipes the file into the tcp stream.
 * Caller is responsible for closing the tcp stream and any required cleanup.
 */
int sws_pipe_file(
    uv_loop_t* loop
  , uv_stream_t* tcp
  , const char* file_path
  , ssize_t size
  , sws_pipe_file_cb pipe_file_cb
  );

#endif
