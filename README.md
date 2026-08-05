# dbgfmt

C++ debugger value/type formatting framework, with built-in formatters for
libc++ and libstdc++.

`dbgfmt` turns raw debuggee memory into human-readable representations of
values, driven by type information from the [`cm`](https://github.com/cxx-lang-tools/cm)
type-model library. It is used to implement "pretty printing" of variables,
expressions, and standard-library containers in a C++ debugger front end,
but has no dependency on any particular debugger and can be embedded in any
tool that already has `cm`-based type/value information for a debuggee.

## Concepts

- **`context`** — the formatting context. Owns the set of registered
  formatters and current format options, and is the entry point for
  formatting a value (`context::format`).
- **`value` / `value_type`** — thin, reference-counted wrappers around a
  backend-provided value/type. Formatters and calling code operate purely
  in terms of these, never the backend implementation directly.
- **`formatter`** — the extension point for custom formatting logic.
  Implementations decide whether they accept a given `value` and, if so,
  produce a `fmt_result`.
- **`fmt_result`** — the outcome of formatting a value: a display string
  plus an optional list of child values (for aggregate/container display),
  built incrementally via `fmt_result_childs_list`.
- **`debug_object`** — a named, addressable object in the debuggee (e.g. a
  variable or a field), used as an anchor for locating and formatting
  values.

## Backend interface

`dbgfmt` itself doesn't know how to read debuggee memory or resolve types —
it delegates that to a **backend**, an implementation of the interfaces
under `include/dbgfmt/backend/` (`type_context_impl`, `value_context_impl`,
`type_impl`, `value_impl`, and the per-type-kind variants such as
`record_type_impl`, `array_type_impl`, `pointer_type_impl`, etc.). A
concrete backend built on `cm` is provided in `src/backend/cm/` and used by
the bundled `stl` formatters and tests; other backends can be supplied by
implementing the same interfaces.

## Built-in formatters

`src/stl/` contains formatters for common standard-library types, split
between an ABI-agnostic layer and per-standard-library-implementation
layers:

- `stl/libcxx/` — libc++-specific type layouts (e.g. `tuple`, `is_empty`
  helpers).
- `stl/stdcpp/` — libstdc++-specific type layouts (e.g. `tuple`).
- ABI-agnostic formatters for `string`, `vector`, `list`, `forward_list`,
  `tuple`, `bitset`, `chrono`, `shared_ptr`, `bind` results, and more,
  built on top of these.

Formatters for scalar and aggregate kinds not specific to any library
(arrays, records, enums, pointers, member pointers, functors, lambdas,
etc.) live directly under `src/`.

## Layout

```
include/dbgfmt/     Public headers: context, value/type wrappers, formatter
                     base classes, and the backend interfaces
src/                 Implementation: concrete formatters, the cm-based
                     backend (src/backend/cm/), and STL-specific formatters
                     (src/stl/)
src/data/            Small helper library for endianness-aware access to
                     raw byte storage (used to interpret debuggee memory)
src/test/            Unit tests (Boost.Test) exercising the formatting
                     framework against synthetic cm type/value models
```

## Dependencies

- **Boost** (`log`, `unit_test_framework`, `program_options`), located via
  `find_package`.
- **[`cm`](https://github.com/cxx-lang-tools/cm)** — type-model library
  used by the bundled backend and STL formatters.
- **[`cxxtn`](https://github.com/cxxdbg/cxxtn)** — general-purpose C++
  utility library used throughout the implementation.

`cm` and `cxxtn` are each located via `DBGFMT_FETCH_CM`/`DBGFMT_FETCH_CXXTN`
(see below) rather than being vendored directly in this repository.

## Building

```sh
cmake -B build
cmake --build build
ctest --test-dir build
```

### Options

- **`DBGFMT_FETCH_CXXTN`** (default `ON`) — if `ON`, fetches `cxxtn` via
  `FetchContent`; if `OFF`, locates an existing `cxxtn` target via
  `find_package`.
- **`DBGFMT_FETCH_CM`** (default `ON`) — same semantics as
  `DBGFMT_FETCH_CXXTN`, for `cm`.

When embedding `dbgfmt` as a subdirectory of a parent project that already
provides `cm`/`cxxtn` targets of its own, set both options to `OFF` before
calling `add_subdirectory()` so `dbgfmt` reuses those targets instead of
fetching or searching for its own.

## License

Apache License 2.0 — see [LICENSE](LICENSE).
