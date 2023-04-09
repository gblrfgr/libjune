#ifndef LIBJUNE_COLLECTIONS_VECTOR_H
#define LIBJUNE_COLLECTIONS_VECTOR_H

#include "libjune/memory.h"
#include "libjune/utils.h"
#include <stdbool.h>
#include <string.h>

/// @brief Arraylike object that can automatically grow as elements are added.
typedef struct {
  lj_allocator_t *allocator;
  size_t element_size;
  char *content_start;
  char *content_end;
  char *buffer_start;
  char *buffer_end;
} lj_vector_t;

const size_t LJI_BUFFER_INITIAL_SIZE = 8U;

/// @brief Create a new lj_vector_t.
/// @param element_size The result of sizeof(element).
/// @param allocator An lj_allocator_t corresponding to the allocator the vector
/// should use.
/// @return The finished vector object.
lj_vector_t lj_new_vector(size_t element_size, lj_allocator_t *allocator) {
  void *buffer = lj_allocate(allocator, LJI_BUFFER_INITIAL_SIZE * element_size);
  return (lj_vector_t){
      .allocator = allocator,
      .element_size = element_size,
      .content_start = buffer,
      .content_end = buffer,
      .buffer_start = buffer,
      .buffer_end = buffer + LJI_BUFFER_INITIAL_SIZE * element_size,
  };
}

/// @brief Delete a vector and free its memory.
/// @param vec The vector to delete.
void lj_delete_vector(lj_vector_t *vec) {
  lj_deallocate(vec->allocator, vec->buffer_start);
  // burn the earth! leave nothing behind!
  vec->content_start = vec->content_end = vec->buffer_start = vec->buffer_end =
      NULL;
}

void lji_vector_grow(lj_vector_t *vec) {
  ptrdiff_t empty_space = (vec->buffer_end - vec->buffer_start) -
                          (vec->content_end - vec->content_start);
  if (empty_space < ((vec->buffer_end - vec->buffer_start) / 2)) {
    size_t new_capacity = (vec->buffer_end - vec->buffer_start);
    new_capacity += new_capacity / 2;
    new_capacity += vec->element_size - (new_capacity % vec->element_size);
    size_t content_length = vec->content_end - vec->content_start;
    void *buffer = lj_allocate(vec->allocator, new_capacity);
    memcpy(buffer, vec->content_start, content_length);
    vec->buffer_start = vec->content_start = (char *)buffer;
    vec->buffer_end = vec->buffer_start + new_capacity;
    vec->content_end = vec->content_start + content_length;
  } else {
    memmove(vec->buffer_start, vec->content_start,
            (vec->content_end - vec->content_start));
  }
}

/// @brief Gets the size of a vector.
/// @param vec The vector in question.
/// @return The size of the vector.
size_t lj_vector_size(lj_vector_t *vec) {
  return (vec->content_end - vec->content_start) / vec->element_size;
}

/// @brief Gets the amount of elements the vector can hold without growing.
/// @param vec The vector in question.
/// @return The number of elements the vector can hold without growing.
size_t lj_vector_capacity(lj_vector_t *vec) {
  return (vec->buffer_end - vec->buffer_start) / vec->element_size;
}

void lj_vector_reserve(lj_vector_t *vec, size_t space) {
  while (lj_vector_capacity(&vec) < space) {
    lji_vector_grow(&vec);
  }
}

void lj_vector_shrink_to_fit(lj_vector_t *vec) {
  char *new_buffer =
      lj_allocate(vec->allocator, vec->content_end - vec->content_start);
  memcpy(new_buffer, vec->content_start, vec->content_end - vec->content_start);
  free(vec->buffer_start);
  vec->buffer_start = new_buffer;
  vec->buffer_end = new_buffer + (vec->content_end - vec->content_start);
  vec->content_end = vec->buffer_end;
  vec->content_start = vec->buffer_start;
}

/// @brief Clear out the contents of a vector.
/// @param vec The vector to clear out.
void lj_vector_clear(lj_vector_t *vec) {
  vec->content_start = vec->content_end = vec->buffer_start;
}

/// @brief Fill a vector with one value; the number of elements will remain the
/// same.
/// @param vec The vector in question.
/// @param val The value to fill with.
void lj_vector_fill(lj_vector_t *vec, void *val) {
  for (char *i = vec->content_start; i != vec->content_end;
       i += vec->element_size) {
    memcpy(i, val, vec->element_size);
  }
}

