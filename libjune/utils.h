/*  Documentation found in docs/utils.md  */

#ifndef LIBJUNE_UTILS_H
#define LIBJUNE_UTILS_H


#include <stdbool.h>

#define   LJ_SWAP(type, a, b)  do { type temp = *(a); \
                                    *(a) = *(b); \
                                    *(b) = temp; } while (0);


inline bool lj_isalnum(char c) {
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           ('0' <= c && c <= '9');
}

#endif