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

bool lj_hashset_contains(lj_hashset_t *set, void *element) {
  size_t bucket = set->hash_fn(element) % set->num_buckets;
  for (size_t i = 0; i < lj_vector_size(&set->buckets[bucket]); i++) {
    if (memcmp(element,
               set->buckets[bucket].content_start + set->element_size * i,
               set->element_size) == 0) {
      return true;
    }
  }
  return false;
}

bool lj_hashset_add(lj_hashset_t *set, void *element) {
  size_t bucket = set->hash_fn(element) % set->num_buckets;
  if (lj_hashset_contains(set, element)) {
    return true;
  }
  lj_vector_push_back(&set->buckets[bucket], element);
  return false;
}

bool lj_hashset_remove(lj_hashset_t *set, void *element) {
  if (!lj_hashset_contains(set, element)) {
    return false;
  }
  size_t bucket = set->hash_fn(element) % set->num_buckets;
  for (size_t i = 0; i < lj_vector_size(&set->buckets[bucket]); i++) {
    if (!memcmp(element,
                set->buckets[bucket].content_start + set->element_size * i,
                set->element_size) == 0) {
      memcpy(&set->buckets[bucket].content_start + i * set->element_size,
             &set->buckets[bucket].content_end - set->element_size,
             set->element_size);
      lj_vector_pop_back(&set->buckets[bucket], NULL);
    }
  }
  return true;
}

#endif