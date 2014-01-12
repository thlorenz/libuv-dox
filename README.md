# libuv-dox

Documenting types and methods of [libuv](https://github.com/joyent/libuv/), mostly by reading
[`uv.h`](https://github.com/joyent/libuv/blob/f166d6d7055bbd9da83594b045a496e15b3c302a/include/uv.h).

## Organization

The information found in `uv.h` has been organized into three categories:

- [types](https://github.com/thlorenz/libuv-dox/blob/master/types.md)
- [callbacks](https://github.com/thlorenz/libuv-dox/blob/master/callbacks.md)
- [methods](https://github.com/thlorenz/libuv-dox/blob/master/methods.md)

[Examples](https://github.com/thlorenz/libuv-dox/tree/master/examples) were added to show how the API is used.


### types

The inheritance relationship is documented via `:`, i.e. [`uv_tcp_t :
uv_stream_t`](https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_tcp_t--uv_stream_t) denotes that `uv_tcp_t`
derives from `uv_stream_t` and thus has all the stream type fields.

Macros of extra/private fields have been resolved as much as was sensible and the original location and name is documented,
i.e. the private fields for [`uv_fs_t`](https://github.com/thlorenz/libuv-dox/blob/master/types.md#uv_fs_t) found inside
`include/uv-unix.h` are included.

**Note:** `uv-unix.h` was always preferred over `uv-win.h` when resolving fields from macros.

### callbacks

Although they are techniqually types as well they are documented in a separately in order to speed up lookup.

### methods

The methods are categorized and ordered to mirror the categorization and order of the types that they use.

## version

The libuv version at that is the source of this documentation is: `v0.11.17`.

## License

MIT

[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/thlorenz/libuv-dox/trend.png)](https://bitdeli.com/free "Bitdeli Badge")
