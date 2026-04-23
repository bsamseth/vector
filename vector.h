#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * Instantiate a vector for the given element type.
 *
 * The second argument is the resulting vector type's name, which will be an
 * alias for a simple pointer to the element type, provided only for
 * readability.
 */
#define VECTOR_IMPL(element_type, typealias)                                   \
  typedef element_type *typealias;                                             \
                                                                               \
  [[maybe_unused]] static inline size_t typealias##_len(const typealias vec) { \
    return rawvec_len(vec) / sizeof(element_type);                             \
  }                                                                            \
  [[maybe_unused]] static inline size_t typealias##_capacity(                  \
      const typealias vec) {                                                   \
    return rawvec_capacity(vec) / sizeof(element_type);                        \
  }                                                                            \
  [[maybe_unused]] static inline size_t typealias##_remaining(                 \
      const typealias vec) {                                                   \
    return rawvec_remaining(vec) / sizeof(element_type);                       \
  }                                                                            \
  [[maybe_unused]] static inline typealias typealias##_spare_capacity(         \
      typealias vec) {                                                         \
    return &vec[typealias##_len(vec)];                                         \
  }                                                                            \
  [[maybe_unused]] static inline bool typealias##_is_empty(                    \
      const typealias vec) {                                                   \
    return rawvec_is_empty(vec);                                               \
  }                                                                            \
  [[maybe_unused]] static inline typealias typealias##_init(                   \
      size_t initial_capacity) {                                               \
    rawvec vec = rawvec_init(initial_capacity * sizeof(element_type));         \
    return (typealias)vec;                                                     \
  }                                                                            \
  [[maybe_unused]] static inline void typealias##_free(typealias ptr) {        \
    rawvec_free((rawvec)ptr);                                                  \
  }                                                                            \
  [[maybe_unused]] static inline bool typealias##_resize(typealias *ptr,       \
                                                         size_t capacity) {    \
    return rawvec_resize((rawvec *)ptr, capacity * sizeof(element_type));      \
  }                                                                            \
  [[maybe_unused]] static inline bool typealias##_reserve(typealias *ptr,      \
                                                          size_t additional) { \
    return rawvec_reserve((rawvec *)ptr, additional * sizeof(element_type));   \
  }                                                                            \
  [[maybe_unused]] static inline bool typealias##_shrink_to_fit(               \
      typealias *ptr) {                                                        \
    return rawvec_shrink_to_fit((rawvec *)ptr);                                \
  }                                                                            \
  [[maybe_unused]] static inline void typealias##_set_len(typealias ptr,       \
                                                          size_t len) {        \
    rawvec_set_len((rawvec)ptr, len * sizeof(element_type));                   \
  }                                                                            \
  [[maybe_unused]] static inline bool typealias##_memcpy(                      \
      typealias *ptr, size_t offset, const element_type *source, size_t n) {   \
    return rawvec_memcpy((rawvec *)ptr, offset * sizeof(element_type),         \
                         (void *)source, n * sizeof(element_type));            \
  }                                                                            \
  [[maybe_unused]] static inline bool typealias##_extend(                      \
      typealias *ptr, const element_type *source, size_t n) {                  \
    return rawvec_extend((rawvec *)ptr, (void *)source,                        \
                         n * sizeof(element_type));                            \
  }                                                                            \
  [[maybe_unused]] static inline bool typealias##_push(typealias *ptr,         \
                                                       element_type element) { \
    if (sizeof(element_type) == 1)                                             \
      return rawvec_push((rawvec *)ptr, *(char *)&element);                    \
    return typealias##_extend(ptr, &element, 1);                               \
  }                                                                            \
  [[maybe_unused]] static inline element_type typealias##_pop(typealias ptr) { \
    size_t length = typealias##_len(ptr);                                      \
    assert(length > 0);                                                        \
    element_type value = ptr[length - 1];                                      \
    typealias##_set_len(ptr, length - 1);                                      \
    return value;                                                              \
  }                                                                            \
  [[maybe_unused]] static inline element_type *typealias##_last_ptr(           \
      typealias ptr) {                                                         \
    size_t length = typealias##_len(ptr);                                      \
    assert(length > 0);                                                        \
    return &ptr[length - 1];                                                   \
  }                                                                            \
  [[maybe_unused]] static inline element_type typealias##_last(                \
      typealias ptr) {                                                         \
    return *typealias##_last_ptr(ptr);                                         \
  }                                                                            \
  [[maybe_unused]] static inline bool typealias##_extend_from_within(          \
      typealias *ptr, size_t offset, const element_type *source, size_t n) {   \
    return rawvec_extend_from_within(                                          \
        (rawvec *)ptr, offset * sizeof(element_type), (void *)source,          \
        n * sizeof(element_type));                                             \
  }

