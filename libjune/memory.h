/// @file libjune/memory.h

#ifndef LIBJUNE_MEMORY_H
#define LIBJUNE_MEMORY_H

#include <stdlib.h>

/// @brief Type used to represent an allocator in an implementation-agnostic
/// way.
typedef struct lj_allocator_t {
  void *(*allocate_fn)(void *, size_t);
  void (*deallocate_fn)(void *, void *);
  void *state;
} lj_allocator_t;

/// @brief Allocate memory from an allocator. If no memory is available or the
/// allocator reaches some internal error, NULL is returned.
/// @param allocator The allocator in question.
/// @param volume The number of bytes to allocate.
/// @return A pointer to the new memory, or NULL if allocation failed.
void *lj_allocate(lj_allocator_t *allocator, size_t volume) {
  return allocator->allocate_fn(allocator->state, volume);
}

/// @brief Deallocate memory previously allocated using an allocator. Passing in
/// memory already deallocated or not previously allocated by the same allocator
/// has undefined behavior. Passing in NULL is a no-op.
/// @param allocator The allocator in question.
/// @param memory The memory to deallocate.
void lj_deallocate(lj_allocator_t *allocator, void *memory) {
  allocator->deallocate_fn(allocator->state, memory);
}

/// @private
void *lji_default_allocate_fn(void *state, size_t volume) {
  return malloc(volume);
}

/// @private
void lji_default_deallocate_fn(void *state, void *memory) { free(memory); }

/// @brief Default allocator. Uses libc malloc(1) and free(1).
lj_allocator_t lj_default_allocator = (lj_allocator_t){
    .allocate_fn = &lji_default_allocate_fn,
    .deallocate_fn = &lji_default_deallocate_fn,
    .state = NULL,
};

#endif