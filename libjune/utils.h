/*  Documentation found in docs/utils.md  */

#ifndef LIBJUNE_UTILS_H
#define LIBJUNE_UTILS_H


#define   LJ_SWAP(type, a, b)  do { type temp = *(a); \
                                    *(a) = *(b); \
                                    *(b) = temp; } while (0);


#endif