# send-file

This example shows how to send a file by piping its contents into the response. 

Although it includes setting up a server, binding to a tcp socket and sending headers, the major part implements the equivalent of:

```js
fs.createReadStream(filename).pipe(res);
```

## Starting Server

The main function does just that by first resolving the address for the given port and binding it to a tcp socket. Then
it starts listening on that socket for connections. 

Here is the code with lots of error handling removed for clarity:

```c
int main() {
  loop = uv_default_loop();

  // 1. resolve address
  struct sockaddr_in bind_addr;
  uv_ip4_addr("0.0.0.0", PORT, &bind_addr);

  // 2. init and bind to tcp socket
  uv_tcp_init(loop, &server);
  uv_tcp_bind(&server, (const struct sockaddr*) &bind_addr);

  // 3. listen
  uv_listen((uv_stream_t*) &server, BACKLOG, on_connect);

  uv_run(loop, UV_RUN_DEFAULT);
  return 0;
}
```

## Accepting Requests

The server now is ready to accept requests which is handled inside `on_connect`.

Once a request is accepted, we read in the request data. We don't really care what it is since in this simple example we always
send `main.c` no matter what the client actually requested.

## Getting file stats and initializing pipe

Once we read the request, we first obtain the size of the file we are about to send via a call to `uv_fs_stat`. The code
is fairly straight forward (`stat_file`, `on_file_stat`).

We then are ready to initialize the pipe and pass the obtained `path` and `size` information along. The size we don't
really need although we use it inside `on_write_res` to ensure that we read the entire file: 

```c
assert(file_tcp_pipe->size == file_tcp_pipe->nread);
```

We initialize a `uv_pipe_t` for the file descriptor we get by opening the file and open it. At this point we also create
a `file_tcp_pipe_t` in order to keep track of all the info we need, including pointers we need to resolve further down
and finally cleanup when responding to the request completes.


```c
file_tcp_pipe_t *file_tcp_pipe =  (file_tcp_pipe_t*) malloc(sizeof(file_tcp_pipe_t));
file_tcp_pipe->nread           =  0;
file_tcp_pipe->nwritten        =  0;
file_tcp_pipe->size            =  size;
file_tcp_pipe->tcp             =  tcp;
file_tcp_pipe->file_pipe       =  file_pipe;
```

In order to always be able to get to it, we attach it to both the file pipe and the tcp handle:

```c
tcp->data        =  (void*) file_tcp_pipe;
file_pipe->data  =  (void*) file_tcp_pipe;
```

## Writing headers and piping the file

Now we have everything setup and start by writing the headers, setting `"Transfer-Coding: chunked"` to indicate that
we'll send the data in chunks.

Note that we purposely are allocating smaller buffers (`alloc_cb`) in order to see how even a smaller file gets
piped into the response in chunks.

In order to pipe the file we call `uv_read_start` and now have `on_file_read`, `write_data` and `on_write_res` work
together until all the content is transferred at which point we close the tcp handle.

```c
if (file_tcp_pipe->nwritten == file_tcp_pipe->nread) {
  assert(file_tcp_pipe->size == file_tcp_pipe->nread);
  uv_close((uv_handle_t*) file_tcp_pipe->tcp, on_res_end);
}
```

That completes responding to the request.

## Tips

One important technique I started using after realizing that it is hard to keep track of your pointers across callbacks
is to create one data structure that provides access to everything you need. Then you just have to worry about getting
it across by attaching it to the `data` property of each request type.

```c
typedef struct {
  ssize_t nread;
  ssize_t nwritten;
  ssize_t size;
  char *file_path;
  uv_pipe_t *file_pipe;
  uv_stream_t *tcp;
} file_tcp_pipe_t;
```

This made life a lot easier than trying to keep everything around without an extra data structure, but by chaining
pointers, i.e. something like `tcp->data = X` and `write_req->data = tcp`. 
