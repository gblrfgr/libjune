#include "libjune/collections/vector.h"
#include "libjune/memory.h"
#include "libjune/unit.h"
#include <math.h>

static char *test_push_back_pop_back() {
  lj_vector_t vec = lj_new_vector(sizeof(int), &lj_default_allocator);
  for (int i = 0; i < 15; i++) {
    lj_vector_push_back(&vec, &i);
  }
  lj_assert(lj_vector_size(&vec) == 15,
            "vector length must be 15 when 15 elements are added");
  int j;
  lj_vector_pop_back(&vec, &j);
  lj_vector_pop_back(&vec, NULL);
  lj_assert(true, "passing NULL into pop_back should not cause a segfault");
  lj_assert(
      j == 14,
      "The first value from pop_back should be the last value from push_back");
  lj_vector_clear(&vec);
  lj_assert(lj_vector_size(&vec) == 0,
            "lj_vector_clear should clear the vector");
  lj_assert(!lj_vector_pop_back(&vec, NULL),
            "popping from an empty vector should return false");
  return 0;
}

static char *test_indexing() {
  lj_vector_t vec = lj_new_vector(sizeof(float), &lj_default_allocator);
  for (float i = 0.0f; i < 15.0f; i += 0.1f) {
    lj_vector_push_back(&vec, &i);
  }
  for (size_t i = 0; i < lj_vector_size(&vec); i++) {
    float val;
    lj_assert(lj_vector_get(&vec, i, &val), "index should be within range");
    val = 2.0f * val;
    lj_assert(lj_vector_set(&vec, i, &val), "index should be within range");
  }
  return 0;
}

int main(const int argc, const char **argv) {
  lj_run_test(test_push_back_pop_back);
  lj_run_test(test_indexing);
  lj_finish_tests();
}