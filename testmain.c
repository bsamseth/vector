#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

VECTOR_IMPL(int, intvec)

int main() {
  intvec vec = intvec_init(0);
  assert(intvec_is_empty(vec));
  int data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  intvec_memmove(&vec, 0, data, 10);
  assert(!intvec_is_empty(vec));

  assert(intvec_len(vec) == 10);
  assert(intvec_capacity(vec) == 10);

  intvec_push(&vec, 101);

  assert(intvec_len(vec) == 11);
  assert(intvec_capacity(vec) >= 11);
  assert(vec[10] == 101);

  intvec_push(&vec, 102);
  assert(intvec_len(vec) == 12);
  assert(intvec_capacity(vec) >= 12);
  assert(vec[11] == 102);

  char b = intvec_pop(vec);
  assert(intvec_len(vec) == 11);
  assert(b == 102);

  intvec_memmove(&vec, 7, data, 10);
  assert(intvec_len(vec) == 17);
  intvec_push(&vec, 103);

  for (size_t i = 0; i < intvec_len(vec); ++i) {
    printf("Entry %lu: %d\n", i, vec[i]);
  }

  printf("Capacity: %lu\n", intvec_capacity(vec));

  intvec_free(vec);

  return 0;
}
