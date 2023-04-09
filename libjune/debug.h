#ifndef LIBJUNE_DEBUG_H
#define LIBJUNE_DEBUG_H

#include <stdio.h>
#include <stdlib.h>

#ifndef LJ_NODEBUG
#define LJ_ASSERT(condition, message)                                          \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(stderr,                                                          \
              "Assertion failed at line %i (function %s) in file %s: %s\n",    \
              __LINE__, __func__, __FILE__, (message));                        \
      fflush(stdout);                                                          \
      fflush(stderr);                                                          \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0);
#else
#define LJ_ASSERT(condition, message)
#endif

#endif