// A raw vector of bytes.
//
// This is just an alias for `char *`, and it can be used as such with two
// caveats:
//
// 1. Using a `char*` not obtained from `rawvec_init` in any `rawvec_*` API is
// undefined behaviour.
// 2. Calling `free` on a rawvec is undefined behaviour. Use `rawvec_free`
// instead.
//
// The functions in the rawvec API allow you to pull out additinal data such as
// the length of the vector, or its capacity, as well as dynamically extend it
// by pushing and copying data into the vector.
typedef char *rawvec;

// A raw vector is a vector whose element type is a single byte.
//
// The vector is implemented with this dynamically sized structure. The user
// will get a pointer to the `data` member in this struct, which lets the user
// use the vector as a plain pointer, but with magic powers if desired.
typedef struct {
  size_t count;
  size_t capacity;
  char data[];
} __rawvec_t;

// =============================================================================
// Private macros
// =============================================================================
//  Return a `rawvec` from a `__rawvec_t*`.
#define __rawvec_user_ptr_from_rawvec(vec) ((rawvec)(&((size_t *)(vec))[2]))
//  Return a `__rawvec_t*` from a `rawvec`.
#define __rawvec_from_user_ptr(ptr) ((__rawvec_t *)(((size_t *)(ptr)) - 2))

// =============================================================================
// Public macros
// =============================================================================
// Return the number of elements in the vector.
#define rawvec_len(ptr) __rawvec_from_user_ptr((ptr))->count
// Return the total number of elements the vector can hold without reallocating.
#define rawvec_capacity(ptr) __rawvec_from_user_ptr((ptr))->capacity
// Return the number of elements that can be pushed without reallocating.
#define rawvec_remaining(ptr) (rawvec_capacity(ptr) - rawvec_len(ptr))
// Return true if the vector is empty.
#define rawvec_is_empty(ptr) (rawvec_len(ptr) == 0)

// Initialize a rawvec with some initial capacity.
rawvec rawvec_init(size_t initial_capacity);

// Free the allocation associated with `ptr`.
void rawvec_free(rawvec ptr);

// Resize the vector so that its capacity is the new capacity.
//
// Returns true if the vector was moved during resizing.
bool rawvec_resize(rawvec *ptr, size_t capacity);

// Reserve capacity for at least `additional` more elements to be inserted.
//
// The implementation may allocate more than this, but after this call it is
// guaranteed that the capacity is at least `rawvec_len(*ptr) + additional`.
bool rawvec_reserve(rawvec *ptr, size_t additional);

// Set the number of initialized elements to `len`.
//
// Undefined if `len` exceeds the capacity. This will not initialize any
// elements if the length increases, it is the users responsibility to ensure
// any such elements are initialized before use.
void rawvec_set_len(rawvec ptr, size_t len);

// Push a new byte to the end of the vector.
//
// Returns true if the vector was moved during resizing.
bool rawvec_push(rawvec *ptr, char byte);

// Pop the last element from the vector.
//
// Returns the value that was present, and decreases the length of the vector by
// one. Undefined if the vector is empty.
char rawvec_pop(rawvec ptr);

// Like `memcpy(&ptr[offset], source, n)` while ensuring the vector has enough
// space.
//
// Undefined if offset exceeds the current length.
// Undefined if `[source, source+n)` overlaps with the vector's memory, as the
// vector might be moved if it needs to grow.
//
// Returns true if the vector was moved during resizing.
bool rawvec_memcpy(rawvec *ptr, size_t offset, const void *source, size_t n);

// Shorthand for `rawvec_memcpy` with `offset = rawvec_len(ptr)`.
bool rawvec_extend(rawvec *ptr, const void *source, size_t n);

// Shrink the capacity to match the current length.
//
// Returns true if the vector was moved during resizing, or false if the
// capacity already matches the length.
bool rawvec_shrink_to_fit(rawvec *ptr);

// Copy n bytes from `source` into the vector at `offset`, shifting later
// elements to the right.
//
// Undefined if `[source, source+n)` overlaps with the vector's memory, as the
// vector might be moved if it needs to grow.
//
// Returns true if the vector was moved during resizing.
bool rawvec_extend_from_within(rawvec *ptr, size_t offset, const void *source,
                               size_t n);

