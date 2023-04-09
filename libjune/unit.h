#ifndef LIBJUNE_UNIT_H
#define LIBJUNE_UNIT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

size_t lji_tests_run = 1U;

#define lj_assert(test, message)                                               \
  do {                                                                         \
    if (!(test)) {                                                             \
      return (message);                                                        \
    }                                                                          \
    lji_tests_run++;                                                           \
  } while (false)

#define lj_run_test(test_fn)                                                   \
  do {                                                                         \
    const char *lji_test_result = (test_fn)();                                 \
    if (lji_test_result) {                                                     \
      fprintf(stderr, "Test #%zu failed with message \"%s\"\n",               \
              lji_tests_run, lji_test_result);                                 \
    }                                                                          \
  } while (false)

#endif