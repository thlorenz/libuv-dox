**Table of Contents**  *generated with [DocToc](http://doctoc.herokuapp.com/)*

- [loop](#loop)
	- [`uv_loop_t`](#uv_loop_t)
- [requests](#requests)
	- [`uv_req_t`](#uv_req_t)
	- [`uv_getaddrinfo_t : uv_req_t`](#uv_getaddrinfo_t--uv_req_t)
	- [`uv_shutdown_t : uv_req_t`](#uv_shutdown_t--uv_req_t)
	- [`uv_write_t : uv_req_t`](#uv_write_t--uv_req_t)
	- [`uv_connect_t : uv_req_t`](#uv_connect_t--uv_req_t)
	- [`uv_udp_send_t : uv_req_t`](#uv_udp_send_t--uv_req_t)
	- [`uv_fs_t : uv_req_t`](#uv_fs_t--uv_req_t)
	- [`uv_work_t : uv_req_t`](#uv_work_t--uv_req_t)
	- [`uv_connect_t : uv_req_t`](#uv_connect_t--uv_req_t-1)
		- [`uv_fs_t`](#uv_fs_t)
- [buffers](#buffers)
	- [`uv_buf_t `](#uv_buf_t-)
- [handles](#handles)
	- [`uv_handle_t`](#uv_handle_t)
	- [streams](#streams)
		- [`uv_stream_t : uv_handle_t`](#uv_stream_t--uv_handle_t)
		- [`uv_tcp_t : uv_stream_t`](#uv_tcp_t--uv_stream_t)
		- [`uv_tty_t : uv_stream_t`](#uv_tty_t--uv_stream_t)
		- [`uv_pipe_t : uv_stream_t`](#uv_pipe_t--uv_stream_t)
	- [udp](#udp)
		- [`uv_udp_t : uv_handle_t`](#uv_udp_t--uv_handle_t)
	- [poll](#poll)
		- [`uv_poll_t : uv_handle_t`](#uv_poll_t--uv_handle_t)
	- [prepare](#prepare)
		- [`uv_prepare_t : uv_handle_t`](#uv_prepare_t--uv_handle_t)
	- [idle](#idle)
		- [`uv_idle_t : uv_handle_t`](#uv_idle_t--uv_handle_t)
	- [async](#async)
		- [`uv_async_t : uv_handle_t`](#uv_async_t--uv_handle_t)
	- [timer](#timer)
		- [`uv_timer_t : uv_handle_t`](#uv_timer_t--uv_handle_t)
	- [process](#process)
		- [`uv_process_t : uv_handle_t`](#uv_process_t--uv_handle_t)
	- [file system](#file-system)
		- [`uv_fs_event_t : uv_handle_t`](#uv_fs_event_t--uv_handle_t)
		- [`uv_fs_poll_t : uv_handle_t`](#uv_fs_poll_t--uv_handle_t)
- [signal](#signal)
		- [`uv_signal_t`](#uv_signal_t)
- [cpu info](#cpu-info)
	- [`uv_cpu_info_t`](#uv_cpu_info_t)
- [interface address](#interface-address)
	- [`uv_interface_address_t`](#uv_interface_address_t)
- [file info](#file-info)
	- [`uv_stat_t`](#uv_stat_t)
		- [`uv_timespec_t`](#uv_timespec_t)
- [stdio](#stdio)
	- [`uv_stdio_container_t`](#uv_stdio_container_t)
- [process](#process-1)
	- [`uv_process_options_t`](#uv_process_options_t)
- [enumerations](#enumerations)
	- [`uv_errno_t`](#uv_errno_t)
	- [`uv_handle_type`](#uv_handle_type)
	- [`uv_req_type`](#uv_req_type)
	- [`uv_udp_flags`](#uv_udp_flags)
	- [`uv_poll_event`](#uv_poll_event)
	- [`uv_stdio_flags`](#uv_stdio_flags)
	- [`uv_process_flags`](#uv_process_flags)
	- [`uv_fs_type`](#uv_fs_type)
	- [`uv_fs_event`](#uv_fs_event)
	- [`uv_fs_event_flags`](#uv_fs_event_flags)

# loop

## `uv_loop_t`

```c
/* Public fields */
struct uv_loop_s {
  /* User data - use this for whatever. */
  void* data;
  /* Loop reference counting */
  unsigned int active_handles;
  void* handle_queue[2];
  void* active_reqs[2];
  /* Internal flag to signal loop stop */
  unsigned int stop_flag;

  // UV_LOOP_PRIVATE_FIELDS (include/uv-unix.h)

  unsigned long flags;
  int backend_fd;
  void* pending_queue[2];
  void* watcher_queue[2];
  uv__io_t** watchers;
  unsigned int nwatchers;
  unsigned int nfds;
  void* wq[2];
  uv_mutex_t wq_mutex;
  uv_async_t wq_async;
  uv_handle_t* closing_handles;
  void* process_handles[1][2];
  void* prepare_handles[2];
  void* check_handles[2];
  void* idle_handles[2];
  void* async_handles[2];
  struct uv__async async_watcher;
  /* RB_HEAD(uv__timers, uv_timer_s) */
  struct uv__timers {
    struct uv_timer_s* rbh_root;
  } timer_handles;
  uint64_t time;
  int signal_pipefd[2];
  uv__io_t signal_io_watcher;
  uv_signal_t child_watcher;
  int emfile_fd;
  uint64_t timer_counter;

  // UV_PLATFORM_LOOP_FIELDS (include/uv-darwin.h
  int rcount;
  int wcount;
};
```
# requests

## `uv_req_t` 

Base for all request types

```c
/* Abstract base class of all requests. */
struct uv_req_s {

  // UV_REQ_FIELDS
  /* public */
  void* data;

  /* read-only */
  uv_req_type type;

  /* private */
  void* active_queue[2];

  // UV_REQ_PRIVATE_FIELDS (include/uv-unix.h)
  /* empty */
};
```

**Note:** all request types below subclass `uv_req_s` via the inclusion of `UV_REQ_FIELDS`.

## `uv_getaddrinfo_t : uv_req_t`

```c
struct uv_getaddrinfo_s {
  /* read-only */
  uv_loop_t* loop;

  // UV_GETADDRINFO_PRIVATE_FIELDS (include/uv-unix.h)
  struct uv__work work_req;
  uv_getaddrinfo_cb cb;
  struct addrinfo* hints;
  char* hostname;
  char* service;
  struct addrinfo* res;
  int retcode;
};
```

## `uv_shutdown_t : uv_req_t`

```c
struct uv_shutdown_s {
  uv_stream_t* handle;
  uv_shutdown_cb cb;
  
  // UV_SHUTDOWN_PRIVATE_FIELDS (include/uv-unix.h)
  /* empty */
};
```

## `uv_write_t : uv_req_t`

```c
struct uv_write_s {
  uv_write_cb cb;
  uv_stream_t* send_handle;
  uv_stream_t* handle;

  // UV_WRITE_PRIVATE_FIELDS (include/uv-unix.h)
  void* queue[2];
  unsigned int write_index;
  uv_buf_t* bufs;
  unsigned int nbufs;
  int error;
  uv_buf_t bufsml[4];
};
```

## `uv_connect_t : uv_req_t`

```c
struct uv_connect_s {
  uv_connect_cb cb;
  uv_stream_t* handle;

  // UV_CONNECT_PRIVATE_FIELDS (include/uv-unix.h)
  void* queue[2];
};
```

## `uv_udp_send_t : uv_req_t`

```c
struct uv_udp_send_s {
  uv_udp_t* handle;
  uv_udp_send_cb cb;

  // UV_UDP_SEND_PRIVATE_FIELDS (include/uv-unix.h)
  void* queue[2];
  struct sockaddr_in6 addr;
  unsigned int nbufs;
  uv_buf_t* bufs;
  ssize_t status;
  uv_udp_send_cb send_cb;
  uv_buf_t bufsml[4];
};
```

## `uv_fs_t : uv_req_t`

```c
struct uv_fs_s {
  UV_REQ_FIELDS
  uv_fs_type fs_type;
  uv_loop_t* loop;
  uv_fs_cb cb;
  ssize_t result;
  void* ptr;
  const char* path;
  uv_stat_t statbuf;  /* Stores the result of uv_fs_stat and uv_fs_fstat. */

  // UV_FS_PRIVATE_FIELDS (include/uv-unix.h)
  const char *new_path;
  uv_file file;
  int flags;
  mode_t mode;
  void* buf;
  size_t len;
  off_t off;
  uv_uid_t uid;
  uv_gid_t gid;
  double atime;
  double mtime;
  struct uv__work work_req;
};
```

## `uv_work_t : uv_req_t`

```c
struct uv_work_s {
  uv_loop_t* loop;
  uv_work_cb work_cb;
  uv_after_work_cb after_work_cb;

  // UV_WORK_PRIVATE_FIELDS (include/uv-unix.h)
  struct uv__work work_req;
};
```

## `uv_connect_t : uv_req_t`

```c
struct uv_connect_s {
  uv_connect_cb cb;
  uv_stream_t* handle;

  // UV_CONNECT_PRIVATE_FIELDS (include/uv-unix.h)
  void* queue[2];
};
```

### `uv_fs_t`

```c
struct uv_fs_s {
  uv_fs_type fs_type;
  uv_loop_t* loop;
  uv_fs_cb cb;
  ssize_t result;
  void* ptr;
  const char* path;
  uv_stat_t statbuf;  /* Stores the result of uv_fs_stat and uv_fs_fstat. */

  // UV_FS_PRIVATE_FIELDS (include/uv-unix.h)
  const char *new_path;
  uv_file file;
  int flags;
  mode_t mode;
  void* buf;
  size_t len;
  off_t off;
  uv_uid_t uid;
  uv_gid_t gid;
  double atime;
  double mtime;
  struct uv__work work_req;
};
```

# buffers


## `uv_buf_t `

```c
/*
 * Constructor for uv_buf_t.
 * Due to platform differences the user cannot rely on the ordering of the
 * base and len members of the uv_buf_t struct. The user is responsible for
 * freeing base after the uv_buf_t is done. Return struct passed by value.
 */
UV_EXTERN uv_buf_t uv_buf_init(char* base, unsigned int len);
```

# handles

## `uv_handle_t`

```c
/* The abstract base class of all handles.  */
struct uv_handle_s {

  // UV_HANDLE_FIELDS (include/uv.h)
  /* public */
  uv_close_cb close_cb;
  void* data;
  /* read-only */
  uv_loop_t* loop;
  uv_handle_type type;
  /* private */
  void* handle_queue[2];

  // UV_HANDLE_PRIVATE_FIELDS (include/uv-unix.h)
  int flags;
  uv_handle_t* next_closing;
};
```

**Note:** all handle types (including stream types) subclass `uv_handle_t`.

## streams

### `uv_stream_t : uv_handle_t`

```c
struct uv_stream_s {

  // UV_STREAM_FIELDS (include/uv.h)
  /* number of bytes queued for writing */
  size_t write_queue_size;
  uv_alloc_cb alloc_cb;
  uv_read_cb read_cb;
  uv_read2_cb read2_cb;
  /* private */

  // UV_STREAM_PRIVATE_FIELDS (include/uv-unix.h)
  uv_connect_t *connect_req;
  uv_shutdown_t *shutdown_req;
  uv__io_t io_watcher;
  void* write_queue[2];
  void* write_completed_queue[2];
  uv_connection_cb connection_cb;
  int delayed_error;
  int accepted_fd;

  // UV_STREAM_PRIVATE_PLATFORM_FIELDS (include/uv-unix.h)
  /* empty */
};
```

**Note:** all stream types subclass `uv_stream_t` and thus `uv_handle_t`.

### `uv_tcp_t : uv_stream_t`

Represents a TCP stream or TCP server.

```c
struct uv_tcp_s {
  // UV_TCP_PRIVATE_FIELDS (include/uv-unix.h)
  /* empty */
};
```

### `uv_tty_t : uv_stream_t`

Representing a stream for the console.

```c
struct uv_tty_s {
  // UV_TTY_PRIVATE_FIELDS (include/uv-unix.h)
  struct termios orig_termios;
  int mode;
};
```

### `uv_pipe_t : uv_stream_t`

```c
/*
 * Representing a pipe stream or pipe server. On Windows this is a Named
 * Pipe. On Unix this is a UNIX domain socket.
 */
struct uv_pipe_s {
  int ipc; /* non-zero if this pipe is used for passing handles */

  // UV_PIPE_PRIVATE_FIELDS (include/uv-unix.h)
  const char* pipe_fname; /* strdup'ed */
};
```

## udp

### `uv_udp_t : uv_handle_t`

```c
struct uv_udp_s {
  // UV_UDP_PRIVATE_FIELDS (include/uv-unix.h)
  uv_alloc_cb alloc_cb;
  uv_udp_recv_cb recv_cb;
  uv__io_t io_watcher;
  void* write_queue[2];
  void* write_completed_queue[2];
};
```

## poll

### `uv_poll_t : uv_handle_t`

```c
/*
 * The uv_poll watcher is used to watch file descriptors for readability and
 * writability, similar to the purpose of poll(2).
 *
 * The purpose of uv_poll is to enable integrating external libraries that
 * rely on the event loop to signal it about the socket status changes, like
 * c-ares or libssh2. Using uv_poll_t for any other other purpose is not
 * recommended; uv_tcp_t, uv_udp_t, etc. provide an implementation that is
 * much faster and more scalable than what can be achieved with uv_poll_t,
 * especially on Windows.
 *
 * It is possible that uv_poll occasionally signals that a file descriptor is
 * readable or writable even when it isn't. The user should therefore always
 * be prepared to handle EAGAIN or equivalent when it attempts to read from or
 * write to the fd.
 *
 * It is not okay to have multiple active uv_poll watchers for the same socket.
 * This can cause libuv to busyloop or otherwise malfunction.
 *
 * The user should not close a file descriptor while it is being polled by an
 * active uv_poll watcher. This can cause the poll watcher to report an error,
 * but it might also start polling another socket. However the fd can be safely
 * closed immediately after a call to uv_poll_stop() or uv_close().
 *
 * On windows only sockets can be polled with uv_poll. On unix any file
 * descriptor that would be accepted by poll(2) can be used with uv_poll.
 */
struct uv_poll_s {
  uv_poll_cb poll_cb;
  // UV_POLL_PRIVATE_FIELDS (include/uv-unix.h)
  uv__io_t io_watcher;
};
```

## prepare

### `uv_prepare_t : uv_handle_t`

```c
/*
 * Every active prepare handle gets its callback called exactly once per loop
 * iteration, just before the system blocks to wait for completed i/o.
 */
struct uv_prepare_s {
  // UV_PREPARE_PRIVATE_FIELDS (include/uv-unix.h)
  uv_prepare_cb prepare_cb;
  void* queue[2];
};
```

## idle

### `uv_idle_t : uv_handle_t`

```c
/*
 * Every active idle handle gets its callback called repeatedly until it is
 * stopped. This happens after all other types of callbacks are processed.
 * When there are multiple "idle" handles active, their callbacks are called
 * in turn.
 */
struct uv_idle_s {
  // UV_IDLE_PRIVATE_FIELDS (include/uv-unix.h)
  uv_idle_cb idle_cb;
  void* queue[2];
};
```

## async

### `uv_async_t : uv_handle_t`

```c
/*
 * uv_async_send wakes up the event loop and calls the async handle's callback.
 * There is no guarantee that every uv_async_send call leads to exactly one
 * invocation of the callback; the only guarantee is that the callback function
 * is called at least once after the call to async_send. Unlike all other
 * libuv functions, uv_async_send can be called from another thread.
 */
struct uv_async_s {
  // UV_ASYNC_PRIVATE_FIELDS (include/uv-unix.h)
  uv_async_cb async_cb;
  void* queue[2];
  int pending;
};
```

## timer

### `uv_timer_t : uv_handle_t`

```c
/*
 * Used to get woken up at a specified time in the future.
 */
struct uv_timer_s {
  // UV_TIMER_PRIVATE_FIELDS (include/uv-unix.h)
  /* RB_ENTRY(uv_timer_s) tree_entry; */
  struct {
    struct uv_timer_s* rbe_left;
    struct uv_timer_s* rbe_right;
    struct uv_timer_s* rbe_parent;
    int rbe_color;
  } tree_entry;
  uv_timer_cb timer_cb;
  uint64_t timeout;
  uint64_t repeat;
  uint64_t start_id;
};
```

## process

### `uv_process_t : uv_handle_t`

```c
struct uv_process_s {
  uv_exit_cb exit_cb;
  int pid;

  // UV_PROCESS_PRIVATE_FIELDS (include/uv-unix.h)
  void* queue[2];
  int status;
};
```

## file system

### `uv_fs_event_t : uv_handle_t`

```c
struct uv_fs_event_s {
  char* filename;

  // UV_FS_EVENT_PRIVATE_FIELDS (include/uv-unix.h)
  uv_fs_event_cb cb;
  UV_PLATFORM_FS_EVENT_FIELDS
};
```

### `uv_fs_poll_t : uv_handle_t`

```c
struct uv_fs_poll_s {
  /* Private, don't touch. */
  void* poll_ctx;
};
```

# signal

### `uv_signal_t`

```c
/*
 * UNIX signal handling on a per-event loop basis. The implementation is not
 * ultra efficient so don't go creating a million event loops with a million
 * signal watchers.
 *
 * Note to Linux users: SIGRT0 and SIGRT1 (signals 32 and 33) are used by the
 * NPTL pthreads library to manage threads. Installing watchers for those
 * signals will lead to unpredictable behavior and is strongly discouraged.
 * Future versions of libuv may simply reject them.
 *
 * Some signal support is available on Windows:
 *
 *   SIGINT is normally delivered when the user presses CTRL+C. However, like
 *   on Unix, it is not generated when terminal raw mode is enabled.
 *
 *   SIGBREAK is delivered when the user pressed CTRL+BREAK.
 *
 *   SIGHUP is generated when the user closes the console window. On SIGHUP the
 *   program is given approximately 10 seconds to perform cleanup. After that
 *   Windows will unconditionally terminate it.
 *
 *   SIGWINCH is raised whenever libuv detects that the console has been
 *   resized. SIGWINCH is emulated by libuv when the program uses an uv_tty_t
 *   handle to write to the console. SIGWINCH may not always be delivered in a
 *   timely manner; libuv will only detect size changes when the cursor is
 *   being moved. When a readable uv_tty_handle is used in raw mode, resizing
 *   the console buffer will also trigger a SIGWINCH signal.
 *
 * Watchers for other signals can be successfully created, but these signals
 * are never generated. These signals are: SIGILL, SIGABRT, SIGFPE, SIGSEGV,
 * SIGTERM and SIGKILL.
 *
 * Note that calls to raise() or abort() to programmatically raise a signal are
 * not detected by libuv; these will not trigger a signal watcher.
 */
struct uv_signal_s {
  uv_signal_cb signal_cb;
  int signum;

  // UV_SIGNAL_PRIVATE_FIELDS (include/uv-unix.h)
  /* RB_ENTRY(uv_signal_s) tree_entry; */                                     \
  struct {                                                                    \
    struct uv_signal_s* rbe_left;                                             \
    struct uv_signal_s* rbe_right;                                            \
    struct uv_signal_s* rbe_parent;                                           \
    int rbe_color;                                                            \
  } tree_entry;                                                               \
  /* Use two counters here so we don have to fiddle with atomics. */          \
  unsigned int caught_signals;                                                \
  unsigned int dispatched_signals;
};
```

# cpu info

## `uv_cpu_info_t`

```c
struct uv_cpu_info_s {
  char* model;
  int speed;
  struct uv_cpu_times_s {
    uint64_t user;
    uint64_t nice;
    uint64_t sys;
    uint64_t idle;
    uint64_t irq;
  } cpu_times;
};
```

# interface address

## `uv_interface_address_t`

```c
struct uv_interface_address_s {
  char* name;
  char phys_addr[6];
  int is_internal;
  union {
    struct sockaddr_in address4;
    struct sockaddr_in6 address6;
  } address;
  union {
    struct sockaddr_in netmask4;
    struct sockaddr_in6 netmask6;
  } netmask;
};
```

# file info

## `uv_stat_t`

```c
typedef struct {
  uint64_t st_dev;
  uint64_t st_mode;
  uint64_t st_nlink;
  uint64_t st_uid;
  uint64_t st_gid;
  uint64_t st_rdev;
  uint64_t st_ino;
  uint64_t st_size;
  uint64_t st_blksize;
  uint64_t st_blocks;
  uint64_t st_flags;
  uint64_t st_gen;
  uv_timespec_t st_atim;
  uv_timespec_t st_mtim;
  uv_timespec_t st_ctim;
  uv_timespec_t st_birthtim;
};
```

### `uv_timespec_t`

```c
typedef struct {
  long tv_sec;
  long tv_nsec;
};
```

# stdio

## `uv_stdio_container_t`

```c
typedef struct uv_stdio_container_s {
  uv_stdio_flags flags;

  union {
    uv_stream_t* stream;
    int fd;
  } data;
} uv_stdio_container_t;
```

# process

## `uv_process_options_t`

```c
typedef struct uv_process_options_s {
  uv_exit_cb exit_cb; /* Called after the process exits. */
  const char* file; /* Path to program to execute. */
  /*
   * Command line arguments. args[0] should be the path to the program. On
   * Windows this uses CreateProcess which concatenates the arguments into a
   * string this can cause some strange errors. See the note at
   * windows_verbatim_arguments.
   */
  char** args;
  /*
   * This will be set as the environ variable in the subprocess. If this is
   * NULL then the parents environ will be used.
   */
  char** env;
  /*
   * If non-null this represents a directory the subprocess should execute
   * in. Stands for current working directory.
   */
  const char* cwd;
  /*
   * Various flags that control how uv_spawn() behaves. See the definition of
   * `enum uv_process_flags` below.
   */
  unsigned int flags;
  /*
   * The `stdio` field points to an array of uv_stdio_container_t structs that
   * describe the file descriptors that will be made available to the child
   * process. The convention is that stdio[0] points to stdin, fd 1 is used for
   * stdout, and fd 2 is stderr.
   *
   * Note that on windows file descriptors greater than 2 are available to the
   * child process only if the child processes uses the MSVCRT runtime.
   */
  int stdio_count;
  uv_stdio_container_t* stdio;
  /*
   * Libuv can change the child process' user/group id. This happens only when
   * the appropriate bits are set in the flags fields. This is not supported on
   * windows; uv_spawn() will fail and set the error to UV_ENOTSUP.
   */
  uv_uid_t uid;
  uv_gid_t gid;
} uv_process_options_t;
```

# enumerations

## `uv_errno_t`

```c
typedef enum {

  // UV_ERRNO_MAP(XX) (include/uv-unix.h)
  XX(E2BIG, "argument list too long")
  XX(EACCES, "permission denied")
  XX(EADDRINUSE, "address already in use")
  XX(EADDRNOTAVAIL, "address not available")
  XX(EAFNOSUPPORT, "address family not supported")
  XX(EAGAIN, "resource temporarily unavailable")
  XX(EAI_ADDRFAMILY, "address family not supported")
  XX(EAI_AGAIN, "temporary failure")
  XX(EAI_BADFLAGS, "bad ai_flags value")
  XX(EAI_BADHINTS, "invalid value for hints")
  XX(EAI_CANCELED, "request canceled")
  XX(EAI_FAIL, "permanent failure")
  XX(EAI_FAMILY, "ai_family not supported")
  XX(EAI_MEMORY, "out of memory")
  XX(EAI_NODATA, "no address")
  XX(EAI_NONAME, "unknown node or service")
  XX(EAI_OVERFLOW, "argument buffer overflow")
  XX(EAI_PROTOCOL, "resolved protocol is unknown")
  XX(EAI_SERVICE, "service not available for socket type")
  XX(EAI_SOCKTYPE, "socket type not supported")
  XX(EAI_SYSTEM, "system error")
  XX(EALREADY, "connection already in progress")
  XX(EBADF, "bad file descriptor")
  XX(EBUSY, "resource busy or locked")
  XX(ECANCELED, "operation canceled")
  XX(ECHARSET, "invalid Unicode character")
  XX(ECONNABORTED, "software caused connection abort")
  XX(ECONNREFUSED, "connection refused")
  XX(ECONNRESET, "connection reset by peer")
  XX(EDESTADDRREQ, "destination address required")
  XX(EEXIST, "file already exists")
  XX(EFAULT, "bad address in system call argument")
  XX(EHOSTUNREACH, "host is unreachable")
  XX(EINTR, "interrupted system call")
  XX(EINVAL, "invalid argument")
  XX(EIO, "i/o error")
  XX(EISCONN, "socket is already connected")
  XX(EISDIR, "illegal operation on a directory")
  XX(ELOOP, "too many symbolic links encountered")
  XX(EMFILE, "too many open files")
  XX(EMSGSIZE, "message too long")
  XX(ENAMETOOLONG, "name too long")
  XX(ENETDOWN, "network is down")
  XX(ENETUNREACH, "network is unreachable")
  XX(ENFILE, "file table overflow")
  XX(ENOBUFS, "no buffer space available")
  XX(ENODEV, "no such device")
  XX(ENOENT, "no such file or directory")
  XX(ENOMEM, "not enough memory")
  XX(ENONET, "machine is not on the network")
  XX(ENOSPC, "no space left on device")
  XX(ENOSYS, "function not implemented")
  XX(ENOTCONN, "socket is not connected")
  XX(ENOTDIR, "not a directory")
  XX(ENOTEMPTY, "directory not empty")
  XX(ENOTSOCK, "socket operation on non-socket")
  XX(ENOTSUP, "operation not supported on socket")
  XX(EPERM, "operation not permitted")
  XX(EPIPE, "broken pipe")
  XX(EPROTO, "protocol error")
  XX(EPROTONOSUPPORT, "protocol not supported")
  XX(EPROTOTYPE, "protocol wrong type for socket")
  XX(EROFS, "read-only file system")
  XX(ESHUTDOWN, "cannot send after transport endpoint shutdown")
  XX(ESPIPE, "invalid seek")
  XX(ESRCH, "no such process")
  XX(ETIMEDOUT, "connection timed out")
  XX(EXDEV, "cross-device link not permitted")
  XX(UNKNOWN, "unknown error")
  XX(EOF, "end of file")

  UV_ERRNO_MAX = UV__EOF - 1
} uv_errno_t;
```

## `uv_handle_type`

```c
typedef enum {
  UV_UNKNOWN_HANDLE = 0,

  // UV_HANDLE_TYPE_MAP(XX) (include/uv.h)
  XX(ASYNC, async)
  XX(CHECK, check)
  XX(FS_EVENT, fs_event)
  XX(FS_POLL, fs_poll)
  XX(HANDLE, handle)
  XX(IDLE, idle)
  XX(NAMED_PIPE, pipe)
  XX(POLL, poll)
  XX(PREPARE, prepare)
  XX(PROCESS, process)
  XX(STREAM, stream)
  XX(TCP, tcp)
  XX(TIMER, timer)
  XX(TTY, tty)
  XX(UDP, udp)
  XX(SIGNAL, signal)
  UV_FILE,

  UV_HANDLE_TYPE_MAX
} uv_handle_type;
```

## `uv_req_type`

```c
typedef enum {
  UV_UNKNOWN_REQ = 0,

  // UV_REQ_TYPE_MAP(XX) (include/uv.h)
  XX(REQ, req)
  XX(CONNECT, connect)
  XX(WRITE, write)
  XX(SHUTDOWN, shutdown)
  XX(UDP_SEND, udp_send)
  XX(FS, fs)
  XX(WORK, work)
  XX(GETADDRINFO, getaddrinfo)

  UV_REQ_TYPE_PRIVATE
  UV_REQ_TYPE_MAX
} uv_req_type;
```

## `uv_udp_flags`

```c
enum uv_udp_flags {
  /* Disables dual stack mode. */
  UV_UDP_IPV6ONLY = 1,
  /*
   * Indicates message was truncated because read buffer was too small. The
   * remainder was discarded by the OS. Used in uv_udp_recv_cb.
   */
  UV_UDP_PARTIAL = 2
};
```

## `uv_poll_event`

```c
enum uv_poll_event {
  UV_READABLE = 1,
  UV_WRITABLE = 2
};
```

## `uv_stdio_flags`

```c
typedef enum {
  UV_IGNORE         = 0x00,
  UV_CREATE_PIPE    = 0x01,
  UV_INHERIT_FD     = 0x02,
  UV_INHERIT_STREAM = 0x04,

  /* When UV_CREATE_PIPE is specified, UV_READABLE_PIPE and UV_WRITABLE_PIPE
   * determine the direction of flow, from the child process' perspective. Both
   * flags may be specified to create a duplex data stream.
   */
  UV_READABLE_PIPE  = 0x10,
  UV_WRITABLE_PIPE  = 0x20
} uv_stdio_flags;
```

## `uv_process_flags`

These are the flags that can be used for the uv_process_options.flags field.

```c
enum uv_process_flags {
  /*
   * Set the child process' user id. The user id is supplied in the `uid` field
   * of the options struct. This does not work on windows; setting this flag
   * will cause uv_spawn() to fail.
   */
  UV_PROCESS_SETUID = (1 << 0),
  /*
   * Set the child process' group id. The user id is supplied in the `gid`
   * field of the options struct. This does not work on windows; setting this
   * flag will cause uv_spawn() to fail.
   */
  UV_PROCESS_SETGID = (1 << 1),
  /*
   * Do not wrap any arguments in quotes, or perform any other escaping, when
   * converting the argument list into a command line string. This option is
   * only meaningful on Windows systems. On unix it is silently ignored.
   */
  UV_PROCESS_WINDOWS_VERBATIM_ARGUMENTS = (1 << 2),
  /*
   * Spawn the child process in a detached state - this will make it a process
   * group leader, and will effectively enable the child to keep running after
   * the parent exits.  Note that the child process will still keep the
   * parent's event loop alive unless the parent process calls uv_unref() on
   * the child's process handle.
   */
  UV_PROCESS_DETACHED = (1 << 3),
  /*
   * Hide the subprocess console window that would normally be created. This
   * option is only meaningful on Windows systems. On unix it is silently
   * ignored.
   */
  UV_PROCESS_WINDOWS_HIDE = (1 << 4)
};
```

## `uv_fs_type`

```c
typedef enum {
  UV_FS_UNKNOWN = -1,
  UV_FS_CUSTOM,
  UV_FS_OPEN,
  UV_FS_CLOSE,
  UV_FS_READ,
  UV_FS_WRITE,
  UV_FS_SENDFILE,
  UV_FS_STAT,
  UV_FS_LSTAT,
  UV_FS_FSTAT,
  UV_FS_FTRUNCATE,
  UV_FS_UTIME,
  UV_FS_FUTIME,
  UV_FS_CHMOD,
  UV_FS_FCHMOD,
  UV_FS_FSYNC,
  UV_FS_FDATASYNC,
  UV_FS_UNLINK,
  UV_FS_RMDIR,
  UV_FS_MKDIR,
  UV_FS_RENAME,
  UV_FS_READDIR,
  UV_FS_LINK,
  UV_FS_SYMLINK,
  UV_FS_READLINK,
  UV_FS_CHOWN,
  UV_FS_FCHOWN
} uv_fs_type;
```

## `uv_fs_event`

```c
enum uv_fs_event {
  UV_RENAME = 1,
  UV_CHANGE = 2
};
```

## `uv_fs_event_flags`

Flags to be passed to `uv_fs_event_start`.

```c
enum uv_fs_event_flags {
  /*
   * By default, if the fs event watcher is given a directory name, we will
   * watch for all events in that directory. This flags overrides this behavior
   * and makes fs_event report only changes to the directory entry itself. This
   * flag does not affect individual files watched.
   * This flag is currently not implemented yet on any backend.
   */
  UV_FS_EVENT_WATCH_ENTRY = 1,

  /*
   * By default uv_fs_event will try to use a kernel interface such as inotify
   * or kqueue to detect events. This may not work on remote filesystems such
   * as NFS mounts. This flag makes fs_event fall back to calling stat() on a
   * regular interval.
   * This flag is currently not implemented yet on any backend.
   */
  UV_FS_EVENT_STAT = 2,

  /*
   * By default, event watcher, when watching directory, is not registering
   * (is ignoring) changes in it's subdirectories.
   * This flag will override this behaviour on platforms that support it.
   */
  UV_FS_EVENT_RECURSIVE = 4
};
```
