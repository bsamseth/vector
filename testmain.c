#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

VECTOR_IMPL(int, intvec)

int main() {
  intvec vec = intvec_init(0);
  assert(rawvec_is_empty(vec));
  int data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
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
