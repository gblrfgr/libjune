#ifndef LIBJUNE_MEMORY_H
#define LIBJUNE_MEMORY_H

#include <stdlib.h>

typedef struct lj_allocator_t {
  void *(*allocate_fn)(void *, size_t);
  void (*deallocate_fn)(void *, void *);
  void *state;
} lj_allocator_t;

void *lj_allocate(lj_allocator_t *allocator, size_t volume) {
  return allocator->allocate_fn(allocator->state, volume);
}

void lj_deallocate(lj_allocator_t *allocator, void *memory) {
  allocator->deallocate_fn(allocator->state, memory);
}

void *lji_default_allocate_fn(void *state, size_t volume) {
  return malloc(volume);
}

void lji_default_deallocate_fn(void *state, void *memory) { free(memory); }

lj_allocator_t lj_default_allocator = (lj_allocator_t){
    .allocate_fn = &lji_default_allocate_fn,
    .deallocate_fn = &lji_default_deallocate_fn,
    .state = NULL,
};

#endif