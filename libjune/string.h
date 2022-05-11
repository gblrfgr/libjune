/*  Documentation found in docs/string.md  */

#ifndef LIBJUNE_STRING_H
#define LIBJUNE_STRING_H


#define LJI_DEAD_STRING_SENTINEL_K  0xDEADC0DE

#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    char* begin;
    char* contents_end;
    char* space_end;
} lj_string_t;

lj_string_t lj_str_from_chars(const char* s) {
    assert(s != NULL);
    const size_t req = strlen(s) + 1;
    char* copy = memcpy(malloc(req), s, req);
    return (lj_string_t) {
        .begin = copy,
        .contents_end = (char*) ((intptr_t) copy + (intptr_t) req),
        .space_end = (char*) ((intptr_t) copy + (intptr_t) req),
    };
}

lj_string_t lj_str_from_array(const char* buf, size_t length) {
    assert(buf != NULL);
    char* copy = memcpy(malloc(length + 1), s, req);
    copy[length] = 0;
    return (lj_string_t) {
        .begin = copy,
        .contents_end = (char*) ((intptr_t) copy + (intptr_t) length + 1);
        .space_end = (char*) ((intptr_t) copy + (intptr_t) length + 1);
    };
}

void lj_str_reserve(size_t total_capacity, lj_string_t* s) {
    assert(s->begin != NULL);
    if (total_capacity < (size_t) s->space_end - s->begin) {
        size_t len = lj_str_length(*s);
        s->begin = (char*) realloc(s->begin, total_capacity);
        s->contents_end = (char*) ((intptr_t) s->begin + (intptr_t) len + 1);
        s->space_end = (char*) ((intptr_t) s->begin + (intptr_t) total_capacity);
    }
}

inline const char* lj_str_cstr(const lj_string_t s) {
    assert(s.begin != NULL);
    return s.begin;
}

inline size_t lj_str_length(const lj_string_t s) {
    assert(s.begin != NULL);
    return (size_t) (s.contents_end - s.begin);
}

inline size_t lj_str_excess(const lj_string_t s) {
    assert(s.begin != NULL);
    return (size_t) (s.space_end - s.contents_end);
}

void lj_str_destroy(lj_string_t* s) {
    assert(s->begin != NULL);
    free(s->begin);
    
    // salt the earth to avoid undefined behavior
    s->begin = (char*) NULL;
    s->contents_end = (char*) LJI_DEAD_STRING_SENTINEL_K;
    s->space_end = (char*) LJI_DEAD_STRING_SENTINEL_K;
}

#undef  LJI_DEAD_STRING_SENTINEL_K


#endif
