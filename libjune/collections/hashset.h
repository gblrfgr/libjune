#ifndef LIBJUNE_HASH_H
#define LIBJUNE_HASH_H

#include <libjune/collections/vector.h>
#include <libjune/memory.h>
#include <stdlib.h>

typedef struct {
  size_t element_size;
  size_t num_buckets;
  lj_vector_t *buckets;
  size_t (*hash_fn)(void *);
  lj_allocator_t *allocator;
} lj_hashset_t;

lj_hashset_t lj_new_hashset(size_t element_size, size_t num_buckets,
                            size_t (*hash_fn)(void *),
                            lj_allocator_t *allocator) {
  lj_vector_t *buckets =
      (lj_vector_t *)lj_allocate(allocator, sizeof(lj_vector_t) * num_buckets);
  for (size_t i = 0; i < num_buckets; i++) {
    buckets[i] = lj_new_vector(element_size, allocator);
  }
  return (lj_hashset_t){
      .element_size = element_size,
      .num_buckets = num_buckets,
      .buckets = buckets,
      .hash_fn = hash_fn,
  };
}

void lj_hashset_delete(lj_hashset_t *set) {
  for (size_t i = 0; i < set->num_buckets; i++) {
    lj_delete_vector(&set->buckets[i]);
  }
  lj_deallocate(set->allocator, set->buckets);
}

#endif