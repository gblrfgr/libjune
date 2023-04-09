#ifndef LIBJUNE_STRING_H
#define LIBJUNE_STRING_H

#include <assert.h>
#include <libjune/collections/vector.h>
#include <libjune/memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef lj_vector_t lj_string_t;

lj_string_t lj_string_new(lj_allocator_t *allocator) {
  lj_string_t result = lj_new_vector(sizeof(char), allocator);
  lj_vector_push_back(&result, '\0');
  return result;
}

void lj_string_delete(lj_string_t *s) { lj_delete_vector(s); }

lj_string_t lj_string_from_array(const char *buf, size_t length,
                                 lj_allocator_t *allocator) {
  lj_string_t result = lj_new_vector(sizeof(char), allocator);
  lj_vector_reserve(&result, length + 1);
  memcpy(result.content_start, buf, length);
  result.content_end = result.buffer_end - result.element_size;
  lj_vector_push_back(&result, '\0');
  return result;
}

lj_string_t lj_string_from_cstr(const char *s, lj_allocator_t *allocator) {
  lj_string_t result = lj_new_vector(sizeof(char), allocator);
  lj_vector_reserve(&result, strlen(s) + 1);
  strcpy(result.content_start, s);
  result.content_end = result.buffer_end;
  return result;
}

void lj_string_reserve(lj_string_t *s, size_t total_capacity) {
  lj_vector_reserve(s, total_capacity);
}

void lj_string_push_back(lj_string_t *s, char c) {
  lj_vector_pop_back(s, NULL);
  lj_vector_push_back(s, &c);
}

lj_string_t lj_string_concatenate(lj_string_t *first, lj_string_t *second) {
  assert(first->allocator == second->allocator);
  lj_string_t result =
      lj_string_from_cstr(first->content_start, first->allocator);
  lj_string_reserve(&result,
                    lj_vector_size(first) + lj_vector_size(second) - 1);
  memcpy(result.content_end - 1, second->content_start, lj_vector_size(second));
  result.content_end =
      result.content_start + lj_vector_size(first) + lj_vector_size(second) - 1;
  return result;
}

#endif
