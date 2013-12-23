# libuv-dox-examples

Some to demonstrate parts of the documented API.

## Get started

Clone this repo, and init submodules:

```sh
git clone https://github.com/thlorenz/libuv-dox
git submodule update --init
```

Then follow the steps described in [the libuv
Readme](https://github.com/joyent/libuv/blob/master/README.md#build-instructions) in order to build the libuv library.

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

## Oddities

All examples have the `uv.h` file linked. While that is not necessary to make these, it pleases my `clang-complete` vim
plugin to no end ;)
