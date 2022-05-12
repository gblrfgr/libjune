/*  Documentation found in docs/string.md  */

#ifndef LIBJUNE_STRING_H
#define LIBJUNE_STRING_H


#define LJI_DEAD_STRING_SENTINEL_K  0xDEADC0DE

#include <string.h>
#include <stdlib.h>
#include <assert.h>


// OFF-BY-ONE PITFALL:
// `contents_end` points to the null terminator. `space_end` points to the last allocated character.
typedef struct {
    char* begin;
    char* contents_end;
    char* space_end;
} lj_string_t;


lj_string_t lj_str_from_chars(const char* s) {
    assert(s != NULL);
    const size_t req = strlen(s);
    char* copy = memcpy(malloc(req + 1), s, req + 1);
    return (lj_string_t) {
        .begin = copy,
        .contents_end = (char*) ((intptr_t) copy + (intptr_t) req),
        .space_end = (char*) ((intptr_t) copy + (intptr_t) req),
    };
}

lj_string_t lj_str_new_empty() {
    char* space = malloc(1);
    *space = '\0';
    return (lj_string_t) {
        .begin = space,
        .contents_end = space,
        .space_end = space,
    };
}

lj_string_t lj_str_from_array(const char* buf, size_t length) {
    assert(buf != NULL);
    char* copy = memcpy(malloc(length + 1), s, length + 1);
    copy[length] = 0;
    return (lj_string_t) {
        .begin = copy,
        .contents_end = (char*) ((intptr_t) copy + (intptr_t) length);
        .space_end = (char*) ((intptr_t) copy + (intptr_t) length);
    };
}

// OFF-BY-ONE PITFALL: `total_capacity` does not include the null terminator
void lj_str_reserve(size_t total_capacity, lj_string_t* s) {
    assert(s->begin != NULL);
    if (total_capacity + 1 < lj_str_capacity(*s)) {
        size_t len = lj_str_length(*s);
        s->begin = (char*) realloc(s->begin, total_capacity + 1);
        s->contents_end = (char*) ((intptr_t) s->begin + (intptr_t) len);
        s->space_end = (char*) ((intptr_t) s->begin + (intptr_t) total_capacity);
    }
}

void lji_str_autoexpand(lj_string_t* s) {
    assert(s->begin != NULL);
    size_t necessary = lj_str_capacity(*s) + ((lj_str_capacity(*s) >> 1) | 1);
    lj_str_reserve(necessary, s);
}

void lj_str_pushback(const char c, lj_string_t *s) {
    assert(s->begin != NULL);
    if (lj_str_excess(*s) == 0) {
        lji_str_autoexpand(s);
    }
    *(s->contents_end) = c;
    s->contents_end++;
    *(s->contents_end) = '\0';
}

void lj_str_pushstr(const char* s, lj_string_t *onto) {
    assert(s->begin != NULL);
    size_t len = strlen(s);
    while (len > lj_str_excess(*s)) {
        lji_str_autoexpand(s);
    }
    memcpy(onto->contents_end, s, len + 1);
    onto->contents_end += len;
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

inline size_t lj_str_capacity(const lj_string_t s) {
    assert(s.begin != NULL);
    return (size_t) ((s.space_end - s.begin) + 1);
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
