#ifndef LIBJUNE_STRING_H
#define LIBJUNE_STRING_H


#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../libjune/debug.h"

// length and capacity do not include the null terminator
typedef struct {
    char* begin;
    size_t length;
    size_t capacity;
} lj_string_t;

typedef struct {
    const char* const begin;
    const size_t length;
} lj_stringview_t;


lj_string_t lj_str_from_chars(const char* s) {
    LJ_ASSERT(s != NULL, "s is NULL");
    const size_t req = strlen(s) + 1;
    char* copy = (char*) memcpy(malloc(req), s, req);
    return (lj_string_t) {
        .begin = copy,
        .length = req - 1,
        .capacity = req - 1,
    };
}

lj_string_t lj_str_new_empty() {
    char* space = (char*) malloc(1);
    *space = '\0';
    return (lj_string_t) {
        .begin = space,
        .length = 0,
        .capacity = 0,
    };
}

lj_string_t lj_str_from_array(const char* buf, size_t length) {
    LJ_ASSERT(buf != NULL, "buf is NULL");
    char* copy = (char*) memcpy(malloc(length + 1), buf, length + 1);
    copy[length] = 0;
    return (lj_string_t) {
        .begin = copy,
        .length = length,
        .capacity = length,
    };
}

// total_capacity does not include the null terminator
void lj_str_reserve(lj_string_t* s, size_t total_capacity) {
    LJ_ASSERT(s->begin != NULL, "s has already been destroyed");
    if (total_capacity > s->capacity) {
        s->begin = (char*) realloc(s->begin, total_capacity + 1);
        s->capacity = total_capacity;
    }
}

void lji_str_autoexpand(lj_string_t* s) {
    LJ_ASSERT(s->begin != NULL, "s has already been destroyed");
    size_t necessary = s->capacity << 1;
    lj_str_reserve(s, necessary);
}

void lj_str_pushback( lj_string_t *s, const char c) {
    LJ_ASSERT(s->begin != NULL, "s has already been destroyed");
    if (s->length == s->capacity) {
        lji_str_autoexpand(s);
    }
    (s->begin)[s->length] = c;
    s->length++;
    (s->begin)[s->length] = '\0';
}

void lj_str_pushstr(lj_string_t *onto, const char* s) {
    LJ_ASSERT(onto->begin != NULL, "s has already been destroyed");
    size_t len = strlen(s);
    lj_str_reserve(onto, len + onto->length);
    memcpy(onto->begin + onto->length, s, len + 1);
    onto->length += len;
}


void lj_str_destroy(lj_string_t* s) {
    LJ_ASSERT(s->begin != NULL, "s has already been destroyed");
    free(s->begin);
    
    // salt the earth
    s->begin = (char*) NULL;
    s->length = ~0;
    s->capacity = ~0;
}


#endif
