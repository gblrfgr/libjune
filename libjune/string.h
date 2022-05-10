/*  Documentation found in docs/string.md  */

#ifndef LIBJUNE_STRING_H
#define LIBJUNE_STRING_H


#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  char* contents;
  size_t length;
  size_t capacity;
} lji_bigstring_t;

typedef union {
  char _small[sizeof(lji_bigstring_t)];
  lji_bigstring_t _big;
} lj_string_t;

lj_string_t lj_str_from_chars(char* str) {
  unsigned int req = strlen(str) + 1;
  if (req > sizeof(lji_bigstring_t)) {
    char* buf = (char*) malloc(req + (req >> 1));
    memcpy(buf, str, req);
    return (lj_string_t) {
      ._big = (lji_bigstring_t) {
        .contents = buf,
        .capacity = (req + (req >> 1) - 1) | 1,
        .length = req - 1,
      }
    };
  } else {
    lj_string_t res = (lj_string_t) {0};
    memcpy(res._small, str, req);
    res._small[sizeof(lj_string_t) - 1] = (24 - req) << 2;
    return res;
  }
}

inline bool lj_str_is_big(lj_string_t str) {
  return (bool) ((intptr_t) str._big.capacity & 1);
}

char* lj_str_cstr(lj_string_t *s) {
  if (lj_str_is_big(*s)) {
    return s->_big.contents;
  } else {
    return s->_small;
  }
}

size_t lj_str_length(lj_string_t *s) {
  if (lj_str_is_big(*s)) {
    return s->_big.length;
  } else {
    return (size_t) 23 - (s->_small[sizeof(lj_string_t) - 1] >> 1);
  }
}

void lji_bigstr_expand(lji_bigstring_t* b) {
  b->capacity = (b->capacity + (b->capacity >> 1)) | 1;
  b->contents = (char*) realloc(b->contents, b->capacity);
}


#endif
