#ifndef LIBJUNE_UNIT_H
#define LIBJUNE_UNIT_H

#include <stdbool.h>
#include <stdlib.h>

size_t lji_tests_run = 0U;
size_t lji_tests_failed = 0U;

#define lj_assert(test, message)                                               \
  do {                                                                         \
    if (!(test)) {                                                             \
      lji_tests_failed++;                                                      \
      return (message);                                                        \
    }                                                                          \
  } while (false)

#define lj_run_test(test_fn)                                                   \
  do {                                                                         \
    const char *lji_test_result = (test_fn)();                                 \
    lji_tests_run++;                                                           \
    if (lji_test_result) {                                                     \
      fprintf(stderr, "Test #%zu failed with message \"%s\"\n", lji_tests_run, \
              lji_test_result);                                                \
    }                                                                          \
  } while (false)

#define lj_finish_tests()                                                      \
  do {                                                                         \
    printf("%zu of %zu tests passed\n", lji_tests_run - lji_tests_failed,      \
           lji_tests_run);                                                     \
  } while (false)

#endif
