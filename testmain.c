#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "unity.h"

#include "vector.h"

typedef struct {
  char first;
  size_t second;
  bool last;
} padded_t;

VECTOR_IMPL(int, intvec)
VECTOR_IMPL(padded_t, paddedvec)

void test_intvec_demo_should_work(void) {
  intvec vec = intvec_init(0);
  TEST_ASSERT(intvec_is_empty(vec));
  int data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  intvec_memmove(&vec, 0, data, 10);
  TEST_ASSERT(!intvec_is_empty(vec));

  TEST_ASSERT(intvec_len(vec) == 10);
  TEST_ASSERT(intvec_capacity(vec) == 10);

  intvec_push(&vec, 101);

  TEST_ASSERT(intvec_len(vec) == 11);
  TEST_ASSERT(intvec_capacity(vec) >= 11);
  TEST_ASSERT(vec[10] == 101);

  intvec_push(&vec, 102);
  TEST_ASSERT(intvec_len(vec) == 12);
  TEST_ASSERT(intvec_capacity(vec) >= 12);
  TEST_ASSERT(vec[11] == 102);

  char b = intvec_pop(vec);
  TEST_ASSERT(intvec_len(vec) == 11);
  TEST_ASSERT(b == 102);

  intvec_memmove(&vec, 7, data, 10);
  TEST_ASSERT(intvec_len(vec) == 17);
  intvec_push(&vec, 103);

  intvec_free(vec);
}

void test_paddedvec_demo_should_work(void) {
  paddedvec vec = paddedvec_init(0);
  TEST_ASSERT(paddedvec_is_empty(vec));
  padded_t data[10] = {{.first = 1, .second = 123, .last = true}};
  paddedvec_memmove(&vec, 0, data, 10);
  TEST_ASSERT(!paddedvec_is_empty(vec));

  TEST_ASSERT(paddedvec_len(vec) == 10);
  TEST_ASSERT(paddedvec_capacity(vec) == 10);

  padded_t add = {.first = 9, .second = 31, .last = true};
  paddedvec_push(&vec, add);

  TEST_ASSERT(paddedvec_len(vec) == 11);
  TEST_ASSERT(paddedvec_capacity(vec) >= 11);
  TEST_ASSERT(vec[10].first == add.first && vec[10].second == add.second &&
              vec[10].last == add.last);

  paddedvec_push(&vec, add);
  TEST_ASSERT(paddedvec_len(vec) == 12);
  TEST_ASSERT(paddedvec_capacity(vec) >= 12);
  TEST_ASSERT(vec[11].first == add.first && vec[11].second == add.second &&
              vec[11].last == add.last);

  padded_t b = paddedvec_pop(vec);
  TEST_ASSERT(paddedvec_len(vec) == 11);
  TEST_ASSERT(b.first == add.first && b.second == add.second &&
              b.last == add.last);

  paddedvec_memmove(&vec, 7, data, 10);
  TEST_ASSERT(paddedvec_len(vec) == 17);
  paddedvec_push(&vec, add);

  paddedvec_free(vec);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_intvec_demo_should_work);
  RUN_TEST(test_paddedvec_demo_should_work);
  return UNITY_END();
}
void setUp(void) {}
void tearDown(void) {}
