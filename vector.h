#pragma once

#include "rawvec.h"

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
  static inline size_t typealias##_len(typealias vec) {                        \
    return rawvec_len(vec) / sizeof(element_type);                             \
  }                                                                            \
  static inline size_t typealias##_capacity(typealias vec) {                   \
    return rawvec_capacity(vec) / sizeof(element_type);                        \
  }                                                                            \
  static inline size_t typealias##_remaining(typealias vec) {                  \
    return rawvec_remaining(vec) / sizeof(element_type);                       \
  }                                                                            \
  static inline bool typealias##_is_empty(typealias vec) {                     \
    return rawvec_is_empty(vec);                                               \
  }                                                                            \
  static inline typealias typealias##_init(size_t initial_capacity) {          \
    rawvec vec = rawvec_init(initial_capacity * sizeof(element_type));         \
    return (typealias)vec;                                                     \
  }                                                                            \
  static inline void typealias##_free(typealias ptr) {                         \
    rawvec_free((rawvec)ptr);                                                  \
  }                                                                            \
  static inline bool typealias##_resize(typealias *ptr, size_t capacity) {     \
    return rawvec_resize((rawvec *)ptr, capacity * sizeof(element_type));      \
  }                                                                            \
  static inline bool typealias##_set_len(typealias ptr, size_t len) {          \
    return rawvec_set_len((rawvec)ptr, len * sizeof(element_type));            \
  }                                                                            \
  static inline bool typealias##_push(typealias *ptr, element_type element) {  \
    assert(!"todo");                                                           \
    return false;                                                              \
  }                                                                            \
  static inline element_type typealias##_pop(typealias ptr) {                  \
    assert(!"todo");                                                           \
    return (element_type){0};                                                  \
  }                                                                            \
  static inline bool typealias##_memmove(typealias *ptr, size_t offset,        \
                                         const void *source, size_t n) {       \
    assert(!"todo");                                                           \
    return false;                                                              \
  }