#ifdef VECTOR_INCLUDE_IMPLEMENTATION
#ifndef VECTOR_IMPLEMENTATION_INCLUDED
#define VECTOR_IMPLEMENTATION_INCLUDED
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

rawvec rawvec_init(size_t initial_capacity) {
  __rawvec_t *vec = malloc(sizeof(__rawvec_t) + initial_capacity);
  assert(vec != NULL);
  vec->count = 0;
  vec->capacity = initial_capacity;
  return __rawvec_user_ptr_from_rawvec(vec);
}

void rawvec_free(rawvec ptr) {
  __rawvec_t *vec = __rawvec_from_user_ptr(ptr);
  free(vec);
}

bool rawvec_resize(rawvec *ptr, size_t capacity) {
  __rawvec_t *vec = __rawvec_from_user_ptr(*ptr);
  size_t old_count = vec->count;

  vec = realloc(vec, sizeof(__rawvec_t) + capacity);
  assert(vec != NULL);
  vec->capacity = capacity;
  if (old_count > capacity)
    vec->count = capacity;
  else
    vec->count = old_count;

  rawvec new_user_ptr = __rawvec_user_ptr_from_rawvec(vec);
  bool changed = *ptr != new_user_ptr;
  *ptr = new_user_ptr;

  return changed;
}

bool rawvec_reserve(rawvec *ptr, size_t additional) {
  __rawvec_t *vec = __rawvec_from_user_ptr(*ptr);

  if (additional == 0)
    return false;

  // First check if we already have enough room.
  if (additional > SIZE_MAX - vec->count) // Avoid overflow.
    return false;
  size_t required_capacity = vec->count + additional;
  if (required_capacity <= vec->capacity)
    return false;

  // Otherwise, determine the next size to grow to.
  // Growth strategy: Multiply by 1.625 until enough room.
  // If initial capacity is zero, start with the required capacity.
  size_t new_capacity = vec->capacity ? vec->capacity : required_capacity;
  while (new_capacity < required_capacity) {
    size_t old_capacity = new_capacity;
    new_capacity = (old_capacity * 13) / 8;
    if (new_capacity <= old_capacity) {
      new_capacity = required_capacity;
      break;
    }
  }

  return rawvec_resize(ptr, new_capacity);
}

void rawvec_set_len(rawvec ptr, size_t len) {
  __rawvec_t *vec = __rawvec_from_user_ptr(ptr);
  assert(len <= vec->capacity);
  vec->count = len;
}

bool rawvec_push(rawvec *ptr, char byte) {
  bool changed = rawvec_reserve(ptr, 1);
  __rawvec_t *vec = __rawvec_from_user_ptr(*ptr);
  (*ptr)[vec->count++] = byte;
  return changed;
}

char rawvec_pop(rawvec ptr) {
  __rawvec_t *vec = __rawvec_from_user_ptr(ptr);
  assert(vec->count > 0);
  return ptr[--vec->count];
}

bool rawvec_memcpy(rawvec *ptr, size_t offset, const void *source, size_t n) {
  __rawvec_t *vec = __rawvec_from_user_ptr(*ptr);
  assert(offset <= vec->count);
  size_t n_added = n - (vec->count - offset);
  bool changed = rawvec_reserve(ptr, n_added);

  memcpy((*ptr) + offset, source, n);
  if (changed)
    vec = __rawvec_from_user_ptr(*ptr);
  vec->count += n_added;
  return changed;
}

bool rawvec_extend(rawvec *ptr, const void *source, size_t n) {
  return rawvec_memcpy(ptr, rawvec_len(*ptr), source, n);
}

bool rawvec_shrink_to_fit(rawvec *ptr) {
  __rawvec_t *vec = __rawvec_from_user_ptr(*ptr);
  if (vec->capacity == vec->count)
    return false;
  return rawvec_resize(ptr, vec->count);
}

bool rawvec_extend_from_within(rawvec *ptr, size_t offset, const void *source,
                               size_t n) {
  __rawvec_t *vec = __rawvec_from_user_ptr(*ptr);
  assert(offset <= vec->count);
  bool changed = rawvec_reserve(ptr, n);
  size_t n_elements_from_offset = rawvec_len(*ptr) - offset;
  memmove(*ptr + offset + n, *ptr + offset, n_elements_from_offset);
  memcpy(*ptr + offset, source, n);
  if (changed)
    vec = __rawvec_from_user_ptr(*ptr);
  vec->count += n;
  return changed;
}
#endif
#endif
