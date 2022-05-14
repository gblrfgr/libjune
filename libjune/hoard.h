#ifndef LIBJUNE_HOARD_H
#define LIBJUNE_HOARD_H


#include <stdlib.h>
#include "libjune\utils.h"

#define LJ_VECTOR(type_t)           \
    struct { (type_t)* contents;    \
             size_t length;         \
             size_t capacity; }

#define LJ_VECTOR_NEW(type_t)                                   \
    (LJ_VECTOR(type)) { .contents = malloc(sizeof((type_t))),   \
                        .length = 1,                            \
                        .capacity = 1 }

#define LJ_VECTOR_PUSHBACK(vec_ptr, value)                  \
    do {                                                    \
        if ((vec_ptr)->length == (vec_ptr)->capacity) {     \
            LJ_VECTOR_AUTOEXPAND((vec_ptr));                \
        }                                                   \
        (vec_ptr)->contents[(vec_ptr)->length] = value;     \
        (vec_ptr)->length++;                                \
    } while (0);

#define LJ_VECTOR_AUTOEXPAND(vec_ptr)                                                       \
    do {                                                                                    \
        (vec_ptr)->capacity = (vec_ptr)->capacity << 1;                                     \
        (vec_ptr)->contents = realloc((vec_ptr)->contents,                                  \
                                      sizeof(*(vec_ptr)->contents) * (vec_ptr)->capacity);  \
    } while (0);


#endif