/// @brief Retrieve an element from a vector.
/// @param vec The vector in question.
/// @param ind The index of the element to retrieve.
/// @param out Pointer to a value that will be replaced by the element's value.
/// Cannot be NULL.
/// @return A bool; if false, the index was out of range and no element was
/// retrieved.
bool lj_vector_get(lj_vector_t *vec, size_t ind, void *out) {
  if ((ind * vec->element_size) >= (vec->content_end - vec->content_start)) {
    return false;
  } else {
    memcpy(out, vec->content_start + vec->element_size * ind,
           vec->element_size);
    return true;
  }
}

/// @brief Set an element in a vector.
/// @param vec The vector in question.
/// @param ind The index at which to place the element.
/// @param val Pointer to the value to place into the vector.
/// @return A bool; if false, the index was out of range and the vector was
/// unchanged.x
bool lj_vector_set(lj_vector_t *vec, size_t ind, void *val) {
  if ((ind * vec->element_size) >= (vec->content_end - vec->content_start)) {
    return false;
  } else {
    memcpy(vec->content_start + vec->element_size * ind, val,
           vec->element_size);
    return true;
  }
}

/// @brief Add a new element to the end of a vector.
/// @param vec The vector in question.
/// @param val The value to add.
void lj_vector_push_back(lj_vector_t *vec, void *val) {
  memcpy(vec->content_end, val, vec->element_size);
  vec->content_end += vec->element_size;
  if (vec->content_end - vec->buffer_end < vec->element_size) {
    lji_vector_grow(vec);
  }
}

/// @brief Take the last element off the end of a vector.
/// @param vec The vector in question.
/// @param out A pointer to a value to replace with the last element of the
/// vector.
///     If NULL is passed in, the last element is just dropped.
/// @return A bool; if the vector was empty, the function returns false and the
/// vector is unchanged.
bool lj_vector_pop_back(lj_vector_t *vec, void *out) {
  if (vec->content_start == vec->content_end) {
    return false;
  } else {
    if (out != NULL) {
      memcpy(out, vec->content_end - vec->element_size, vec->element_size);
    }
    vec->content_end -= vec->element_size;
    return true;
  }
}

/// @brief Add a new element to the beginning of the vector.
/// @param vec The vector in question.
/// @param val A pointer to the value of the new element.
void lj_vector_push_front(lj_vector_t *vec, void *val) {
  if (vec->content_start == vec->buffer_start) {
    if (vec->content_end == vec->buffer_end) {
      lji_vector_grow(vec);
    }
    memmove(vec->content_start + (vec->buffer_end - vec->content_end),
            vec->content_start, (vec->content_end - vec->content_start));
  }
  vec->content_start -= vec->element_size;
  memcpy(vec->content_start, val, vec->element_size);
}

/// @brief Remove the first element in the vector.
/// @param vec The vector in question.
/// @param out A pointer to a value to replace with the element erased. If this
/// pointer is NULL, no value is output.
/// @return A bool indicating if the vector was empty or not and thus if it was
/// changed.
bool lj_vector_pop_front(lj_vector_t *vec, void *out) {
  if (vec->content_start == vec->content_end) {
    return false;
  } else {
    if (out != NULL) {
      memcpy(out, vec->content_start, vec->element_size);
    }
    vec->content_start += vec->element_size;
    return true;
  }
}

/// @brief Erase an element from a vector.
/// @param vec The vector in question.
/// @param ind The index of the element to erase.
/// @param out A pointer to a value to replace with the element erased. If this
/// pointer is NULL, no value is output.
/// @return A bool indicating if the index was in range and thus if the vector
/// was changed.
bool lj_vector_erase(lj_vector_t *vec, size_t ind, void *out) {
  if (ind == 0) {
    lj_vector_pop_front(vec, out);
    return true;
  } else if (ind == lj_vector_size(vec) - 1) {
    lj_vector_pop_back(vec, out);
    return true;
  } else if (ind > lj_vector_size(vec) - 1) {
    return false;
  } else {
    if (out != NULL) {
      lj_vector_get(vec, ind, out);
    }
    char *element_loc = vec->content_start + ind * vec->element_size;
    char *first_after = vec->content_start + (ind + 1) * vec->element_size;
    memmove(element_loc, first_after, vec->content_end - first_after);
    vec->content_end -= vec->element_size;
    return true;
  }
}

#endif