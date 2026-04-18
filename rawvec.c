#include "rawvec.h"

#include <assert.h>
#include <stdbool.h>
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

bool rawvec_memmove(rawvec *ptr, size_t offset, const void *source, size_t n) {
  __rawvec_t *vec = __rawvec_from_user_ptr(*ptr);
  assert(offset <= vec->count);
  size_t n_added = n - (vec->count - offset);
  bool changed = rawvec_reserve(ptr, n_added);

  // If the allocation moved due to the resize, then it cannot overlap with
  // source, so it is safe to do memcpy in place of memmove.
  if (changed) {
    memcpy((*ptr) + offset, source, n);
    vec = __rawvec_from_user_ptr(*ptr);
  } else
    memmove((*ptr) + offset, source, n);

  vec->count += n_added;
  return changed;
}

#ifdef RAWVEC_TEST_MAIN
#include <stdio.h>
int main() {
  char *vec = rawvec_init(0);
  assert(rawvec_is_empty(vec));
  char data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  rawvec_memmove(&vec, 0, data, 10);
  assert(!rawvec_is_empty(vec));

  assert(rawvec_len(vec) == 10);
  assert(rawvec_capacity(vec) == 10);

  rawvec_push(&vec, 101);

  assert(rawvec_len(vec) == 11);
  assert(rawvec_capacity(vec) >= 11);
  assert(vec[10] == 101);

  rawvec_push(&vec, 102);
  assert(rawvec_len(vec) == 12);
  assert(rawvec_capacity(vec) >= 12);
  assert(vec[11] == 102);

  char b = rawvec_pop(vec);
  assert(rawvec_len(vec) == 11);
  assert(b == 102);

  rawvec_memmove(&vec, 7, data, 10);
  assert(rawvec_len(vec) == 17);
  rawvec_push(&vec, 103);

  for (size_t i = 0; i < rawvec_len(vec); ++i) {
    printf("Entry %lu: %d\n", i, vec[i]);
  }

  printf("Capacity: %lu\n", rawvec_capacity(vec));

  rawvec_free(vec);

  return 0;
}
#endif
