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
  static inline bool typealias##_reserve(typealias *ptr, size_t additional) {  \
    return rawvec_reserve((rawvec *)ptr, additional * sizeof(element_type));   \
  }                                                                            \
  static inline bool typealias##_shrink_to_fit(typealias *ptr) {               \
    return rawvec_shrink_to_fit((rawvec *)ptr);                                \
  }                                                                            \
  static inline void typealias##_set_len(typealias ptr, size_t len) {          \
    rawvec_set_len((rawvec)ptr, len * sizeof(element_type));                   \
  }                                                                            \
  static inline bool typealias##_push(typealias *ptr, element_type element) {  \
    char *element_as_bytes = (char *)&element;                                 \
    int changed = 0;                                                           \
    for (size_t i = 0; i < sizeof(element_type); ++i)                          \
      changed += rawvec_push((rawvec *)ptr, element_as_bytes[i]);              \
    return changed > 0;                                                        \
  }                                                                            \
  static inline element_type typealias##_pop(typealias ptr) {                  \
    size_t length = typealias##_len(ptr);                                      \
    assert(length > 0);                                                        \
    element_type value = ptr[length - 1];                                      \
    for (size_t i = 0; i < sizeof(element_type); ++i)                          \
      rawvec_pop((rawvec)ptr);                                                 \
    return value;                                                              \
  }                                                                            \
  static inline bool typealias##_memmove(                                      \
      typealias *ptr, size_t offset, const element_type *source, size_t n) {   \
    return rawvec_memmove((rawvec *)ptr, offset * sizeof(element_type),        \
                          (void *)source, n * sizeof(element_type));           \
  }                                                                            \
  static inline bool typealias##_extend(                                       \
      typealias *ptr, const element_type *source, size_t n) {                  \
    return rawvec_extend((rawvec *)ptr, (void *)source,                        \
                         n * sizeof(element_type));                            \
  }
