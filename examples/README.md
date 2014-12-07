# libuv-dox-examples

Some examples to demonstrate parts of the documented libuv API.

## Get started

Clone this repo, and init submodules:

```sh
git clone https://github.com/thlorenz/libuv-dox
git submodule update --init
```

Then follow the steps described in [the libuv
Readme](https://github.com/libuv/libuv/blob/master/README.md#build-instructions) in order to build the libuv library.

The instructions that worked for me (on a Mac) are as follows:

```sh
cd libuv
mkdir -p build
git clone https://git.chromium.org/external/gyp.git build/gyp

./gyp_uv.py -f make
make -C out
```

For OS X other steps are recommended, but the above worked just fine (I'm not planning to work with libuv via an XCode
project any time soon).

## Building the sample projects

Each sample has a `Makefile` that should work for you granted you followed the steps above, i.e.:

```sh
cd helloworld
make
./main
```

Or even shorter `make run` will recompile and run the example as well.

## libuv book

The examples starting with a number i.e. `02-uvcat` have been adapted from the
[uvbook](http://nikhilm.github.io/uvbook/).

- adapted to match current libuv API
- `assert` statements where inserted to document expected values, i.e. `req->fs_type`
- some error handling was introduced
- log statements on `stderr` where added

## others

- [send-file](https://github.com/thlorenz/libuv-dox/tree/master/examples/send-file) shows how to send a file by piping
  its contents into the response
- very simple and incomplete
  [static-web-server](https://github.com/thlorenz/libuv-dox/tree/master/examples/static-web-server)

## Isolating logs vs. output

For examples that have lots of output do the following in order to show logs only:

    ./main ... 1>/dev/null

This is similar to running `make runerr`.

To omit logs do:


    ./main 2>/dev/null 

This is similar to running `make runout`.
