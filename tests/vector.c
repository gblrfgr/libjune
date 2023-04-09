#include "libjune/collections/vector.h"
#include "libjune/memory.h"
#include <assert.h>
#include <stdio.h>

int main(const int argc, const char **argv) {
  lj_vector_t vec = lj_new_vector(sizeof(int), &lj_default_allocator);
  for (int i = 0; i < 15; i++) {
    lj_vector_push_back(&vec, &i);
  }
  int j;
  lj_vector_pop_front(&vec, &j);
  assert(j == 0);
  lj_vector_pop_front(&vec, NULL);
  lj_vector_pop_back(&vec, NULL);
  lj_vector_pop_back(&vec, &j);
  assert(j == 13);
  assert(lj_vector_size(&vec) == 11);
  size_t cap = lj_vector_capacity(&vec);
  lj_vector_clear(&vec);
  assert(lj_vector_size(&vec) == 0);
  assert(cap == lj_vector_capacity(&vec));
  assert(lj_vector_pop_back(&vec, NULL) == false);
  lj_vector_push_front(&vec, &j);
  assert(lj_vector_size(&vec) == 1);
  assert(lj_vector_get(&vec, 0, &j) == true);
  assert(j == 13);
  assert(lj_vector_get(&vec, 19, &j) == false);
  printf("All tests passed!\n");
}