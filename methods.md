**Table of Contents**  *generated with [DocToc](http://doctoc.herokuapp.com/)*

- [loop](#loop)
	- [`uv_loop_new`](#uv_loop_new)
	- [`uv_loop_delete`](#uv_loop_delete)
	- [`uv_default_loop`](#uv_default_loop)
	- [`uv_run`](#uv_run)
	- [`uv_loop_alive`](#uv_loop_alive)
	- [`uv_stop`](#uv_stop)
	- [Examples](#examples)
	- [reference count](#reference-count)
		- [`uv_ref`](#uv_ref)
		- [`uv_unref`](#uv_unref)
		- [`uv_has_ref`](#uv_has_ref)
	- [time](#time)
		- [`uv_update_time`](#uv_update_time)
	- [backend - embedding loop in another loop](#backend---embedding-loop-in-another-loop)
		- [`uv_backend`](#uv_backend)
- [handles](#handles)
	- [`uv_handle_size`](#uv_handle_size)
	- [`uv_is_active`](#uv_is_active)
	- [`uv_walk`](#uv_walk)
	- [`uv_close`](#uv_close)
- [requests](#requests)
	- [`uv_req_size`](#uv_req_size)
- [buffers](#buffers)
	- [`uv_buf_init`](#uv_buf_init)
- [streams](#streams)
	- [`uv_listen`](#uv_listen)
	- [`uv_accept`](#uv_accept)
	- [`uv_read_start`](#uv_read_start)
	- [`uv_read_stop`](#uv_read_stop)
	- [`uv_read2_start`](#uv_read2_start)
	- [`uv_write`](#uv_write)
	- [`uv_write2`](#uv_write2)
	- [`uv_try_write`](#uv_try_write)
	- [`uv_is_readable`](#uv_is_readable)
	- [`uv_is_writable`](#uv_is_writable)
	- [`uv_stream_set_blocking`](#uv_stream_set_blocking)
	- [`uv_is_closing`](#uv_is_closing)
- [tcp](#tcp)
	- [`uv_tcp_init`](#uv_tcp_init)
	- [`uv_tcp_open`](#uv_tcp_open)
	- [`uv_tcp_nodelay`](#uv_tcp_nodelay)
	- [`uv_tcp_keepalive`](#uv_tcp_keepalive)
	- [`uv_tcp_simultaneous_accepts`](#uv_tcp_simultaneous_accepts)
	- [`uv_tcp_bind`](#uv_tcp_bind)
	- [`uv_tcp_getsockname`](#uv_tcp_getsockname)
	- [`uv_tcp_getpeername`](#uv_tcp_getpeername)
	- [`uv_tcp_connect`](#uv_tcp_connect)
- [udp](#udp)
	- [`uv_udp_init`](#uv_udp_init)
	- [`uv_udp_open`](#uv_udp_open)
	- [`uv_udp_bind`](#uv_udp_bind)
	- [`uv_udp_getsockname`](#uv_udp_getsockname)
	- [`uv_udp_set_membership`](#uv_udp_set_membership)
	- [`uv_udp_set_multicast_loop`](#uv_udp_set_multicast_loop)
	- [`uv_udp_set_multicast_ttl`](#uv_udp_set_multicast_ttl)
	- [`uv_udp_set_broadcast`](#uv_udp_set_broadcast)
	- [`uv_udp_set_ttl`](#uv_udp_set_ttl)
	- [`uv_udp_send`](#uv_udp_send)
	- [`uv_udp_recv_start`](#uv_udp_recv_start)
	- [`uv_udp_recv_stop`](#uv_udp_recv_stop)
- [tty](#tty)
	- [`uv_tty_init`](#uv_tty_init)
	- [`uv_tty_set_mode`](#uv_tty_set_mode)
	- [`uv_tty_reset_mode`](#uv_tty_reset_mode)
	- [`uv_tty_get_winsize`](#uv_tty_get_winsize)
- [pipe](#pipe)
	- [`uv_pipe_init`](#uv_pipe_init)
	- [`uv_pipe_open`](#uv_pipe_open)
	- [`uv_pipe_bind`](#uv_pipe_bind)
	- [`uv_pipe_connect`](#uv_pipe_connect)
	- [`uv_pipe_pending_instances`](#uv_pipe_pending_instances)
- [poll](#poll)
	- [`uv_poll_init`](#uv_poll_init)
	- [`uv_poll_init_socket`](#uv_poll_init_socket)
	- [`uv_poll_start`](#uv_poll_start)
	- [`uv_poll_stop`](#uv_poll_stop)
- [prepare](#prepare)
	- [`uv_prepare_init`](#uv_prepare_init)
	- [`uv_prepare_start`](#uv_prepare_start)
	- [`uv_prepare_stop`](#uv_prepare_stop)
- [idle](#idle)
	- [`uv_idle_init`](#uv_idle_init)
	- [`uv_idle_start`](#uv_idle_start)
	- [`uv_idle_stop`](#uv_idle_stop)
- [async](#async)
	- [`uv_async_init`](#uv_async_init)
	- [`uv_async_send`](#uv_async_send)
- [timer](#timer)
	- [`uv_timer_init`](#uv_timer_init)
	- [`uv_timer_start`](#uv_timer_start)
	- [`uv_timer_stop`](#uv_timer_stop)
	- [`uv_timer_again`](#uv_timer_again)
	- [`uv_timer_set_repeat`](#uv_timer_set_repeat)
	- [`uv_timer_get_repeat`](#uv_timer_get_repeat)
- [addrinfo](#addrinfo)
	- [`uv_getaddrinfo`](#uv_getaddrinfo)
	- [`uv_freeaddrinfo`](#uv_freeaddrinfo)
- [process](#process)
	- [`uv_spawn`](#uv_spawn)
	- [`uv_process_kill`](#uv_process_kill)
	- [`uv_kill`](#uv_kill)
	- [`uv_setup_args`](#uv_setup_args)
	- [`uv_get_process_title`](#uv_get_process_title)
	- [`uv_set_process_title`](#uv_set_process_title)
	- [`uv_resident_set_memory`](#uv_resident_set_memory)
	- [`uv_uptime`](#uv_uptime)
- [work queue](#work-queue)
	- [`uv_queue_work`](#uv_queue_work)
	- [`uv_cancel`](#uv_cancel)
- [cpu info](#cpu-info)
	- [`uv_cpu_info`](#uv_cpu_info)
	- [`uv_free_cpu_info`](#uv_free_cpu_info)
- [interface addresses](#interface-addresses)
	- [`uv_interface_addresses`](#uv_interface_addresses)
	- [`uv_free_interface_addresses`](#uv_free_interface_addresses)
- [file system](#file-system)
	- [`uv_fs_req_cleanup`](#uv_fs_req_cleanup)
	- [`uv_fs_close`](#uv_fs_close)
	- [`uv_fs_open`](#uv_fs_open)
		- [`uv_fs_t *req` passed to callback](#uv_fs_t-req-passed-to-callback)
	- [`uv_fs_read`](#uv_fs_read)
		- [`uv_fs_t *req` passed to callback](#uv_fs_t-req-passed-to-callback-1)
	- [`uv_fs_unlink`](#uv_fs_unlink)
	- [`uv_fs_write`](#uv_fs_write)
		- [`uv_fs_t *req` passed to callback](#uv_fs_t-req-passed-to-callback-2)
	- [`uv_fs_mkdir`](#uv_fs_mkdir)
	- [`uv_fs_rmdir`](#uv_fs_rmdir)
	- [`uv_fs_readdir`](#uv_fs_readdir)
	- [`uv_fs_stat`](#uv_fs_stat)
	- [`uv_fs_fstat`](#uv_fs_fstat)
	- [`uv_fs_rename`](#uv_fs_rename)
	- [`uv_fs_fsync`](#uv_fs_fsync)
	- [`uv_fs_fdatasync`](#uv_fs_fdatasync)
	- [`uv_fs_ftruncate`](#uv_fs_ftruncate)
	- [`uv_fs_sendfile`](#uv_fs_sendfile)
	- [`uv_fs_chmod`](#uv_fs_chmod)
	- [`uv_fs_utime`](#uv_fs_utime)
	- [`uv_fs_futime`](#uv_fs_futime)
	- [`uv_fs_lstat`](#uv_fs_lstat)
	- [`uv_fs_link`](#uv_fs_link)
	- [`uv_fs_symlink`](#uv_fs_symlink)
	- [`uv_fs_readlink`](#uv_fs_readlink)
	- [`uv_fs_fchmod`](#uv_fs_fchmod)
	- [`uv_fs_chown`](#uv_fs_chown)
	- [`uv_fs_fchown`](#uv_fs_fchown)
	- [`uv_guess_handle`](#uv_guess_handle)
	- [fs poll](#fs-poll)
		- [`uv_fs_poll_init`](#uv_fs_poll_init)
		- [`uv_fs_poll_start`](#uv_fs_poll_start)
		- [`uv_fs_poll_stop`](#uv_fs_poll_stop)
	- [fs event](#fs-event)
		- [`uv_fs_event_init`](#uv_fs_event_init)
		- [`uv_fs_event_start`](#uv_fs_event_start)
		- [`uv_fs_event_stop`](#uv_fs_event_stop)
- [signal](#signal)
	- [`uv_signal_init`](#uv_signal_init)
	- [`uv_signal_start`](#uv_signal_start)
	- [`uv_signal_stop`](#uv_signal_stop)
- [load average](#load-average)
	- [`uv_loadavg`](#uv_loadavg)
- [errors](#errors)
	- [`uv_strerror`](#uv_strerror)
	- [`uv_err_name`](#uv_err_name)
- [utilities](#utilities)
	- [ip address conversion](#ip-address-conversion)
		- [`uv_ip4_addr`](#uv_ip4_addr)
		- [`uv_ip6_addr`](#uv_ip6_addr)
		- [`uv_ip4_name`](#uv_ip4_name)
		- [`uv_ip6_name`](#uv_ip6_name)
	- [ntop and pton](#ntop-and-pton)
		- [`uv_inet_ntop`](#uv_inet_ntop)
		- [`uv_inet_pton`](#uv_inet_pton)
	- [path](#path)
		- [`uv_exepath`](#uv_exepath)
		- [`uv_cwd`](#uv_cwd)
		- [`uv_chdir`](#uv_chdir)
	- [memory](#memory)
		- [`uv_get_free_memory`](#uv_get_free_memory)
		- [`uv_get_total_memory`](#uv_get_total_memory)
	- [time](#time-1)
		- [`uv_hrtime`](#uv_hrtime)
	- [stdio](#stdio)
		- [`uv_disable_stdio_inheritance`](#uv_disable_stdio_inheritance)
	- [dynamic library access](#dynamic-library-access)
		- [`uv_dlopen`](#uv_dlopen)
		- [`uv_dlclose`](#uv_dlclose)
		- [`uv_dlsym`](#uv_dlsym)
		- [`uv_dlerror`](#uv_dlerror)
	- [mutex](#mutex)
		- [`uv_mutex_init`](#uv_mutex_init)
		- [`uv_mutex_destroy`](#uv_mutex_destroy)
		- [`uv_mutex_lock`](#uv_mutex_lock)
		- [`uv_mutex_trylock`](#uv_mutex_trylock)
		- [`uv_mutex_unlock`](#uv_mutex_unlock)
	- [rwlock](#rwlock)
		- [`uv_rwlock_init`](#uv_rwlock_init)
		- [`uv_rwlock_destroy`](#uv_rwlock_destroy)
		- [`uv_rwlock_rdlock`](#uv_rwlock_rdlock)
		- [`uv_rwlock_tryrdlock`](#uv_rwlock_tryrdlock)
		- [`uv_rwlock_rdunlock`](#uv_rwlock_rdunlock)
		- [`uv_rwlock_wrlock`](#uv_rwlock_wrlock)
		- [`uv_rwlock_trywrlock`](#uv_rwlock_trywrlock)
		- [`uv_rwlock_wrunlock`](#uv_rwlock_wrunlock)
	- [semaphores](#semaphores)
		- [`uv_sem_init`](#uv_sem_init)
		- [`uv_sem_destroy`](#uv_sem_destroy)
		- [`uv_sem_post`](#uv_sem_post)
		- [`uv_sem_wait`](#uv_sem_wait)
		- [`uv_sem_trywait`](#uv_sem_trywait)
	- [condition variables](#condition-variables)
		- [`uv_cond_init`](#uv_cond_init)
		- [`uv_cond_destroy`](#uv_cond_destroy)
		- [`uv_cond_signal`](#uv_cond_signal)
		- [`uv_cond_broadcast`](#uv_cond_broadcast)
		- [`uv_cond_wait`](#uv_cond_wait)
		- [`uv_cond_timedwait`](#uv_cond_timedwait)
	- [barrier](#barrier)
		- [`uv_barrier_init`](#uv_barrier_init)
		- [`uv_barrier_destroy`](#uv_barrier_destroy)
		- [`uv_barrier_wait`](#uv_barrier_wait)
	- [once](#once)
		- [`uv_once`](#uv_once)
	- [thread local storage](#thread-local-storage)
		- [`uv_key_create`](#uv_key_create)
		- [`uv_key_delete`](#uv_key_delete)
		- [`uv_key_get`](#uv_key_get)
		- [`uv_key_set`](#uv_key_set)
		- [`uv_thread_create`](#uv_thread_create)
		- [`long `](#long-)
		- [`uv_thread_join`](#uv_thread_join)

# loop


## `uv_loop_new`

```c
uv_loop_t* uv_loop_new(void)
```

## `uv_loop_delete`

```c
void uv_loop_delete(uv_loop_t*);
```

## `uv_default_loop`

```c
uv_loop_t* uv_default_loop(void);
```

## `uv_run`

```c
/*
 * This function runs the event loop. It will act differently depending on the
 * specified mode:
 *  - UV_RUN_DEFAULT: Runs the event loop until the reference count drops to
 *    zero. Always returns zero.
 *  - UV_RUN_ONCE: Poll for new events once. Note that this function blocks if
 *    there are no pending events. Returns zero when done (no active handles
 *    or requests left), or non-zero if more events are expected (meaning you
 *    should run the event loop again sometime in the future).
 *  - UV_RUN_NOWAIT: Poll for new events once but don't block if there are no
 *    pending events.
 */
int uv_run(uv_loop_t*, uv_run_mode mode);
```

## `uv_loop_alive`

```c
/*
 * This function checks whether the reference count, the number of active
 * handles or requests left in the event loop, is non-zero.
 */
int uv_loop_alive(const uv_loop_t* loop);
```

## `uv_stop`

```c
/*
 * This function will stop the event loop by forcing uv_run to end
 * as soon as possible, but not sooner than the next loop iteration.
 * If this function was called before blocking for i/o, the loop won't
 * block for i/o on this iteration.
 */
void uv_stop(uv_loop_t*);
```

## Examples

```c
uv_loop_t *loop = uv_loop_new();

printf("Now quitting.\n");
uv_run(loop, UV_RUN_DEFAULT);
```

```c
uv_idle_t idler;

uv_loop_t *loop = uv_default_loop();
uv_idle_init(loop, &idler);
uv_idle_start(&idler, wait_for_a_while);

printf("Idling...\n");
uv_run(loop, UV_RUN_DEFAULT);
```

## reference count

### `uv_ref`

```c
/*
 * Manually modify the event loop's reference count. Useful if the user wants
 * to have a handle or timeout that doesn't keep the loop alive.
 */
void uv_ref(uv_handle_t*);
```

### `uv_unref`

```c
void uv_unref(uv_handle_t*);
```

### `uv_has_ref`

```c
int uv_has_ref(const uv_handle_t*);
```

## time

### `uv_update_time`

```c
/*
 * Update the event loop's concept of "now". Libuv caches the current time
 * at the start of the event loop tick in order to reduce the number of
 * time-related system calls.
 *
 * You won't normally need to call this function unless you have callbacks
 * that block the event loop for longer periods of time, where "longer" is
 * somewhat subjective but probably on the order of a millisecond or more.
 */
void uv_update_time(uv_loop_t*);

### `uv_now`

/*
 * Return the current timestamp in milliseconds. The timestamp is cached at
 * the start of the event loop tick, see |uv_update_time()| for details and
 * rationale.
 *
 * The timestamp increases monotonically from some arbitrary point in time.
 * Don't make assumptions about the starting point, you will only get
 * disappointed.
 *
 * Use uv_hrtime() if you need sub-millisecond granularity.
 */
uint64_t uv_now(uv_loop_t*);
```

## backend - embedding loop in another loop

### `uv_backend`

```c
/*
 * Get backend file descriptor. Only kqueue, epoll and event ports are
 * supported.
 *
 * This can be used in conjunction with `uv_run(loop, UV_RUN_NOWAIT)` to
 * poll in one thread and run the event loop's event callbacks in another.
 *
 * Useful for embedding libuv's event loop in another event loop.
 * See test/test-embed.c for an example.
 *
 * Note that embedding a kqueue fd in another kqueue pollset doesn't work on
 * all platforms. It's not an error to add the fd but it never generates
 * events.
 */
int uv_backend_fd(const uv_loop_t*);

### `uv_backend_timeout`

/*
 * Get the poll timeout. The return value is in milliseconds, or -1 for no
 * timeout.
 */
int uv_backend_timeout(const uv_loop_t*);
```

# handles

## `uv_handle_size`

```c
/*
 * Returns size of various handle types, useful for FFI
 * bindings to allocate correct memory without copying struct
 * definitions
 */
size_t uv_handle_size(uv_handle_type type);
```

## `uv_is_active`

```c
/*
 * Returns non-zero if the handle is active, zero if it's inactive.
 *
 * What "active" means depends on the type of handle:
 *
 *  - A uv_async_t handle is always active and cannot be deactivated, except
 *    by closing it with uv_close().
 *
 *  - A uv_pipe_t, uv_tcp_t, uv_udp_t, etc. handle - basically any handle that
 *    deals with I/O - is active when it is doing something that involves I/O,
 *    like reading, writing, connecting, accepting new connections, etc.
 *
 *  - A uv_check_t, uv_idle_t, uv_timer_t, etc. handle is active when it has
 *    been started with a call to uv_check_start(), uv_idle_start(), etc.
 *
 *      Rule of thumb: if a handle of type uv_foo_t has a uv_foo_start()
 *      function, then it's active from the moment that function is called.
 *      Likewise, uv_foo_stop() deactivates the handle again.
 *
 */
int uv_is_active(const uv_handle_t* handle);
```

## `uv_walk`

```c
/*
 * Walk the list of open handles.
 */
void uv_walk(uv_loop_t* loop, uv_walk_cb walk_cb, void* arg);
```

## `uv_close`

```c
/*
 * Request handle to be closed. close_cb will be called asynchronously after
 * this call. This MUST be called on each handle before memory is released.
 *
 * Note that handles that wrap file descriptors are closed immediately but
 * close_cb will still be deferred to the next iteration of the event loop.
 * It gives you a chance to free up any resources associated with the handle.
 *
 * In-progress requests, like uv_connect_t or uv_write_t, are cancelled and
 * have their callbacks called asynchronously with status=UV_ECANCELED.
 */
void uv_close(uv_handle_t* handle, uv_close_cb close_cb);
```

# requests

## `uv_req_size`

```c
/*
 * Returns size of request types, useful for dynamic lookup with FFI
 */
size_t uv_req_size(uv_req_type type);
```

# buffers

## `uv_buf_init`

```c
/*
 * Constructor for uv_buf_t.
 * Due to platform differences the user cannot rely on the ordering of the
 * base and len members of the uv_buf_t struct. The user is responsible for
 * freeing base after the uv_buf_t is done. Return struct passed by value.
 */
uv_buf_t uv_buf_init(char* base, unsigned int len);
```

# streams

## `uv_listen`

```c
int uv_listen(uv_stream_t* stream, int backlog, uv_connection_cb cb);
```

## `uv_accept`

```c

/*
 * This call is used in conjunction with uv_listen() to accept incoming
 * connections. Call uv_accept after receiving a uv_connection_cb to accept
 * the connection. Before calling uv_accept use uv_*_init() must be
 * called on the client. Non-zero return value indicates an error.
 *
 * When the uv_connection_cb is called it is guaranteed that uv_accept will
 * complete successfully the first time. If you attempt to use it more than
 * once, it may fail. It is suggested to only call uv_accept once per
 * uv_connection_cb call.
 */

int uv_accept(uv_stream_t* server, uv_stream_t* client);
```

## `uv_read_start`

```c
/*
 * Read data from an incoming stream. The callback will be made several
 * times until there is no more data to read or uv_read_stop is called.
 * When we've reached EOF nread will be set to UV_EOF.
 *
 * When nread < 0, the buf parameter might not point to a valid buffer;
 * in that case buf.len and buf.base are both set to 0.
 *
 * Note that nread might also be 0, which does *not* indicate an error or
 * eof; it happens when libuv requested a buffer through the alloc callback
 * but then decided that it didn't need that buffer.
 */
int uv_read_start(uv_stream_t*, uv_alloc_cb alloc_cb, uv_read_cb read_cb);
```

## `uv_read_stop`

```c
int uv_read_stop(uv_stream_t*);
```

## `uv_read2_start`

```c
/*
 * Extended read methods for receiving handles over a pipe. The pipe must be
 * initialized with ipc == 1.
 */
int uv_read2_start(uv_stream_t*, uv_alloc_cb alloc_cb, uv_read2_cb read_cb);
```

## `uv_write`

```c
/*
 * Write data to stream. Buffers are written in order. Example:
 *
 *   uv_buf_t a[] = {
 *     { .base = "1", .len = 1 },
 *     { .base = "2", .len = 1 }
 *   };
 *
 *   uv_buf_t b[] = {
 *     { .base = "3", .len = 1 },
 *     { .base = "4", .len = 1 }
 *   };
 *
 *   uv_write_t req1;
 *   uv_write_t req2;
 *
 *   // writes "1234"
 *   uv_write(&req1, stream, a, 2);
 *   uv_write(&req2, stream, b, 2);
 *
 */
int uv_write(uv_write_t* req,
             uv_stream_t* handle,
             const uv_buf_t bufs[],
             unsigned int nbufs,
             uv_write_cb cb);
```

## `uv_write2`

```c
/*
 * Extended write function for sending handles over a pipe. The pipe must be
 * initialized with ipc == 1.
 * send_handle must be a TCP socket or pipe, which is a server or a connection
 * (listening or connected state).  Bound sockets or pipes will be assumed to
 * be servers.
 */
int uv_write2(uv_write_t* req,
              uv_stream_t* handle,
              const uv_buf_t bufs[],
              unsigned int nbufs,
              uv_stream_t* send_handle,
              uv_write_cb cb);
```

## `uv_try_write`

```c
/*
 * Same as `uv_write()`, but won't queue write request if it can't be completed
 * immediately.
 * Will return either:
 * - positive number of bytes written
 * - zero - if queued write is needed
 * - negative error code
 */
int uv_try_write(uv_stream_t* handle,
                 const uv_buf_t bufs[],
                 unsigned int nbufs);
```

## `uv_is_readable`

```c
int uv_is_readable(const uv_stream_t* handle);
```

## `uv_is_writable`

```c
int uv_is_writable(const uv_stream_t* handle);
```

## `uv_stream_set_blocking`

```c
/*
 * Enable or disable blocking mode for a stream.
 *
 * When blocking mode is enabled all writes complete synchronously. The
 * interface remains unchanged otherwise, e.g. completion or failure of the
 * operation will still be reported through a callback which is made
 * asychronously.
 *
 * Relying too much on this API is not recommended. It is likely to change
 * significantly in the future.
 *
 * On windows this currently works only for uv_pipe_t instances. On unix it
 * works for tcp, pipe and tty instances. Be aware that changing the blocking
 * mode on unix sets or clears the O_NONBLOCK bit. If you are sharing a handle
 * with another process, the other process is affected by the change too,
 * which can lead to unexpected results.
 *
 * Also libuv currently makes no ordering guarantee when the blocking mode
 * is changed after write requests have already been submitted. Therefore it is
 * recommended to set the blocking mode immediately after opening or creating
 * the stream.
 */
int uv_stream_set_blocking(uv_stream_t* handle, int blocking);
```

## `uv_is_closing`

```c
/*
 * Used to determine whether a stream is closing or closed.
 *
 * N.B. is only valid between the initialization of the handle
 *      and the arrival of the close callback, and cannot be used
 *      to validate the handle.
 */
int uv_is_closing(const uv_handle_t* handle);
```

# tcp

## `uv_tcp_init`

```c
int uv_tcp_init(uv_loop_t*, uv_tcp_t* handle);
```

## `uv_tcp_open`

```c
/*
 * Opens an existing file descriptor or SOCKET as a tcp handle.
 */
int uv_tcp_open(uv_tcp_t* handle, uv_os_sock_t sock);
```

## `uv_tcp_nodelay`

```c
/* Enable/disable Nagle's algorithm. */
int uv_tcp_nodelay(uv_tcp_t* handle, int enable);
```

## `uv_tcp_keepalive`

```c
/*
 * Enable/disable TCP keep-alive.
 *
 * `delay` is the initial delay in seconds, ignored when `enable` is zero.
 */
int uv_tcp_keepalive(uv_tcp_t* handle, int enable, unsigned int delay);
```
## `uv_tcp_simultaneous_accepts`

```c
/*
 * Enable/disable simultaneous asynchronous accept requests that are
 * queued by the operating system when listening for new tcp connections.
 * This setting is used to tune a tcp server for the desired performance.
 * Having simultaneous accepts can significantly improve the rate of
 * accepting connections (which is why it is enabled by default) but
 * may lead to uneven load distribution in multi-process setups.
 */
int uv_tcp_simultaneous_accepts(uv_tcp_t* handle, int enable);
```

## `uv_tcp_bind`

```c
/*
 * Bind the handle to an address and port.  `addr` should point to an
 * initialized struct sockaddr_in or struct sockaddr_in6.
 *
 * When the port is already taken, you can expect to see an UV_EADDRINUSE
 * error from either uv_tcp_bind(), uv_listen() or uv_tcp_connect().
 *
 * That is, a successful call to uv_tcp_bind() does not guarantee that
 * the call to uv_listen() or uv_tcp_connect() will succeed as well.
 */
int uv_tcp_bind(uv_tcp_t* handle, const struct sockaddr* addr);
```

## `uv_tcp_getsockname`

```c
int uv_tcp_getsockname(uv_tcp_t* handle, struct sockaddr* name, int* namelen);
```

## `uv_tcp_getpeername`

```c
int uv_tcp_getpeername(uv_tcp_t* handle, struct sockaddr* name, int* namelen);
```

## `uv_tcp_connect`

```c
/*
 * Establish an IPv4 or IPv6 TCP connection.  Provide an initialized TCP handle
 * and an uninitialized uv_connect_t*.  `addr` should point to an initialized
 * struct sockaddr_in or struct sockaddr_in6.
 *
 * The callback is made when the connection has been established or when a
 * connection error happened.
 */
int uv_tcp_connect(uv_connect_t* req, uv_tcp_t* handle, const struct sockaddr* addr, uv_connect_cb cb);
```

# udp

## `uv_udp_init`

```c
/*
 * Initialize a new UDP handle. The actual socket is created lazily.
 * Returns 0 on success.
 */
int uv_udp_init(uv_loop_t*, uv_udp_t* handle);
```

## `uv_udp_open`

```c
/*
 * Opens an existing file descriptor or SOCKET as a udp handle.
 *
 * Unix only:
 *  The only requirement of the sock argument is that it follows the
 *  datagram contract (works in unconnected mode, supports sendmsg()/recvmsg(),
 *  etc.). In other words, other datagram-type sockets like raw sockets or
 *  netlink sockets can also be passed to this function.
 *
 * This sets the SO_REUSEPORT socket flag on the BSDs and OS X. On other
 * UNIX platforms, it sets the SO_REUSEADDR flag.  What that means is that
 * multiple threads or processes can bind to the same address without error
 * (provided they all set the flag) but only the last one to bind will receive
 * any traffic, in effect "stealing" the port from the previous listener.
 * This behavior is something of an anomaly and may be replaced by an explicit
 * opt-in mechanism in future versions of libuv.
 */
int uv_udp_open(uv_udp_t* handle, uv_os_sock_t sock);
```

## `uv_udp_bind`

```c
/*
 * Bind to a IPv4 address and port.
 *
 * Arguments:
 *  handle    UDP handle. Should have been initialized with `uv_udp_init`.
 *  addr      struct sockaddr_in or struct sockaddr_in6 with the address and
 *            port to bind to.
 *  flags     Unused.
 *
 * Returns:
 *  0 on success, or an error code < 0 on failure.
 *
 * This sets the SO_REUSEPORT socket flag on the BSDs and OS X. On other
 * UNIX platforms, it sets the SO_REUSEADDR flag.  What that means is that
 * multiple threads or processes can bind to the same address without error
 * (provided they all set the flag) but only the last one to bind will receive
 * any traffic, in effect "stealing" the port from the previous listener.
 * This behavior is something of an anomaly and may be replaced by an explicit
 * opt-in mechanism in future versions of libuv.
 */
int uv_udp_bind(uv_udp_t* handle, const struct sockaddr* addr, unsigned int flags);
```

## `uv_udp_getsockname`

```c
int uv_udp_getsockname(uv_udp_t* handle, struct sockaddr* name, int* namelen);
```

## `uv_udp_set_membership`

```c
/*
 * Set membership for a multicast address
 *
 * Arguments:
 *  handle              UDP handle. Should have been initialized with
 *                      `uv_udp_init`.
 *  multicast_addr      multicast address to set membership for
 *  interface_addr      interface address
 *  membership          Should be UV_JOIN_GROUP or UV_LEAVE_GROUP
 *
 * Returns:
 *  0 on success, or an error code < 0 on failure.
 */
int uv_udp_set_membership(uv_udp_t* handle,
                          const char* multicast_addr, 
                          const char* interface_addr,
                          uv_membership membership);
```

## `uv_udp_set_multicast_loop`

```c
/*
 * Set IP multicast loop flag. Makes multicast packets loop back to
 * local sockets.
 *
 * Arguments:
 *  handle              UDP handle. Should have been initialized with
 *                      `uv_udp_init`.
 *  on                  1 for on, 0 for off
 *
 * Returns:
 *  0 on success, or an error code < 0 on failure.
 */
int uv_udp_set_multicast_loop(uv_udp_t* handle, int on);
```

## `uv_udp_set_multicast_ttl`

```c
/*
 * Set the multicast ttl
 *
 * Arguments:
 *  handle              UDP handle. Should have been initialized with
 *                      `uv_udp_init`.
 *  ttl                 1 through 255
 *
 * Returns:
 *  0 on success, or an error code < 0 on failure.
 */
int uv_udp_set_multicast_ttl(uv_udp_t* handle, int ttl);
```

## `uv_udp_set_broadcast`

```c
/*
 * Set broadcast on or off
 *
 * Arguments:
 *  handle              UDP handle. Should have been initialized with
 *                      `uv_udp_init`.
 *  on                  1 for on, 0 for off
 *
 * Returns:
 *  0 on success, or an error code < 0 on failure.
 */
int uv_udp_set_broadcast(uv_udp_t* handle, int on);
```

## `uv_udp_set_ttl`

```c
/*
 * Set the time to live
 *
 * Arguments:
 *  handle              UDP handle. Should have been initialized with
 *                      `uv_udp_init`.
 *  ttl                 1 through 255
 *
 * Returns:
 *  0 on success, or an error code < 0 on failure.
 */
int uv_udp_set_ttl(uv_udp_t* handle, int ttl);
```

## `uv_udp_send`

```c
/*
 * Send data. If the socket has not previously been bound with `uv_udp_bind`
 * or `uv_udp_bind6`, it is bound to 0.0.0.0 (the "all interfaces" address)
 * and a random port number.
 *
 * Arguments:
 *  req       UDP request handle. Need not be initialized.
 *  handle    UDP handle. Should have been initialized with `uv_udp_init`.
 *  bufs      List of buffers to send.
 *  nbufs     Number of buffers in `bufs`.
 *  addr      Address of the remote peer. See `uv_ip4_addr`.
 *  send_cb   Callback to invoke when the data has been sent out.
 *
 * Returns:
 *  0 on success, or an error code < 0 on failure.
 */
int uv_udp_send(uv_udp_send_t* req,
                uv_udp_t* handle,
                const uv_buf_t bufs[],
                unsigned int nbufs,
                const struct sockaddr* addr,
                uv_udp_send_cb send_cb);
```

## `uv_udp_recv_start`

```c
/*
 * Receive data. If the socket has not previously been bound with `uv_udp_bind`
 * or `uv_udp_bind6`, it is bound to 0.0.0.0 (the "all interfaces" address)
 * and a random port number.
 *
 * Arguments:
 *  handle    UDP handle. Should have been initialized with `uv_udp_init`.
 *  alloc_cb  Callback to invoke when temporary storage is needed.
 *  recv_cb   Callback to invoke with received data.
 *
 * Returns:
 *  0 on success, or an error code < 0 on failure.
 */
int uv_udp_recv_start(uv_udp_t* handle, uv_alloc_cb alloc_cb, uv_udp_recv_cb recv_cb);
```

## `uv_udp_recv_stop`

```c
/*
 * Stop listening for incoming datagrams.
 *
 * Arguments:
 *  handle    UDP handle. Should have been initialized with `uv_udp_init`.
 *
 * Returns:
 *  0 on success, or an error code < 0 on failure.
 */
int uv_udp_recv_stop(uv_udp_t* handle);
```

# tty

## `uv_tty_init`

```c
/*
 * Initialize a new TTY stream with the given file descriptor. Usually the
 * file descriptor will be
 *   0 = stdin
 *   1 = stdout
 *   2 = stderr
 * The last argument, readable, specifies if you plan on calling
 * uv_read_start with this stream. stdin is readable, stdout is not.
 *
 * TTY streams which are not readable have blocking writes.
 */
int uv_tty_init(uv_loop_t*, uv_tty_t*, uv_file fd, int readable);
```

## `uv_tty_set_mode`

```c
/*
 * Set mode. 0 for normal, 1 for raw.
 */
int uv_tty_set_mode(uv_tty_t*, int mode);
```

## `uv_tty_reset_mode`

```c
/*
 * To be called when the program exits. Resets TTY settings to default
 * values for the next process to take over.
 *
 * This function is async signal-safe on UNIX platforms but can fail with error
 * code UV_EBUSY if you call it when execution is inside uv_tty_set_mode().
 */
int uv_tty_reset_mode(void);
```

## `uv_tty_get_winsize`

```c
/*
 * Gets the current Window size. On success zero is returned.
 */
int uv_tty_get_winsize(uv_tty_t*, int* width, int* height);
```

# pipe

## `uv_pipe_init`

```c
/*
 * Initialize a pipe. The last argument is a boolean to indicate if
 * this pipe will be used for handle passing between processes.
 */
int uv_pipe_init(uv_loop_t*, uv_pipe_t* handle, int ipc);
```

## `uv_pipe_open`

```c
/*
 * Opens an existing file descriptor or HANDLE as a pipe.
 */
int uv_pipe_open(uv_pipe_t*, uv_file file);
```

## `uv_pipe_bind`

```c
/*
 * Bind the pipe to a file path (UNIX) or a name (Windows.)
 *
 * Paths on UNIX get truncated to `sizeof(sockaddr_un.sun_path)` bytes,
 * typically between 92 and 108 bytes.
 */
int uv_pipe_bind(uv_pipe_t* handle, const char* name);
```

## `uv_pipe_connect`

```c
/*
 * Connect to the UNIX domain socket or the named pipe.
 *
 * Paths on UNIX get truncated to `sizeof(sockaddr_un.sun_path)` bytes,
 * typically between 92 and 108 bytes.
 */
void uv_pipe_connect(uv_connect_t* req, uv_pipe_t* handle, const char* name, uv_connect_cb cb);
```

## `uv_pipe_pending_instances`

```c
/*
 * This setting applies to Windows only.
 * Set the number of pending pipe instance handles when the pipe server
 * is waiting for connections.
 */
void uv_pipe_pending_instances(uv_pipe_t* handle, int count);
```

# poll

## `uv_poll_init`

```c
/* Initialize the poll watcher using a file descriptor. */
int uv_poll_init(uv_loop_t* loop, uv_poll_t* handle, int fd);
```

## `uv_poll_init_socket`

```c
/* Initialize the poll watcher using a socket descriptor. On unix this is */
/* identical to uv_poll_init. On windows it takes a SOCKET handle. */
int uv_poll_init_socket(uv_loop_t* loop, uv_poll_t* handle, uv_os_sock_t socket);
```

## `uv_poll_start`

```c
/*
 * Starts polling the file descriptor. `events` is a bitmask consisting made up
 * of UV_READABLE and UV_WRITABLE. As soon as an event is detected the callback
 * will be called with `status` set to 0, and the detected events set en the
 * `events` field.
 *
 * If an error happens while polling status, `status` < 0 and corresponds
 * with one of the UV_E* error codes. The user should not close the socket
 * while uv_poll is active. If the user does that anyway, the callback *may*
 * be called reporting an error status, but this is not guaranteed.
 *
 * Calling uv_poll_start on an uv_poll watcher that is already active is fine.
 * Doing so will update the events mask that is being watched for.
 */
int uv_poll_start(uv_poll_t* handle, int events, uv_poll_cb cb);
```

## `uv_poll_stop`

```c
/* Stops polling the file descriptor. */
int uv_poll_stop(uv_poll_t* handle);
```

# prepare

## `uv_prepare_init`

```c
int uv_prepare_init(uv_loop_t*, uv_prepare_t* prepare);
```

## `uv_prepare_start`

```c
int uv_prepare_start(uv_prepare_t* prepare, uv_prepare_cb cb);
```

## `uv_prepare_stop`

```c
int uv_prepare_stop(uv_prepare_t* prepare);
```

# idle

## `uv_idle_init`

```c
int uv_idle_init(uv_loop_t*, uv_idle_t* idle);
```

## `uv_idle_start`

```c
int uv_idle_start(uv_idle_t* idle, uv_idle_cb cb);
```

## `uv_idle_stop`

```c
int uv_idle_stop(uv_idle_t* idle);
```

# async

## `uv_async_init`

```c
/*
 * Initialize the uv_async_t handle. A NULL callback is allowed.
 *
 * Note that uv_async_init(), unlike other libuv functions, immediately
 * starts the handle. To stop the handle again, close it with uv_close().
 */
int uv_async_init(uv_loop_t*, uv_async_t* async, uv_async_cb async_cb);
```

## `uv_async_send`

```c
/*
 * This can be called from other threads to wake up a libuv thread.
 *
 * libuv is single threaded at the moment.
 */
int uv_async_send(uv_async_t* async);
```

# timer

## `uv_timer_init`

```c
int uv_timer_init(uv_loop_t*, uv_timer_t* handle);
```

## `uv_timer_start`

```c
/*
 * Start the timer. `timeout` and `repeat` are in milliseconds.
 *
 * If timeout is zero, the callback fires on the next tick of the event loop.
 *
 * If repeat is non-zero, the callback fires first after timeout milliseconds
 * and then repeatedly after repeat milliseconds.
 */
int uv_timer_start(uv_timer_t* handle,
                   uv_timer_cb cb,
                   uint64_t timeout,
                   uint64_t repeat);
```

## `uv_timer_stop`

```c
int uv_timer_stop(uv_timer_t* handle);
```

## `uv_timer_again`

```c
/*
 * Stop the timer, and if it is repeating restart it using the repeat value
 * as the timeout. If the timer has never been started before it returns
 * UV_EINVAL.
 */
int uv_timer_again(uv_timer_t* handle);
```

## `uv_timer_set_repeat`

```c
/*
 * Set the repeat value in milliseconds. Note that if the repeat value is set
 * from a timer callback it does not immediately take effect. If the timer was
 * non-repeating before, it will have been stopped. If it was repeating, then
 * the old repeat value will have been used to schedule the next timeout.
 */
void uv_timer_set_repeat(uv_timer_t* handle, uint64_t repeat);
```

## `uv_timer_get_repeat`

```c
uint64_t uv_timer_get_repeat(const uv_timer_t* handle);
```

# addrinfo

## `uv_getaddrinfo`

```c
/*
 * Asynchronous getaddrinfo(3).
 *
 * Either node or service may be NULL but not both.
 *
 * hints is a pointer to a struct addrinfo with additional address type
 * constraints, or NULL. Consult `man -s 3 getaddrinfo` for details.
 *
 * Returns 0 on success or an error code < 0 on failure.
 *
 * If successful, your callback gets called sometime in the future with the
 * lookup result, which is either:
 *
 *  a) err == 0, the res argument points to a valid struct addrinfo, or
 *  b) err < 0, the res argument is NULL. See the UV_EAI_* constants.
 *
 * Call uv_freeaddrinfo() to free the addrinfo structure.
 */
int uv_getaddrinfo(uv_loop_t* loop,
                    uv_getaddrinfo_t* req,
                    uv_getaddrinfo_cb getaddrinfo_cb,
                    const char* node,
                    const char* service,
                    const struct addrinfo* hints);
```

## `uv_freeaddrinfo`

```c
/*
 * Free the struct addrinfo. Passing NULL is allowed and is a no-op.
 */
void uv_freeaddrinfo(struct addrinfo* ai);
```

# process

## `uv_spawn`

```c
/*
 * Initializes the uv_process_t and starts the process. If the process is
 * successfully spawned, then this function will return 0. Otherwise, the
 * negative error code corresponding to the reason it couldn't spawn is
 * returned.
 *
 * Possible reasons for failing to spawn would include (but not be limited to)
 * the file to execute not existing, not having permissions to use the setuid or
 * setgid specified, or not having enough memory to allocate for the new
 * process.
 */
int uv_spawn(uv_loop_t* loop,
             uv_process_t* handle,
             const uv_process_options_t* options);
```

## `uv_process_kill`

```c
/*
 * Kills the process with the specified signal. The user must still
 * call uv_close on the process.
 */
int uv_process_kill(uv_process_t*, int signum);
```

## `uv_kill`

```c
/* Kills the process with the specified signal. */
int uv_kill(int pid, int signum);
```

## `uv_setup_args`

```c
char** uv_setup_args(int argc, char** argv);
```

## `uv_get_process_title`

```c
int uv_get_process_title(char* buffer, size_t size);
```

## `uv_set_process_title`

```c
int uv_set_process_title(const char* title);
```

## `uv_resident_set_memory`

```c
int uv_resident_set_memory(size_t* rss);
```

## `uv_uptime`

```c
int uv_uptime(double* uptime);
```


# work queue

## `uv_queue_work`

```c
/* Queues a work request to execute asynchronously on the thread pool. */
int uv_queue_work(uv_loop_t* loop, uv_work_t* req, uv_work_cb work_cb, uv_after_work_cb after_work_cb);
```

## `uv_cancel`

```c
/* Cancel a pending request. Fails if the request is executing or has finished
 * executing.
 *
 * Returns 0 on success, or an error code < 0 on failure.
 *
 * Only cancellation of uv_fs_t, uv_getaddrinfo_t and uv_work_t requests is
 * currently supported.
 *
 * Cancelled requests have their callbacks invoked some time in the future.
 * It's _not_ safe to free the memory associated with the request until your
 * callback is called.
 *
 * Here is how cancellation is reported to your callback:
 *
 * - A uv_fs_t request has its req->result field set to UV_ECANCELED.
 *
 * - A uv_work_t or uv_getaddrinfo_t request has its callback invoked with
 *   status == UV_ECANCELED.
 *
 * This function is currently only implemented on UNIX platforms. On Windows,
 * it always returns UV_ENOSYS.
 */
int uv_cancel(uv_req_t* req);
```

# cpu info

## `uv_cpu_info`

```c
/*
 * This allocates cpu_infos array, and sets count.  The array
 * is freed using uv_free_cpu_info().
 */
int uv_cpu_info(uv_cpu_info_t** cpu_infos, int* count);
```

## `uv_free_cpu_info`

```c
void uv_free_cpu_info(uv_cpu_info_t* cpu_infos, int count);
```

# interface addresses

## `uv_interface_addresses`

```c
/*
 * This allocates addresses array, and sets count.  The array
 * is freed using uv_free_interface_addresses().
 */
int uv_interface_addresses(uv_interface_address_t** addresses, int* count);
```

## `uv_free_interface_addresses`

```c
void uv_free_interface_addresses(uv_interface_address_t* addresses, int count);
```

# file system


File System Methods.

The `uv_fs_*` functions execute a blocking system call asynchronously (in a
thread pool) and call the specified callback in the specified loop after
completion. If the user gives NULL as the callback the blocking system
call will be called synchronously. req should be a pointer to an
uninitialized `uv_fs_t` object.

`uv_fs_req_cleanup()` must be called after completion of the `uv_fs_` 
function to free any internal memory allocations associated with the
request.

## `uv_fs_req_cleanup`

nulls out `req->path`, `req->new_path` and `req->ptr` (unless it points to `req->statbuf`).

```c
void uv_fs_req_cleanup(uv_fs_t* req);
```

## `uv_fs_close`

```c
int uv_fs_close(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_fs_cb cb);
```

## `uv_fs_open`

```c
int uv_fs_open(uv_loop_t* loop,
               uv_fs_t* req,
               const char* path,
               int flags,
               int mode,
               uv_fs_cb cb);
```

### `uv_fs_t *req` passed to callback

`req->result` is error code or `uv_file` which is passed to `uv_fs_read`.

## `uv_fs_read`

```c
int uv_fs_read(uv_loop_t* loop,
               uv_fs_t* req,
               uv_file file,
               void* buf,
               size_t length,
               int64_t offset,
               uv_fs_cb cb);
```

### `uv_fs_t *req` passed to callback

`req->result` is error code or `ssize_t` number of bytes read.

## `uv_fs_unlink`

```c
int uv_fs_unlink(uv_loop_t* loop,
                 uv_fs_t* req,
                 const char* path,
                 uv_fs_cb cb);
```

## `uv_fs_write`

```c
int uv_fs_write(uv_loop_t* loop,
                uv_fs_t* req,
                uv_file file,
                const void* buf,
                size_t length,
                int64_t offset,
                uv_fs_cb cb);
```

### `uv_fs_t *req` passed to callback

`req->result` is error code or `ssize_t` number of bytes written.

## `uv_fs_mkdir`

```c
int uv_fs_mkdir(uv_loop_t* loop,
                uv_fs_t* req,
                const char* path,
                int mode,
                uv_fs_cb cb);
```

## `uv_fs_rmdir`

```c
int uv_fs_rmdir(uv_loop_t* loop,
                uv_fs_t* req,
                const char* path,
                uv_fs_cb cb);
```

## `uv_fs_readdir`

```c
int uv_fs_readdir(uv_loop_t* loop,
                  uv_fs_t* req,
                  const char* path,
                  int flags,
                  uv_fs_cb cb);
```

## `uv_fs_stat`

```c
int uv_fs_stat(uv_loop_t* loop,
               uv_fs_t* req,
               const char* path,
               uv_fs_cb cb);
```

## `uv_fs_fstat`

```c
int uv_fs_fstat(uv_loop_t* loop,
                uv_fs_t* req,
                uv_file file,
                uv_fs_cb cb);
```

## `uv_fs_rename`

```c
int uv_fs_rename(uv_loop_t* loop,
                 uv_fs_t* req,
                 const char* path,
                 const char* new_path,
                 uv_fs_cb cb);
```

## `uv_fs_fsync`

```c
int uv_fs_fsync(uv_loop_t* loop,
                uv_fs_t* req,
                uv_file file,
                uv_fs_cb cb);
```

## `uv_fs_fdatasync`

```c
int uv_fs_fdatasync(uv_loop_t* loop,
                    uv_fs_t* req,
                    uv_file file,
                    uv_fs_cb cb);
```

## `uv_fs_ftruncate`

```c
int uv_fs_ftruncate(uv_loop_t* loop,
                    uv_fs_t* req,
                    uv_file file,
                    int64_t offset,
                    uv_fs_cb cb);
```

## `uv_fs_sendfile`

```c
int uv_fs_sendfile(uv_loop_t* loop,
                   uv_fs_t* req,
                   uv_file out_fd,
                   uv_file in_fd,
                   int64_t in_offset,
                   size_t length,
                   uv_fs_cb cb);
```

## `uv_fs_chmod`

```c
int uv_fs_chmod(uv_loop_t* loop,
                uv_fs_t* req,
                const char* path,
                int mode,
                uv_fs_cb cb);
```

## `uv_fs_utime`

```c
int uv_fs_utime(uv_loop_t* loop,
                uv_fs_t* req,
                const char* path,
                double atime,
                double mtime,
                uv_fs_cb cb);
```

## `uv_fs_futime`

```c
int uv_fs_futime(uv_loop_t* loop,
                 uv_fs_t* req,
                 uv_file file,
                 double atime,
                 double mtime,
                 uv_fs_cb cb);
```

## `uv_fs_lstat`

```c
int uv_fs_lstat(uv_loop_t* loop,
                uv_fs_t* req,
                const char* path,
                uv_fs_cb cb);
```

## `uv_fs_link`

```c
int uv_fs_link(uv_loop_t* loop,
               uv_fs_t* req,
               const char* path,
               const char* new_path,
               uv_fs_cb cb);
```

## `uv_fs_symlink`

```c
/*
 * This flag can be used with uv_fs_symlink on Windows
 * to specify whether path argument points to a directory.
 */
#define UV_FS_SYMLINK_DIR          0x0001

/*
 * This flag can be used with uv_fs_symlink on Windows
 * to specify whether the symlink is to be created using junction points.
 */
#define UV_FS_SYMLINK_JUNCTION     0x0002
```

```c
int uv_fs_symlink(uv_loop_t* loop,
                  uv_fs_t* req,
                  const char* path,
                  const char* new_path,
                  int flags,
                  uv_fs_cb cb);
```

## `uv_fs_readlink`

```c
int uv_fs_readlink(uv_loop_t* loop,
                   uv_fs_t* req,
                   const char* path,
                   uv_fs_cb cb);
```

## `uv_fs_fchmod`

```c
int uv_fs_fchmod(uv_loop_t* loop,
                 uv_fs_t* req,
                 uv_file file,
                 int mode,
                 uv_fs_cb cb);
```

## `uv_fs_chown`

```c
int uv_fs_chown(uv_loop_t* loop,
                uv_fs_t* req,
                const char* path,
                uv_uid_t uid,
                uv_gid_t gid,
                uv_fs_cb cb);
```

## `uv_fs_fchown`

```c
int uv_fs_fchown(uv_loop_t* loop,
                 uv_fs_t* req,
                 uv_file file,
                 uv_uid_t uid,
                 uv_gid_t gid,
                 uv_fs_cb cb);
```


## `uv_guess_handle`

```c
/*
 * Used to detect what type of stream should be used with a given file
 * descriptor. Usually this will be used during initialization to guess the
 * type of the stdio streams.
 * For isatty() functionality use this function and test for UV_TTY.
 */
uv_handle_type uv_guess_handle(uv_file file);
```

## fs poll

`uv_fs_stat()` based polling file watcher.

### `uv_fs_poll_init`

```c
int uv_fs_poll_init(uv_loop_t* loop, uv_fs_poll_t* handle);
```

### `uv_fs_poll_start`

```c
/*
 * Check the file at `path` for changes every `interval` milliseconds.
 *
 * Your callback is invoked with `status < 0` if `path` does not exist
 * or is inaccessible. The watcher is *not* stopped but your callback is
 * not called again until something changes (e.g. when the file is created
 * or the error reason changes).
 *
 * When `status == 0`, your callback receives pointers to the old and new
 * `uv_stat_t` structs. They are valid for the duration of the callback
 * only!
 *
 * For maximum portability, use multi-second intervals. Sub-second intervals
 * will not detect all changes on many file systems.
 */
int uv_fs_poll_start(uv_fs_poll_t* handle,
                     uv_fs_poll_cb poll_cb,
                     const char* path,
                     unsigned int interval);
```

### `uv_fs_poll_stop`

```c
int uv_fs_poll_stop(uv_fs_poll_t* handle);
```

## fs event

### `uv_fs_event_init`

```c
int uv_fs_event_init(uv_loop_t* loop, uv_fs_event_t* handle);
```

### `uv_fs_event_start`

```c
int uv_fs_event_start(uv_fs_event_t* handle,
                      uv_fs_event_cb cb,
                      const char* filename,
                      unsigned int flags);
```

### `uv_fs_event_stop`

```c
int uv_fs_event_stop(uv_fs_event_t* handle);
```


# signal

## `uv_signal_init`

```c
int uv_signal_init(uv_loop_t* loop, uv_signal_t* handle);
```

## `uv_signal_start`

```c
int uv_signal_start(uv_signal_t* handle, uv_signal_cb signal_cb, int signum);
```

## `uv_signal_stop`

```c
int uv_signal_stop(uv_signal_t* handle);
```

# load average

## `uv_loadavg`

```c
/*
 * Gets load average.
 * See: http://en.wikipedia.org/wiki/Load_(computing)
 * Returns [0,0,0] on Windows.
 */
void uv_loadavg(double avg[3]);
```

# errors

Most functions return 0 on success or an error code < 0 on failure.

## `uv_strerror`

```c
const char* uv_strerror(int err);
```

## `uv_err_name`

```
const char* uv_err_name(int err);
```

# utilities

## ip address conversion

Convert string ip addresses to binary structures

### `uv_ip4_addr`

```c
int uv_ip4_addr(const char* ip, int port, struct sockaddr_in* addr);
```

### `uv_ip6_addr`

```c
int uv_ip6_addr(const char* ip, int port, struct sockaddr_in6* addr);
```

Convert binary addresses to strings

### `uv_ip4_name`

```c
int uv_ip4_name(struct sockaddr_in* src, char* dst, size_t size);
```

### `uv_ip6_name`

```c
int uv_ip6_name(struct sockaddr_in6* src, char* dst, size_t size);
```

## ntop and pton

Cross-platform IPv6-capable implementation of the 'standard' `inet_ntop`
and `inet_pton` functions. On success they return 0. If an error 
the target of the `dst` pointer is unmodified. 

### `uv_inet_ntop`

```c
int uv_inet_ntop(int af, const void* src, char* dst, size_t size);
```

### `uv_inet_pton`

```c
int uv_inet_pton(int af, const char* src, void* dst);
```

## path

### `uv_exepath`

Gets the executable path 

```c
int uv_exepath(char buffer, size_t size);
```

### `uv_cwd`

Gets the current working directory 

```c
int uv_cwd(char buffer, size_t size);
```

### `uv_chdir`

Changes the current working directory 

```c
int uv_chdir(const char* dir);
```

## memory

Gets memory info in bytes 

### `uv_get_free_memory`

```c
uint64_t uv_get_free_memory(void);
```

### `uv_get_total_memory`

```c
uint64_t uv_get_total_memory(void);
```

## time

### `uv_hrtime`

Returns the current high-resolution real time. This is expressed in
nanoseconds. It is relative to an arbitrary time in the past. It is not
related to the time of day and therefore not subject to clock drift. The
primary use is for measuring performance between intervals.

Note not every platform can support nanosecond resolution; however, this
value will always be in nanoseconds.
 
```c
extern uint64_t uv_hrtime(void);
```

## stdio

### `uv_disable_stdio_inheritance`

Disables inheritance for file descriptors  handles that this process
inherited from its parent. The effect is that child processes spawned by
this process don't accidentally inherit these handles.

It is recommended to call this function as early in your program as possible,
before the inherited file descriptors can be closed or duplicated.

Note that this function works on a best-effort basis: there is no guarantee
that libuv can discover all file descriptors that were inherited. In general
it does a better job on Windows than it does on unix.
 
```c
void uv_disable_stdio_inheritance(void);
```

## dynamic library access


### `uv_dlopen`

Opens a shared library. The filename is in utf-8. Returns 0 on success and
-1 on error. Call `uv_dlerror(uv_lib_t)` to get the error message.

```c 
int uv_dlopen(const char* filename, uv_lib_t* lib);
```

### `uv_dlclose`

Close the shared library.

```c
void uv_dlclose(uv_lib_t* lib);
```

### `uv_dlsym`

Retrieves a data pointer from a dynamic library. It is legal for a symbol to
map to NULL. Returns 0 on success and -1 if the symbol was not found.

```c
int uv_dlsym(uv_lib_t* lib, const char* name, void** ptr);
```

### `uv_dlerror`

Returns the last uv_dlopen() or uv_dlsym() error message.

```c
const char* uv_dlerror(uv_lib_t* lib);
```

## mutex

The mutex functions return 0 on success or an error code < 0
(unless the return type is void, of course).

### `uv_mutex_init`

```c
int uv_mutex_init(uv_mutex_t* handle);
```

### `uv_mutex_destroy`

```c
void uv_mutex_destroy(uv_mutex_t* handle);
```

### `uv_mutex_lock`

```c
void uv_mutex_lock(uv_mutex_t* handle);
```

### `uv_mutex_trylock`

```c
int uv_mutex_trylock(uv_mutex_t* handle);
```
### `uv_mutex_unlock`

```c
void uv_mutex_unlock(uv_mutex_t* handle);
```

## rwlock

The rwlock functions return 0 on success or an error code < 0
(unless the return type is void, of course).

### `uv_rwlock_init`

```c
int uv_rwlock_init(uv_rwlock_t* rwlock);
```

### `uv_rwlock_destroy`

```c
void uv_rwlock_destroy(uv_rwlock_t* rwlock);
```

### `uv_rwlock_rdlock`

```c
void uv_rwlock_rdlock(uv_rwlock_t* rwlock);
```

### `uv_rwlock_tryrdlock`

```c
int uv_rwlock_tryrdlock(uv_rwlock_t* rwlock);
```

### `uv_rwlock_rdunlock`

```c
void uv_rwlock_rdunlock(uv_rwlock_t* rwlock);
```

### `uv_rwlock_wrlock`

```c
void uv_rwlock_wrlock(uv_rwlock_t* rwlock);
```

### `uv_rwlock_trywrlock`

```c
int uv_rwlock_trywrlock(uv_rwlock_t* rwlock);
```

### `uv_rwlock_wrunlock`

```c
void uv_rwlock_wrunlock(uv_rwlock_t* rwlock);
```

## semaphores

The sempaphore functions return 0 on success or an error code < 0
(unless the return type is void, of course).

### `uv_sem_init`

```c
int uv_sem_init(uv_sem_t* sem, unsigned int value);
```

### `uv_sem_destroy`

```c
void uv_sem_destroy(uv_sem_t* sem);
```

### `uv_sem_post`

```c
void uv_sem_post(uv_sem_t* sem);
```

### `uv_sem_wait`

```c
void uv_sem_wait(uv_sem_t* sem);
```

### `uv_sem_trywait`

```c
int uv_sem_trywait(uv_sem_t* sem);
```


## condition variables

The condition variable functions return 0 on success or an error code < 0
(unless the return type is void, of course).

### `uv_cond_init`

```c
int uv_cond_init(uv_cond_t* cond);
```

### `uv_cond_destroy`

```c
void uv_cond_destroy(uv_cond_t* cond);
```

### `uv_cond_signal`

```c
void uv_cond_signal(uv_cond_t* cond);
```

### `uv_cond_broadcast`

```c
void uv_cond_broadcast(uv_cond_t* cond);
```

### `uv_cond_wait`

```c
/* Waits on a condition variable without a timeout.
 *
 * Note:
 * 1. callers should be prepared to deal with spurious wakeups.
 */
void uv_cond_wait(uv_cond_t* cond, uv_mutex_t* mutex);
```

### `uv_cond_timedwait`

```c
/* Waits on a condition variable with a timeout in nano seconds.
 * Returns 0 for success or UV_ETIMEDOUT on timeout, It aborts when other
 * errors happen.
 *
 * Note:
 * 1. callers should be prepared to deal with spurious wakeups.
 * 2. the granularity of timeout on Windows is never less than one millisecond.
 * 3. uv_cond_timedwait takes a relative timeout, not an absolute time.
 */
int uv_cond_timedwait(uv_cond_t* cond, uv_mutex_t* mutex, uint64_t timeout);
```

## barrier

### `uv_barrier_init`

```c
int uv_barrier_init(uv_barrier_t* barrier, unsigned int count);
```

### `uv_barrier_destroy`

```c
void uv_barrier_destroy(uv_barrier_t* barrier);
```

### `uv_barrier_wait`

```c
void uv_barrier_wait(uv_barrier_t* barrier);
```

## once

### `uv_once`

```c
/* Runs a function once and only once. Concurrent calls to uv_once() with the
 * same guard will block all callers except one (it's unspecified which one).
 * The guard should be initialized statically with the UV_ONCE_INIT macro.
 */
void uv_once(uv_once_t* guard, void (*callback)(void));
```

## thread local storage

Thread-local storage.  These functions largely follow the semantics of
`pthread_key_create()`, `pthread_key_delete()`, `pthread_getspecific()` and
`pthread_setspecific()`.

Note that the total thread-local storage size may be limited.
That is, it may not be possible to create many TLS keys.
 
### `uv_key_create`

```c
int uv_key_create(uv_key_t* key);
```

### `uv_key_delete`

```c
void uv_key_delete(uv_key_t* key);
```

### `uv_key_get`

```c
void* uv_key_get(uv_key_t* key);
```

### `uv_key_set`

```c
void uv_key_set(uv_key_t* key, void* value);
```

### `uv_thread_create`

```c
int uv_thread_create(uv_thread_t *tid, void (*entry)(void *arg), void *arg);
```

### `long `

```c
unsigned long uv_thread_self(void);
```

### `uv_thread_join`

```c
int uv_thread_join(uv_thread_t *tid);
```

