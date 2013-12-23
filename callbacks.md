**Table of Contents**  *generated with [DocToc](http://doctoc.herokuapp.com/)*

- [`uv_alloc_cb`](#uv_alloc_cb)
- [`uv_read_cb`](#uv_read_cb)
- [`uv_read2_cb`](#uv_read2_cb)
- [`uv_write_cb`](#uv_write_cb)
- [`uv_connect_cb`](#uv_connect_cb)
- [`uv_shutdown_cb`](#uv_shutdown_cb)
- [`uv_connection_cb`](#uv_connection_cb)
- [`uv_close_cb`](#uv_close_cb)
- [`uv_poll_cb`](#uv_poll_cb)
- [`uv_timer_cb`](#uv_timer_cb)
- [`uv_async_cb`](#uv_async_cb)
- [`uv_prepare_cb`](#uv_prepare_cb)
- [`uv_check_cb`](#uv_check_cb)
- [`uv_idle_cb`](#uv_idle_cb)
- [`uv_exit_cb`](#uv_exit_cb)
- [`uv_walk_cb`](#uv_walk_cb)
- [`uv_fs_cb`](#uv_fs_cb)
- [`uv_work_cb`](#uv_work_cb)
- [`uv_after_work_cb`](#uv_after_work_cb)
- [`uv_getaddrinfo_cb`](#uv_getaddrinfo_cb)
- [`uv_fs_event_cb`](#uv_fs_event_cb)
- [`uv_fs_poll_cb`](#uv_fs_poll_cb)
- [`uv_signal_cb`](#uv_signal_cb)
- [`uv_udp_send_cb`](#uv_udp_send_cb)
- [`uv_udp_recv_cb`](#uv_udp_recv_cb)

## `uv_alloc_cb`

```c
/*
 * Should prepare a buffer that libuv can use to read data into.
 *
 * `suggested_size` is a hint. Returning a buffer that is smaller is perfectly
 * okay as long as `buf.len > 0`.
 *
 * If you return a buffer with `buf.len == 0`, libuv skips the read and calls
 * your read or recv callback with nread=UV_ENOBUFS.
 *
 * Note that returning a zero-length buffer does not stop the handle, call
 * uv_read_stop() or uv_udp_recv_stop() for that.
 */
typedef void (*uv_alloc_cb)(uv_handle_t* handle,
                            size_t suggested_size,
                            uv_buf_t* buf);
```

## `uv_read_cb`

```c
/*
 * `nread` is > 0 if there is data available, 0 if libuv is done reading for
 * now, or < 0 on error.
 *
 * The callee is responsible for closing the stream when an error happens.
 * Trying to read from the stream again is undefined.
 *
 * The callee is responsible for freeing the buffer, libuv does not reuse it.
 * The buffer may be a null buffer (where buf->base=NULL and buf->len=0) on
 * EOF or error.
 */
typedef void (*uv_read_cb)(uv_stream_t* stream,
                           ssize_t nread,
                           const uv_buf_t* buf);
```

## `uv_read2_cb`

```c
/*
 * Just like the uv_read_cb except that if the pending parameter is true
 * then you can use uv_accept() to pull the new handle into the process.
 * If no handle is pending then pending will be UV_UNKNOWN_HANDLE.
 */
typedef void (*uv_read2_cb)(uv_pipe_t* pipe,
                                  ssize_t nread,
                                  const uv_buf_t* buf,
                                  uv_handle_type pending);
```

## `uv_write_cb`      

```c
typedef void (*uv_write_cb)(uv_write_t* req, int status);
```

## `uv_connect_cb`    

```c
typedef void (*uv_connect_cb)(uv_connect_t* req, int status);
```

## `uv_shutdown_cb`   

```c
typedef void (*uv_shutdown_cb)(uv_shutdown_t* req, int status);
```

## `uv_connection_cb` 

```c
typedef void (*uv_connection_cb)(uv_stream_t* server, int status);
```

## `uv_close_cb`      

```c
typedef void (*uv_close_cb)(uv_handle_t* handle);
```

## `uv_poll_cb`       

```c
typedef void (*uv_poll_cb)(uv_poll_t* handle, int status, int events);
```

## `uv_timer_cb`      

```c
typedef void (*uv_timer_cb)(uv_timer_t* handle, int status);
```


## `uv_async_cb`      

```c
typedef void (*uv_async_cb)(uv_async_t* handle, int status);
```

## `uv_prepare_cb`    

```c
typedef void (*uv_prepare_cb)(uv_prepare_t* handle,  int status);
```

## `uv_check_cb`      

```c
typedef void (*uv_check_cb)(uv_check_t* handle, int status);
```

## `uv_idle_cb`       

```c
typedef void (*uv_idle_cb)(uv_idle_t* handle, int status);
```

## `uv_exit_cb`       

```c
typedef void (*uv_exit_cb)(uv_process_t*, int64_t exit_status, int term_signal);
```

## `uv_walk_cb`       

```c
typedef void (*uv_walk_cb)(uv_handle_t* handle, void* arg);
```

## `uv_fs_cb`         

```c
typedef void (*uv_fs_cb)(uv_fs_t* req);
```

## `uv_work_cb`       

```c
typedef void (*uv_work_cb)(uv_work_t* req);
```

## `uv_after_work_cb` 

```c
typedef void (*uv_after_work_cb )(uv_work_t* req, int status);
```

## `uv_getaddrinfo_cb`

```c
typedef void (*uv_getaddrinfo_cb)(uv_getaddrinfo_t* req, int status, struct addrinfo* res);
```

## `uv_fs_event_cb`

```c
/*
* This will be called repeatedly after the uv_fs_event_t is initialized.
* If uv_fs_event_t was initialized with a directory the filename parameter
* will be a relative path to a file contained in the directory.
* The events parameter is an ORed mask of enum uv_fs_event elements.
*/
```

```c
typedef void (*uv_fs_event_cb)(uv_fs_event_t* handle, const char* filename,
    int events, int status);
```

## `uv_fs_poll_cb`

```c
typedef void (*uv_fs_poll_cb)(uv_fs_poll_t* handle,
                              int status,
                              const uv_stat_t* prev,
                              const uv_stat_t* curr);
```

## `uv_signal_cb`

```c
typedef void (*uv_signal_cb)(uv_signal_t* handle, int signum);
```

## `uv_udp_send_cb`

```c
/*
 * Called after a uv_udp_send() or uv_udp_send6(). status 0 indicates
 * success otherwise error.
 */
typedef void (*uv_udp_send_cb)(uv_udp_send_t* req, int status);
```

## `uv_udp_recv_cb`

```c
/*
 * Callback that is invoked when a new UDP datagram is received.
 *
 *  handle  UDP handle.
 *  nread   Number of bytes that have been received.
 *          0 if there is no more data to read. You may
 *          discard or repurpose the read buffer.
 *          < 0 if a transmission error was detected.
 *  buf     uv_buf_t with the received data.
 *  addr    struct sockaddr_in or struct sockaddr_in6.
 *          Valid for the duration of the callback only.
 *  flags   One or more OR'ed UV_UDP_* constants.
 *          Right now only UV_UDP_PARTIAL is used.
 */
typedef void (*uv_udp_recv_cb)(uv_udp_t* handle,
                               ssize_t nread,
                               const uv_buf_t* buf,
                               const struct sockaddr* addr,
                               unsigned flags);
```
