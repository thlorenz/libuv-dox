# static web server

![screenshot](https://raw.github.com/thlorenz/libuv-dox/master/examples/static-web-server/assets/screen-shot.png)

This simple example shows how to build a simple static web server with libuv.

It is by no means stable, complete or correct. It's sole purpose is to illustrate how to use the different methods
exposed by libuv to archieve this.

Fixing most of these problems are out of the scope for a simple example like this (just think MIME types).

## serving a request

After starting the server much in the way [explained
here](https://github.com/thlorenz/libuv-dox/tree/master/examples/send-file#starting-server), we handle each request as
follows:

- parse request via [our request parser](https://github.com/thlorenz/libuv-dox/blob/master/examples/static-web-server/request-parser.c) which is a small wrapper arount
  [http-parser](https://github.com/joyent/http-parser)
- after determining what resource we are supposed to serve we try to [resolve the appropriate file including it's
  size](https://github.com/thlorenz/libuv-dox/blob/master/examples/static-web-server/resolve-resource.c)
- finally we open the file and [pipe it into the
  response](https://github.com/thlorenz/libuv-dox/blob/master/examples/static-web-server/pipe-file.c), the details of
  which are explained inside the [send-file example](https://github.com/thlorenz/libuv-dox/tree/master/examples/send-file)


## known problems/bugs

- at this point no headers are sent, leaving the client to figure out what we are sending
- files are opened but never closed - this will be fixed soon
- probably lots more that I have overlooked

If you find bugs, please submit a pull request to fix this. However as this is an example additions to make this more
complete are not desired as this is quite large enough for an example already.

I am planning on spinning another repo out from this and make improvements there while keeping the example small.
