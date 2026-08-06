# dbgfmt

C++ debugger value/type formatting framework, with built-in formatters for
libc++ and libstdc++.

`dbgfmt` formats raw debuggee memory as human-readable value representations,
using type information supplied by a pluggable backend (see below). It
implements pretty-printing of variables and expressions for a C++ debugger
front end, including most standard library types: containers, functional
objects, utility types, and `chrono` types.

All formatters are covered by unit tests, which use a dedicated testing backend
built on top of the [`cm`](https://github.com/cxx-lang-tools/cm) code model
library.

## Concepts

- **`context`** — the formatting context. Owns the set of registered
  formatters and current format options, and is the entry point for
  formatting a value (`context::format`).
- **`value` / `value_type`** — thin wrappers, using intrusive reference
  counting for ownership, around a value/type from the backend. Formatters
  and calling code use these wrappers directly and never touch the backend
  implementation.
- **`formatter`** — the extension point for custom formatting logic.
  Implementations decide whether they accept a given `value` and, if so,
  produce a `fmt_result`.
- **`fmt_result`** — the result of formatting a value. Holds the display
  string for the value, the display string for its type, the source
  position where the value is defined, the source position where the type
  is defined, and an optional list of child values for complex types.

## Backend interface

`dbgfmt` itself doesn't read debuggee memory or resolve types. It delegates
that to a **backend**, an implementation of the interfaces declared in
`include/dbgfmt/backend/`:

- **Context types**: `type_context_impl` and `value_context_impl`, the
  entry points into the type model and the value model.
- **Type/value types**: `type_impl` and `value_impl`, the base interfaces,
  plus one `*_type_impl` interface for each type kind (`array_type_impl`,
  `builtin_type_impl`, `enum_type_impl`, `function_type_impl`,
  `mem_ptr_type_impl`, `pointer_type_impl`, `record_type_impl`,
  `typedef_type_impl`, `vector_type_impl`).
- **Symbol/object types**: `symbol_impl` and `debug_object_impl`.
- **Data access**: `data::storage`, declared in
  `include/dbgfmt/data/storage.hpp`, implemented by the backend to read
  and write raw bytes in the debuggee (memory and registers).
- **Other types**: `source_position` and `format_error`.

`src/backend/cm/` implements these interfaces on top of the `cm` code model
library. This backend is used only by the test suite. Other backends can
be supplied by implementing the same interfaces.

## Built-in formatters

The following types are supported for both libc++ and libstdc++:

- **Containers**: `array`, `vector`, `deque`, `list`, `forward_list`,
  `stack`, `queue`, unordered (hash table) containers, ordered (tree-based)
  containers, `bitset`, `valarray`.
- **Strings**: `string`, `string_view`.
- **Smart pointers**: `shared_ptr`, `unique_ptr`, `auto_ptr`.
- **Functional types**: `function`, `bind` results, argument placeholders,
  `reference_wrapper`.
- **Utility types**: `tuple`, `optional`, `any`, `variant`, `complex`.
- **`chrono`**: durations and time points.
- **`regex`**.

## Layout

- **`include/dbgfmt/`**: public headers: context, value/type wrappers,
  formatter base classes, the backend interfaces, and the data access
  types.
- **`src/`**: library implementation.
- **`src/stl/`**: implementation of the STL formatters.
- **`src/backend/cm/`**: the backend built on `cm`, used only by the test
  suite.
- **`src/data/`**: implementation of the data access types (`data::accessor`
  and the error handling used by them).
- **`src/test/`**: unit tests (Boost.Test) for the formatters, using the
  backend built on `cm` library.

## Dependencies

- **Boost** (`log`, `unit_test_framework`, `program_options`)
- **[`cm`](https://github.com/cxx-lang-tools/cm)** — code model library
  used by the `cm` backend (`src/backend/cm/`), needed only for testing.
- **[`cxxtn`](https://github.com/cxxdbg/cxxtn)** — helper library for
  parsing C++ type and function names.

## Building

```sh
mkdir build
cd build
cmake <src_dir> <cmake_options>
cmake --build .
ctest
```

### CMake Options

- **`DBGFMT_FETCH_CXXTN`** (default `ON`) — if `ON`, fetches `cxxtn` via
  `FetchContent`; if `OFF`, locates an existing `cxxtn` target via
  `find_package`.
- **`DBGFMT_FETCH_CM`** (default `ON`) — same semantics as
  `DBGFMT_FETCH_CXXTN`, for `cm`.

## License

Apache License 2.0. See [LICENSE](LICENSE).
