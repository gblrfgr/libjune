/// @file libjune/collections/string.h

#ifndef LIBJUNE_STRING_H
#define LIBJUNE_STRING_H

#include <libjune/collections/vector.h>
#include <libjune/memory.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @brief A basic ASCII string.
typedef lj_vector_t lj_string_t;

/// @brief Create a new, empty string.
/// @param allocator The allocator to use.
/// @return The resulting string.
lj_string_t lj_string_new(lj_allocator_t *allocator) {
  lj_string_t result = lj_new_vector(sizeof(char), allocator);
  const char terminator = '\0';
  lj_vector_push_back(&result, &terminator);
  return result;
}

/// @brief Free the memory associated with a string and delete it.
/// @param str The string in question.
void lj_string_delete(lj_string_t *str) { lj_delete_vector(str); }

/// @brief Create a string from a character buffer.
/// @param buf The character buffer.
/// @param len The number of characters in the buffer. All characters are
/// assumed to be non-null, including the final character.
/// @param allocator The allocator to use.
/// @return The new string.
lj_string_t lj_string_from_array(const char *buf, size_t len,
                                 lj_allocator_t *allocator) {
  lj_string_t result = lj_new_vector(sizeof(char), allocator);
  lj_vector_reserve(&result, len + 1);
  memcpy(result.content_start, buf, len);
  result.content_end = result.content_start + len;
  char NULL_TERMINATOR = '\0';
  lj_vector_push_back(&result, &NULL_TERMINATOR);
  return result;
}

/// @brief Create a string from a null-terminated character string.
/// @param str The null-terminated string.
/// @param allocator The allocator to use.
/// @return The new string object.
lj_string_t lj_string_from_cstr(const char *str, lj_allocator_t *allocator) {
  lj_string_t result = lj_new_vector(sizeof(char), allocator);
  lj_vector_reserve(&result, strlen(str) + 1);
  strcpy(result.content_start, str);
  result.content_end = result.content_start + strlen(str) + 1;
  return result;
}

/// @brief Convert an lj_string_t to a C-style string.
/// @param str The string to convert.
/// @return A constant null terminated string with the same contents. This
/// string is not guaranteed to have a different memory location from the
/// original string.
const char *lj_string_to_cstr(lj_string_t *str) { return str->content_start; }

/// @brief Determine the length of a string.
/// @param str The string in question.
/// @return The length of the string.
size_t lj_string_length(lj_string_t *str) { return lj_vector_size(str) - 1; }

/// @brief Grow the internal buffer for a string to be able to hold at least
/// some amount of characters without needing to grow.
/// @param str The string in question.
/// @param vol The minimum number of characters the buffer should be able to
/// hold without growing.
void lj_string_reserve(lj_string_t *str, size_t vol) {
  lj_vector_reserve(str, vol + 1);
}

/// @brief Remove all extra space in the string buffer.
/// @param str The string in question.
void lj_string_shrink_to_fit(lj_string_t *str) { lj_vector_shrink_to_fit(str); }

/// @brief Replace a character in a string.
/// @param str The string in question.
/// @param ind The index of the character to replace.
/// @param val The character to add to the string.
/// @return A bool indicating whether or not the index was in range.
bool lj_string_set(lj_string_t *str, size_t ind, char val) {
  if (ind >= lj_string_length(str)) {
    return false;
  }
  return lj_vector_set(str, ind, &val);
}

/// @brief Retrieve a character from a string.
/// @param str The string in question.
/// @param ind The index of the character.
/// @param out A pointer to a character to replace with the value from the
/// string.
/// @return A bool indicating whether or not the index was in range.
bool lj_string_get(lj_string_t *str, size_t ind, char *out) {
  if (ind >= lj_string_length(str)) {
    return false;
  }
  return lj_vector_get(str, ind, out);
}

/// @brief Add a character to the end of a string.
/// @param str The string in question.
/// @param val The character to add.
void lj_string_push_back(lj_string_t *str, char val) {
  lj_vector_pop_back(str, NULL);
  lj_vector_push_back(str, &val);
}

/// @brief Remove the last character from a string.
/// @param str The string in question.
/// @param out A pointer to a value to be replaced with the last character from
/// the string. If NULL is passed in, the last character is just dropped.
/// @return A bool indicating if a character was successfully removed.
bool lj_string_pop_back(lj_string_t *str, char *out) {
  lj_vector_pop_back(str, NULL);
  if (!lj_vector_pop_back(str, out)) {
    char NULL_TERMINATOR = '\0';
    lj_vector_push_back(str, &NULL_TERMINATOR);
    return false;
  }
  return true;
}

/// @brief Determines if two strings are equal.
/// @param a The first string.
/// @param b The second string.
/// @return A bool indicating if the strings are equal.
bool lj_string_equals(lj_string_t *a, lj_string_t *b) {
  return strcmp(lj_string_to_cstr(a), lj_string_to_cstr(b)) == 0;
}

/// @brief Concatenate two strings.
/// @param one The first string.
/// @param two The second string. It must have the same allocator as the first
/// string, or else concatenation will fail.
/// @param out A pointer to a string object to replace with the final result.
/// Any existing string at this pointer will not be deleted by this.
/// @return A bool indicating if the concatenation was successful.
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

/// @brief Returns a substring from an initial string.
/// @param str The initial string.
/// @param from The index to start the substring from.
/// @param to The first index not to include in the substring.
/// @param out A pointer to a string object to replace with the substring. Any
/// existing string at this pointer will not be deleted by this.
/// @return A bool indicating whether the indices provided were valid.
bool lj_string_substring(lj_string_t *str, size_t from, size_t to,
                         lj_string_t *out) {
  if (from >= to || to >= lj_vector_size(str)) {
    return false;
  }
  *out = lj_string_from_array(str->content_start + from, to - from,
                              str->allocator);
  return true;
}

/// @brief Uses a string as a formatting string and outputs the resulting
/// string.
/// @param fmt The formatting string to use.
/// @param varargs The remaining arguments; formatting strings work exactly as
/// printf.
/// @return The resulting formatted string.
lj_string_t lj_string_format(lj_string_t *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  va_list final_args;
  va_copy(final_args, args);
  lj_string_t result = lj_string_new(fmt->allocator);
  const size_t INITIAL_TRY_LENGTH = 12;
  lj_string_reserve(&result, INITIAL_TRY_LENGTH);
  size_t necessary = vsnprintf(result.content_start, INITIAL_TRY_LENGTH,
                               lj_string_to_cstr(fmt), args);
  if (necessary > INITIAL_TRY_LENGTH - 1) {
    lj_string_reserve(&result, necessary + 1);
    vsprintf(result.content_start, lj_string_to_cstr(fmt), final_args);
  }
  result.content_end = result.content_start + necessary + 1;
  return result;
}

#endif