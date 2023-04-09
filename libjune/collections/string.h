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

void lj_string_delete(lj_string_t *str) { lj_delete_vector(str); }

lj_string_t lj_string_from_array(const char *buf, size_t len,
                                 lj_allocator_t *allocator) {
  lj_string_t result = lj_new_vector(sizeof(char), allocator);
  lj_vector_reserve(&result, len + 1);
  memcpy(result.content_start, buf, len);
  result.content_end = result.buffer_end - result.element_size;
  lj_vector_push_back(&result, '\0');
  return result;
}

lj_string_t lj_string_from_cstr(const char *str, lj_allocator_t *allocator) {
  lj_string_t result = lj_new_vector(sizeof(char), allocator);
  lj_vector_reserve(&result, strlen(str) + 1);
  strcpy(result.content_start, str);
  result.content_end = result.buffer_end;
  return result;
}

const char *lj_string_to_cstr(lj_string_t *str) { return str->content_start; }

size_t lj_string_length(lj_string_t *str) { return lj_vector_size(str) - 1; }

void lj_string_reserve(lj_string_t *str, size_t vol) {
  lj_vector_reserve(str, vol);
}

bool lj_string_set(lj_string_t *str, size_t ind, char val) {
  if (ind >= lj_string_length(str)) {
    return false;
  }
  return lj_vector_set(str, ind, &val);
}

bool lj_string_get(lj_string_t *str, size_t ind, char *out) {
  if (ind >= lj_string_length(str)) {
    return false;
  }
  return lj_vector_get(str, ind, out);
}

void lj_string_push_back(lj_string_t *str, char val) {
  lj_vector_pop_back(str, NULL);
  lj_vector_push_back(str, &val);
}

bool lj_string_pop_back(lj_string_t *str, char *out) {
  lj_vector_pop_back(str, NULL);
  if (!lj_vector_pop_back(str, out)) {
    char NULL_TERMINATOR = '\0';
    lj_vector_push_back(str, &NULL_TERMINATOR);
    return false;
  }
  return true;
}

void lj_string_push_front(lj_string_t *str, char val) {
  lj_vector_push_front(str, &val);
}

bool lj_string_pop_front(lj_string_t *str, char *out) {
  if (lj_string_length(str) == 0) {
    return false;
  }
  return lj_vector_pop_front(str, out);
}

bool lj_string_concatenate(lj_string_t *one, lj_string_t *two,
                           lj_string_t *out) {
  if (one->allocator != two->allocator) {
    return false;
  }
  *out = lj_string_from_cstr(one->content_start, one->allocator);
  lj_string_reserve(out, lj_vector_size(two) + lj_vector_size(two) - 1);
  memcpy(out->content_end - 1, two->content_start, lj_vector_size(two));
  out->content_end =
      out->content_start + lj_vector_size(one) + lj_vector_size(two) - 1;
  return true;
}

bool lj_string_substring(lj_string_t *str, size_t from, size_t to,
                         lj_string_t *out) {
  if (from >= to || to >= lj_vector_size(str)) {
    return false;
  }
  *out = lj_string_from_array(str->content_start + from, to - from, str->allocator);
  return true;
}

#endif
