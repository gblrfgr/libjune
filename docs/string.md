# string.h - types and functions for dealing with strings
`string.h` provides a set of types and functions to handle both ASCII and UTF-8 strings in an efficient manner.

---

## lj_string_t - basic string type
`lj_string_t` is an opaque type designed to hold a basic ASCII string.

**warnings**: implementation is not guaranteed to always be the same. handle only through the functions in `string.h`.

**tags**: #types, #ascii

## lj_str_from_chars - basic ASCII constructor
`lj_str_from_chars(const char* s)` constructs an `lj_string_t` from `s`.

**parameters**: `s`, the input `char` array. must be null-terminated; if not, undefined behavior results. if it isn't null-terminated, use `lj_str_from_array(const char* buf, size_t length)` instead.

**returns**: `lj_string_t` constructed from `s` with no excess memory reserved. does not share the same memory as `s`.

**error states**: if `NDEBUG` is not defined, asserts at runtime that `s` is not `NULL`. could also fail if `malloc` or `memcpy` fail, although in this case you have #bigger-fish-to-fry.

**tags**: #string-operations, #ascii, #ndebug

## lj_str_cstr - null-terminated string extractor
`lj_str_cstr(lj_string_t s)` extracts a null-terminated `const char*` from `s`.

**parameters**: `s`, the input `lj_string_t`.

**returns**: null-terminated `const char*` extracted from `s`, sharing memory with `s` (which is why it's `const`)

**error states**: if `NDEBUG` is not defined, asserts at runtime that `s` has not been destroyed.

**tags**: #string-operations, #ascii, #ndebug