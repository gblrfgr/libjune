/*  Documentation found in docs/string.md  */

#ifndef LIBJUNE_STRING_H
#define LIBJUNE_STRING_H


#include <string.h>
#include <stdlib.h>

typedef struct {
    char* begin;
    char* contents_end;
    char* space_end;
} lj_string_t;

lj_string_t lj_str_from_chars(const char* s) {
    const size_t req = strlen(s) + 1;
    char* buf = memcpy(malloc(req), s, req);
    return (lj_string_t) {
        .begin = buf,
        .contents_end = (char*) ((intptr_t) buf + (intptr_t) req),
        .space_end = (char*) ((intptr_t) buf + (intptr_t) req),
    };
}

const char* lj_str_cstr(const lj_string_t s) {
    return s.begin;
}

size_t lj_str_length(const lj_string_t s) {
    return (size_t) (s.contents_end - s.begin);
}

size_t lj_str_excess(const lj_string_t s) {
    return (size_t) (s.space_end - s.contents_end);
}


#endif
