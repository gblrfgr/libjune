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
  printf("All tests passed!\n");
}