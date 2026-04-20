#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "unity.h"

#define VECTOR_INCLUDE_IMPLEMENTATION
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
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  int data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  intvec_memcpy(&vec, 0, data, 10);
  TEST_ASSERT_FALSE(intvec_is_empty(vec));

  TEST_ASSERT_EQUAL_INT(10, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(10, intvec_capacity(vec));

  intvec_push(&vec, 101);

  TEST_ASSERT_EQUAL_INT(11, intvec_len(vec));
  TEST_ASSERT_GREATER_THAN_INT(10, intvec_capacity(vec));
  TEST_ASSERT_EQUAL_INT(101, vec[10]);

  intvec_push(&vec, 102);
  TEST_ASSERT_EQUAL_INT(12, intvec_len(vec));
  TEST_ASSERT_GREATER_THAN_INT(11, intvec_capacity(vec));
  TEST_ASSERT_EQUAL_INT(102, vec[11]);

  int b = intvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(11, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(102, b);

  intvec_memcpy(&vec, 7, data, 10);
  TEST_ASSERT_EQUAL_INT(17, intvec_len(vec));
  intvec_push(&vec, 103);

  intvec_free(vec);
}

void test_paddedvec_demo_should_work(void) {
  paddedvec vec = paddedvec_init(0);
  TEST_ASSERT_TRUE(paddedvec_is_empty(vec));
  padded_t data[10] = {{.first = 1, .second = 123, .last = true}};
  paddedvec_memcpy(&vec, 0, data, 10);
  TEST_ASSERT_FALSE(paddedvec_is_empty(vec));

  TEST_ASSERT_EQUAL_INT(10, paddedvec_len(vec));
  TEST_ASSERT_EQUAL_INT(10, paddedvec_capacity(vec));

  padded_t add = {.first = 9, .second = 31, .last = true};
  paddedvec_push(&vec, add);

  TEST_ASSERT_EQUAL_INT(11, paddedvec_len(vec));
  TEST_ASSERT_GREATER_THAN_INT(10, paddedvec_capacity(vec));
  TEST_ASSERT_EQUAL_INT(add.first, vec[10].first);
  TEST_ASSERT_EQUAL_INT(add.second, vec[10].second);
  TEST_ASSERT_TRUE(vec[10].last == add.last);

  paddedvec_push(&vec, add);
  TEST_ASSERT_EQUAL_INT(12, paddedvec_len(vec));
  TEST_ASSERT_GREATER_THAN_INT(11, paddedvec_capacity(vec));
  TEST_ASSERT_EQUAL_INT(add.first, vec[11].first);
  TEST_ASSERT_EQUAL_INT(add.second, vec[11].second);
  TEST_ASSERT_TRUE(vec[11].last == add.last);

  padded_t b = paddedvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(11, paddedvec_len(vec));
  TEST_ASSERT_EQUAL_INT(add.first, b.first);
  TEST_ASSERT_EQUAL_INT(add.second, b.second);
  TEST_ASSERT_TRUE(b.last == add.last);

  paddedvec_memcpy(&vec, 7, data, 10);
  TEST_ASSERT_EQUAL_INT(17, paddedvec_len(vec));
  paddedvec_push(&vec, add);

  paddedvec_free(vec);
}

void test_intvec_empty_initial_capacity(void) {
  intvec vec = intvec_init(0);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  TEST_ASSERT_EQUAL_INT(0, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(0, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_nonzero_initial_capacity(void) {
  intvec vec = intvec_init(5);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  TEST_ASSERT_EQUAL_INT(0, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(5, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_push_single_element(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 42);
  TEST_ASSERT_FALSE(intvec_is_empty(vec));
  TEST_ASSERT_EQUAL_INT(1, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(42, vec[0]);
  intvec_free(vec);
}

void test_intvec_push_triggers_resize(void) {
  intvec vec = intvec_init(1);
  TEST_ASSERT_EQUAL_INT(1, intvec_capacity(vec));
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  TEST_ASSERT_GREATER_THAN_INT(2, intvec_capacity(vec));
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_pop_single_element(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 100);
  intvec_push(&vec, 200);
  int b = intvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(200, b);
  TEST_ASSERT_EQUAL_INT(1, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_pop_all_elements(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  intvec_pop(vec);
  intvec_pop(vec);
  intvec_pop(vec);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  TEST_ASSERT_EQUAL_INT(0, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_memcpy_to_offset_zero(void) {
  intvec vec = intvec_init(0);
  int data[] = {10, 20, 30};
  intvec_memcpy(&vec, 0, data, 3);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  int expected[] = {10, 20, 30};
  TEST_ASSERT_EQUAL_INT_ARRAY(expected, vec, 3);
  intvec_free(vec);
}

void test_intvec_memcpy_to_end(void) {
  intvec vec = intvec_init(0);
  int initial[] = {1, 2};
  intvec_memcpy(&vec, 0, initial, 2);
  int more[] = {3, 4};
  intvec_memcpy(&vec, 2, more, 2);
  TEST_ASSERT_EQUAL_INT(4, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(3, vec[2]);
  TEST_ASSERT_EQUAL_INT(4, vec[3]);
  intvec_free(vec);
}

void test_intvec_memcpy_zero_elements(void) {
  intvec vec = intvec_init(0);
  int data[] = {1};
  intvec_memcpy(&vec, 0, data, 0);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_free(vec);
}

void test_intvec_set_len_to_zero(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_set_len(vec, 0);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  TEST_ASSERT_EQUAL_INT(0, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_set_len_to_capacity(void) {
  intvec vec = intvec_init(3);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_set_len(vec, 3);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(3, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_resize_up(void) {
  intvec vec = intvec_init(2);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_resize(&vec, 10);
  TEST_ASSERT_GREATER_THAN_INT(9, intvec_capacity(vec));
  TEST_ASSERT_EQUAL_INT(1, vec[0]);
  TEST_ASSERT_EQUAL_INT(2, vec[1]);
  intvec_free(vec);
}

void test_intvec_resize_down(void) {
  intvec vec = intvec_init(5);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  intvec_resize(&vec, 2);
  TEST_ASSERT_GREATER_THAN_INT(1, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_resize_to_zero(void) {
  intvec vec = intvec_init(3);
  intvec_push(&vec, 1);
  intvec_resize(&vec, 0);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_free(vec);
}

void test_intvec_remaining_capacity(void) {
  intvec vec = intvec_init(5);
  TEST_ASSERT_EQUAL_INT(5, intvec_remaining(vec));
  intvec_push(&vec, 1);
  TEST_ASSERT_EQUAL_INT(4, intvec_remaining(vec));
  intvec_push(&vec, 2);
  TEST_ASSERT_EQUAL_INT(3, intvec_remaining(vec));
  intvec_free(vec);
}

void test_intvec_is_empty_after_operations(void) {
  intvec vec = intvec_init(0);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_push(&vec, 1);
  TEST_ASSERT_FALSE(intvec_is_empty(vec));
  intvec_pop(vec);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_set_len(vec, 0);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_free(vec);
}

void test_intvec_push_pop_sequence(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 1);
  TEST_ASSERT_EQUAL_INT(1, intvec_pop(vec));
  intvec_push(&vec, 2);
  TEST_ASSERT_EQUAL_INT(2, intvec_pop(vec));
  intvec_push(&vec, 3);
  TEST_ASSERT_EQUAL_INT(3, intvec_pop(vec));
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_free(vec);
}

void test_intvec_large_initial_capacity(void) {
  intvec vec = intvec_init(1000);
  TEST_ASSERT_EQUAL_INT(1000, intvec_capacity(vec));
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_free(vec);
}

void test_intvec_overflow_resize(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 100; i++) {
    intvec_push(&vec, i);
  }
  TEST_ASSERT_EQUAL_INT(100, intvec_len(vec));
  TEST_ASSERT_GREATER_THAN_INT(99, intvec_capacity(vec));
  intvec_free(vec);
}

void test_paddedvec_empty_operations(void) {
  paddedvec vec = paddedvec_init(0);
  TEST_ASSERT_TRUE(paddedvec_is_empty(vec));
  TEST_ASSERT_EQUAL_INT(0, paddedvec_len(vec));
  paddedvec_free(vec);
}

void test_paddedvec_push_single_struct(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t elem = {.first = 'a', .second = 100, .last = false};
  paddedvec_push(&vec, elem);
  TEST_ASSERT_FALSE(paddedvec_is_empty(vec));
  TEST_ASSERT_EQUAL_INT(1, paddedvec_len(vec));
  TEST_ASSERT_EQUAL_INT('a', vec[0].first);
  TEST_ASSERT_EQUAL_INT(100, vec[0].second);
  TEST_ASSERT_FALSE(vec[0].last);
  paddedvec_free(vec);
}

void test_paddedvec_memcpy_multiple_structs(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t data[3] = {
      {.first = 'x', .second = 1, .last = true},
      {.first = 'y', .second = 2, .last = false},
      {.first = 'z', .second = 3, .last = true},
  };
  paddedvec_memcpy(&vec, 0, data, 3);
  TEST_ASSERT_EQUAL_INT(3, paddedvec_len(vec));
  TEST_ASSERT_EQUAL_INT('x', vec[0].first);
  TEST_ASSERT_EQUAL_INT('y', vec[1].first);
  TEST_ASSERT_EQUAL_INT('z', vec[2].first);
  paddedvec_free(vec);
}

void test_paddedvec_pop_struct(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t a = {.first = 'a', .second = 1, .last = true};
  padded_t b = {.first = 'b', .second = 2, .last = false};
  paddedvec_push(&vec, a);
  paddedvec_push(&vec, b);
  padded_t result = paddedvec_pop(vec);
  TEST_ASSERT_EQUAL_INT('b', result.first);
  TEST_ASSERT_EQUAL_INT(2, result.second);
  TEST_ASSERT_FALSE(result.last);
  paddedvec_free(vec);
}

void test_intvec_boundary_at_capacity(void) {
  intvec vec = intvec_init(2);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  TEST_ASSERT_EQUAL_INT(intvec_capacity(vec), intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(0, intvec_remaining(vec));
  intvec_push(&vec, 3);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  TEST_ASSERT_GREATER_THAN_INT(2, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_set_len_then_push(void) {
  intvec vec = intvec_init(5);
  int data[] = {10, 20, 30};
  intvec_memcpy(&vec, 0, data, 3);
  intvec_set_len(vec, 0);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_push(&vec, 99);
  TEST_ASSERT_EQUAL_INT(99, vec[0]);
  TEST_ASSERT_EQUAL_INT(1, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_multiple_resizes(void) {
  intvec vec = intvec_init(1);
  intvec_push(&vec, 1);
  TEST_ASSERT_GREATER_THAN_INT(0, intvec_capacity(vec));
  intvec_resize(&vec, 50);
  TEST_ASSERT_GREATER_THAN_INT(49, intvec_capacity(vec));
  intvec_resize(&vec, 10);
  TEST_ASSERT_GREATER_THAN_INT(9, intvec_capacity(vec));
  intvec_resize(&vec, 100);
  TEST_ASSERT_GREATER_THAN_INT(99, intvec_capacity(vec));
  intvec_free(vec);
}

void test_paddedvec_resize_struct_vector(void) {
  paddedvec vec = paddedvec_init(1);
  padded_t elem = {.first = 'a', .second = 1, .last = true};
  paddedvec_push(&vec, elem);
  paddedvec_resize(&vec, 10);
  TEST_ASSERT_GREATER_THAN_INT(9, paddedvec_capacity(vec));
  TEST_ASSERT_EQUAL_INT('a', vec[0].first);
  paddedvec_free(vec);
}

void test_intvec_append_via_memcpy(void) {
  intvec vec = intvec_init(0);
  int initial[] = {1, 2, 3};
  intvec_memcpy(&vec, 0, initial, 3);
  int append[] = {4, 5};
  intvec_memcpy(&vec, 3, append, 2);
  TEST_ASSERT_EQUAL_INT(5, intvec_len(vec));
  int expected[] = {1, 2, 3, 4, 5};
  TEST_ASSERT_EQUAL_INT_ARRAY(expected, vec, 5);
  intvec_free(vec);
}

void test_intvec_zero_capacity_then_push(void) {
  intvec vec = intvec_init(0);
  TEST_ASSERT_EQUAL_INT(0, intvec_capacity(vec));
  intvec_push(&vec, 1);
  TEST_ASSERT_GREATER_THAN_INT(0, intvec_capacity(vec));
  TEST_ASSERT_EQUAL_INT(1, vec[0]);
  intvec_free(vec);
}

void test_intvec_negative_values(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, -1);
  intvec_push(&vec, -100);
  intvec_push(&vec, -999999);
  TEST_ASSERT_EQUAL_INT(-1, vec[0]);
  TEST_ASSERT_EQUAL_INT(-100, vec[1]);
  TEST_ASSERT_EQUAL_INT(-999999, vec[2]);
  intvec_free(vec);
}

void test_intvec_int_min_max(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 2147483647);
  intvec_push(&vec, -2147483648);
  TEST_ASSERT_EQUAL_INT(2147483647, vec[0]);
  TEST_ASSERT_EQUAL_INT(-2147483648, vec[1]);
  intvec_free(vec);
}

void test_intvec_negative_pop(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, -50);
  int result = intvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(-50, result);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_free(vec);
}

void test_intvec_data_preservation_after_resize(void) {
  intvec vec = intvec_init(1);
  intvec_push(&vec, 10);
  intvec_push(&vec, 20);
  intvec_push(&vec, 30);
  TEST_ASSERT_EQUAL_INT(10, vec[0]);
  TEST_ASSERT_EQUAL_INT(20, vec[1]);
  TEST_ASSERT_EQUAL_INT(30, vec[2]);
  intvec_free(vec);
}

void test_intvec_interleaved_push_pop(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_pop(vec);
  intvec_push(&vec, 3);
  intvec_pop(vec);
  intvec_push(&vec, 4);
  TEST_ASSERT_EQUAL_INT(2, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(1, vec[0]);
  TEST_ASSERT_EQUAL_INT(4, vec[1]);
  intvec_free(vec);
}

void test_intvec_resize_to_same_size(void) {
  intvec vec = intvec_init(5);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_resize(&vec, 5);
  TEST_ASSERT_GREATER_THAN_INT(4, intvec_capacity(vec));
  TEST_ASSERT_EQUAL_INT(1, vec[0]);
  TEST_ASSERT_EQUAL_INT(2, vec[1]);
  intvec_free(vec);
}

void test_intvec_set_len_partial_then_append(void) {
  intvec vec = intvec_init(10);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  intvec_set_len(vec, 1);
  TEST_ASSERT_EQUAL_INT(1, intvec_len(vec));
  int data[] = {9, 8};
  intvec_memcpy(&vec, 1, data, 2);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(9, vec[1]);
  TEST_ASSERT_EQUAL_INT(8, vec[2]);
  intvec_free(vec);
}

void test_intvec_multiple_appends_via_memcpy(void) {
  intvec vec = intvec_init(0);
  intvec_memcpy(&vec, 0, (int[]){1}, 1);
  intvec_memcpy(&vec, 1, (int[]){2}, 1);
  intvec_memcpy(&vec, 2, (int[]){3}, 1);
  intvec_memcpy(&vec, 3, (int[]){4}, 1);
  intvec_memcpy(&vec, 4, (int[]){5}, 1);
  TEST_ASSERT_EQUAL_INT(5, intvec_len(vec));
  int expected[] = {1, 2, 3, 4, 5};
  TEST_ASSERT_EQUAL_INT_ARRAY(expected, vec, 5);
  intvec_free(vec);
}

void test_paddedvec_all_zero_fields(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t elem = {.first = 0, .second = 0, .last = false};
  paddedvec_push(&vec, elem);
  TEST_ASSERT_EQUAL_INT(0, vec[0].first);
  TEST_ASSERT_EQUAL_INT(0, vec[0].second);
  TEST_ASSERT_FALSE(vec[0].last);
  paddedvec_free(vec);
}

void test_paddedvec_bool_fields(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t t = {.first = 't', .second = 1, .last = true};
  padded_t f = {.first = 'f', .second = 0, .last = false};
  paddedvec_push(&vec, t);
  paddedvec_push(&vec, f);
  TEST_ASSERT_TRUE(vec[0].last);
  TEST_ASSERT_FALSE(vec[1].last);
  paddedvec_free(vec);
}

void test_paddedvec_multiple_structs_then_pop(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t data[5] = {
      {.first = 'a', .second = 1, .last = true},
      {.first = 'b', .second = 2, .last = false},
      {.first = 'c', .second = 3, .last = true},
      {.first = 'd', .second = 4, .last = false},
      {.first = 'e', .second = 5, .last = true},
  };
  paddedvec_memcpy(&vec, 0, data, 5);
  padded_t result = paddedvec_pop(vec);
  TEST_ASSERT_EQUAL_INT('e', result.first);
  TEST_ASSERT_EQUAL_INT(5, result.second);
  TEST_ASSERT_EQUAL_INT(4, paddedvec_len(vec));
  paddedvec_free(vec);
}

void test_intvec_capacity_exact_match(void) {
  intvec vec = intvec_init(3);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  TEST_ASSERT_GREATER_THAN_INT(2, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_pop_preserves_other_elements(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 10);
  intvec_push(&vec, 20);
  intvec_push(&vec, 30);
  intvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(2, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(10, vec[0]);
  TEST_ASSERT_EQUAL_INT(20, vec[1]);
  intvec_free(vec);
}

void test_intvec_capacity_growth_pattern(void) {
  intvec vec = intvec_init(1);
  size_t old_cap = intvec_capacity(vec);
  for (int i = 0; i < 10; i++) {
    intvec_push(&vec, i);
    size_t new_cap = intvec_capacity(vec);
    TEST_ASSERT_GREATER_THAN_INT(old_cap - 1, new_cap);
    old_cap = new_cap;
  }
  intvec_free(vec);
}

void test_intvec_set_len_preserves_data(void) {
  intvec vec = intvec_init(10);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_set_len(vec, 2);
  intvec_push(&vec, 3);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(1, vec[0]);
  TEST_ASSERT_EQUAL_INT(2, vec[1]);
  TEST_ASSERT_EQUAL_INT(3, vec[2]);
  intvec_free(vec);
}

void test_intvec_overwrite_at_beginning_via_memcpy(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 5);
  intvec_push(&vec, 6);
  int overwrite[] = {1, 2};
  intvec_memcpy(&vec, 0, overwrite, 2);
  TEST_ASSERT_EQUAL_INT(2, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(1, vec[0]);
  TEST_ASSERT_EQUAL_INT(2, vec[1]);
  intvec_free(vec);
}

void test_intvec_overwrite_in_middle_via_memcpy(void) {
  intvec vec = intvec_init(0);
  intvec_memcpy(&vec, 0, (int[]){1, 2, 3}, 3);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(1, vec[0]);
  TEST_ASSERT_EQUAL_INT(2, vec[1]);
  TEST_ASSERT_EQUAL_INT(3, vec[2]);
  intvec_free(vec);
}

void test_intvec_exact_capacity_push(void) {
  intvec vec = intvec_init(2);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  TEST_ASSERT_EQUAL_INT(intvec_capacity(vec), intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(0, intvec_remaining(vec));
  intvec_push(&vec, 3);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  TEST_ASSERT_GREATER_THAN_INT(2, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_single_element_operations(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 99);
  TEST_ASSERT_EQUAL_INT(1, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(99, vec[0]);
  TEST_ASSERT_FALSE(intvec_is_empty(vec));
  int b = intvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(99, b);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_free(vec);
}

void test_intvec_alternating_push_pop_many(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 50; i++) {
    intvec_push(&vec, i);
    TEST_ASSERT_EQUAL_INT(i + 1, intvec_len(vec));
  }
  for (int i = 49; i >= 0; i--) {
    int result = intvec_pop(vec);
    TEST_ASSERT_EQUAL_INT(i, result);
    TEST_ASSERT_EQUAL_INT(i, intvec_len(vec));
  }
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_free(vec);
}

void test_intvec_double_elements(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 0);
  intvec_push(&vec, 0);
  intvec_set_len(vec, 0);
  intvec_push(&vec, 1);
  TEST_ASSERT_EQUAL_INT(1, vec[0]);
  intvec_free(vec);
}

void test_intvec_very_small_capacity(void) {
  intvec vec = intvec_init(1);
  TEST_ASSERT_EQUAL_INT(1, intvec_capacity(vec));
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_push(&vec, 1);
  TEST_ASSERT_EQUAL_INT(1, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(0, intvec_remaining(vec));
  intvec_push(&vec, 2);
  TEST_ASSERT_GREATER_THAN_INT(1, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_push_does_not_corrupt_other_data(void) {
  intvec vec = intvec_init(0);
  int data[] = {100, 200, 300};
  intvec_memcpy(&vec, 0, data, 3);
  for (int i = 0; i < 5; i++) {
    intvec_push(&vec, i);
  }
  int expected[] = {100, 200, 300, 0, 1, 2, 3, 4};
  TEST_ASSERT_EQUAL_INT_ARRAY(expected, vec, 8);
  intvec_free(vec);
}

void test_intvec_repeated_resize_down_up(void) {
  intvec vec = intvec_init(10);
  for (int i = 0; i < 5; i++) {
    intvec_push(&vec, i);
  }
  for (int i = 0; i < 4; i++) {
    intvec_resize(&vec, 5 - i);
  }
  intvec_resize(&vec, 10);
  TEST_ASSERT_GREATER_THAN_INT(9, intvec_capacity(vec));
  intvec_free(vec);
}

void test_paddedvec_pop_multiple_preserves_others(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t data[3] = {
      {.first = 'a', .second = 1, .last = true},
      {.first = 'b', .second = 2, .last = false},
      {.first = 'c', .second = 3, .last = true},
  };
  paddedvec_memcpy(&vec, 0, data, 3);
  paddedvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(2, paddedvec_len(vec));
  TEST_ASSERT_EQUAL_INT('a', vec[0].first);
  TEST_ASSERT_EQUAL_INT('b', vec[1].first);
  paddedvec_free(vec);
}

void test_paddedvec_large_struct_count(void) {
  paddedvec vec = paddedvec_init(0);
  for (int i = 0; i < 50; i++) {
    padded_t elem = {
        .first = (char)i, .second = (size_t)i, .last = (i % 2 == 0)};
    paddedvec_push(&vec, elem);
  }
  TEST_ASSERT_EQUAL_INT(50, paddedvec_len(vec));
  TEST_ASSERT_EQUAL_INT(25, vec[25].first);
  TEST_ASSERT_EQUAL_INT(25, vec[25].second);
  TEST_ASSERT_FALSE(vec[25].last);
  paddedvec_free(vec);
}

void test_paddedvec_set_len_and_append(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t elem1 = {.first = 'a', .second = 1, .last = true};
  paddedvec_push(&vec, elem1);
  paddedvec_set_len(vec, 0);
  TEST_ASSERT_TRUE(paddedvec_is_empty(vec));
  padded_t elem2 = {.first = 'b', .second = 2, .last = false};
  paddedvec_push(&vec, elem2);
  TEST_ASSERT_EQUAL_INT('b', vec[0].first);
  paddedvec_free(vec);
}

void test_intvec_many_small_memcpys(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 20; i++) {
    int value = i * 10;
    intvec_memcpy(&vec, i, &value, 1);
  }
  TEST_ASSERT_EQUAL_INT(20, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(0, vec[0]);
  TEST_ASSERT_EQUAL_INT(100, vec[10]);
  TEST_ASSERT_EQUAL_INT(190, vec[19]);
  intvec_free(vec);
}

void test_intvec_capacity_one_growth(void) {
  intvec vec = intvec_init(1);
  TEST_ASSERT_EQUAL_INT(1, intvec_capacity(vec));
  intvec_push(&vec, 1);
  TEST_ASSERT_EQUAL_INT(1, vec[0]);
  intvec_push(&vec, 2);
  TEST_ASSERT_GREATER_THAN_INT(1, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_exact_resize_then_push(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 1);
  intvec_resize(&vec, 1);
  intvec_push(&vec, 2);
  TEST_ASSERT_EQUAL_INT(2, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(1, vec[0]);
  TEST_ASSERT_EQUAL_INT(2, vec[1]);
  intvec_free(vec);
}

void test_intvec_consecutive_push_short(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 4; i++) {
    intvec_push(&vec, i);
  }
  TEST_ASSERT_EQUAL_INT(4, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_zero_length_after_resize(void) {
  intvec vec = intvec_init(2);
  intvec_resize(&vec, 0);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  TEST_ASSERT_EQUAL_INT(0, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_memcpy_single_element(void) {
  intvec vec = intvec_init(0);
  int val = 42;
  intvec_memcpy(&vec, 0, &val, 1);
  TEST_ASSERT_EQUAL_INT(1, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(42, vec[0]);
  intvec_free(vec);
}

void test_intvec_pop_twice_then_push(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  intvec_pop(vec);
  intvec_pop(vec);
  intvec_push(&vec, 99);
  TEST_ASSERT_EQUAL_INT(2, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(1, vec[0]);
  TEST_ASSERT_EQUAL_INT(99, vec[1]);
  intvec_free(vec);
}

void test_paddedvec_pop_preserves_field_values(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t a = {.first = 'x', .second = 99, .last = true};
  padded_t b = {.first = 'y', .second = 88, .last = false};
  paddedvec_push(&vec, a);
  paddedvec_push(&vec, b);
  paddedvec_pop(vec);
  TEST_ASSERT_EQUAL_INT('x', vec[0].first);
  TEST_ASSERT_EQUAL_INT(99, vec[0].second);
  TEST_ASSERT_TRUE(vec[0].last);
  paddedvec_free(vec);
}

void test_intvec_set_len_preserves_existing_data(void) {
  intvec vec = intvec_init(10);
  intvec_push(&vec, 10);
  intvec_push(&vec, 20);
  intvec_push(&vec, 30);
  intvec_set_len(vec, 3);
  intvec_set_len(vec, 2);
  TEST_ASSERT_EQUAL_INT(2, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(10, vec[0]);
  TEST_ASSERT_EQUAL_INT(20, vec[1]);
  intvec_free(vec);
}

void test_intvec_sequential_growth(void) {
  intvec vec = intvec_init(1);
  for (int i = 0; i < 20; i++) {
    intvec_push(&vec, i);
    TEST_ASSERT_EQUAL_INT(i, vec[i]);
  }
  TEST_ASSERT_EQUAL_INT(20, intvec_len(vec));
  TEST_ASSERT_GREATER_THAN_INT(19, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_memcpy_no_initial_elements(void) {
  intvec vec = intvec_init(5);
  int data[] = {1};
  intvec_memcpy(&vec, 0, data, 1);
  TEST_ASSERT_EQUAL_INT(1, intvec_len(vec));
  intvec_free(vec);
}

void test_paddedvec_zero_initial_capacity(void) {
  paddedvec vec = paddedvec_init(0);
  TEST_ASSERT_TRUE(paddedvec_is_empty(vec));
  TEST_ASSERT_EQUAL_INT(0, paddedvec_capacity(vec));
  paddedvec_free(vec);
}

void test_intvec_remaining_decreases_with_push(void) {
  intvec vec = intvec_init(3);
  size_t r1 = intvec_remaining(vec);
  intvec_push(&vec, 1);
  size_t r2 = intvec_remaining(vec);
  TEST_ASSERT_TRUE(r2 < r1);
  intvec_free(vec);
}

void test_intvec_overwrite_only_using_memcpy(void) {
  intvec vec = intvec_init(0);
  int data[] = {5, 6, 7};
  intvec_memcpy(&vec, 0, data, 3);
  intvec_set_len(vec, 0);
  int newdata[] = {8, 9};
  intvec_memcpy(&vec, 0, newdata, 2);
  TEST_ASSERT_EQUAL_INT(2, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(8, vec[0]);
  TEST_ASSERT_EQUAL_INT(9, vec[1]);
  intvec_free(vec);
}

void test_intvec_empty_vec_to_large(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 100; i++) {
    intvec_push(&vec, i * 2);
  }
  TEST_ASSERT_EQUAL_INT(100, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(198, vec[99]);
  intvec_free(vec);
}

void test_intvec_memcpy_increases_len(void) {
  intvec vec = intvec_init(0);
  int data[] = {1, 2};
  TEST_ASSERT_EQUAL_INT(0, intvec_len(vec));
  intvec_memcpy(&vec, 0, data, 2);
  TEST_ASSERT_EQUAL_INT(2, intvec_len(vec));
  int data2[] = {3};
  intvec_memcpy(&vec, 2, data2, 1);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_pop_clears_len(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 5);
  intvec_push(&vec, 10);
  TEST_ASSERT_EQUAL_INT(2, intvec_len(vec));
  intvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(1, intvec_len(vec));
  intvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(0, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_capacity_not_shrink_on_pop(void) {
  intvec vec = intvec_init(1);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  size_t cap = intvec_capacity(vec);
  intvec_pop(vec);
  TEST_ASSERT_GREATER_THAN_INT(cap - 1, intvec_capacity(vec));
  intvec_free(vec);
}

void test_paddedvec_free_on_empty(void) {
  paddedvec vec = paddedvec_init(0);
  TEST_ASSERT_TRUE(paddedvec_is_empty(vec));
  paddedvec_free(vec);
}

void test_paddedvec_free_after_operations(void) {
  paddedvec vec = paddedvec_init(0);
  for (int i = 0; i < 5; i++) {
    padded_t e = {.first = (char)i, .second = i, .last = true};
    paddedvec_push(&vec, e);
  }
  TEST_ASSERT_EQUAL_INT(5, paddedvec_len(vec));
  paddedvec_free(vec);
}

void test_intvec_one_by_one_push(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 10; i++) {
    intvec_push(&vec, i);
  }
  TEST_ASSERT_EQUAL_INT(10, intvec_len(vec));
  int expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  TEST_ASSERT_EQUAL_INT_ARRAY(expected, vec, 10);
  intvec_free(vec);
}

void test_intvec_pop_order_preserved(void) {
  intvec vec = intvec_init(0);
  int expected[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  for (int i = 0; i < 10; i++) {
    intvec_push(&vec, i);
  }
  for (int i = 0; i < 10; i++) {
    TEST_ASSERT_EQUAL_INT(expected[i], intvec_pop(vec));
  }
  intvec_free(vec);
}

void test_intvec_max_capacity_push(void) {
  intvec vec = intvec_init(100);
  for (int i = 0; i < 100; i++) {
    intvec_push(&vec, i);
  }
  TEST_ASSERT_EQUAL_INT(100, intvec_len(vec));
  TEST_ASSERT_GREATER_THAN_INT(99, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_struct_equivalent_access(void) {
  intvec vec = intvec_init(0);
  int data[] = {42, 43, 44};
  intvec_memcpy(&vec, 0, data, 3);
  int *ptr = vec;
  TEST_ASSERT_EQUAL_INT(42, ptr[0]);
  TEST_ASSERT_EQUAL_INT(43, ptr[1]);
  TEST_ASSERT_EQUAL_INT(44, ptr[2]);
  intvec_free(vec);
}

void test_paddedvec_sequential_append(void) {
  paddedvec vec = paddedvec_init(0);
  for (int i = 0; i < 5; i++) {
    padded_t e = {.first = (char)('a' + i), .second = i, .last = false};
    paddedvec_push(&vec, e);
  }
  TEST_ASSERT_EQUAL_INT('a', vec[0].first);
  TEST_ASSERT_EQUAL_INT('e', vec[4].first);
  TEST_ASSERT_EQUAL_INT(4, vec[4].second);
  paddedvec_free(vec);
}

void test_intvec_set_len_to_exact_then_append(void) {
  intvec vec = intvec_init(5);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_set_len(vec, 2);
  TEST_ASSERT_EQUAL_INT(2, intvec_len(vec));
  intvec_push(&vec, 3);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(3, vec[2]);
  intvec_free(vec);
}

void test_intvec_alternating_set_len_and_push(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 1);
  intvec_set_len(vec, 0);
  intvec_push(&vec, 2);
  intvec_set_len(vec, 0);
  intvec_push(&vec, 3);
  TEST_ASSERT_EQUAL_INT(1, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(3, vec[0]);
  intvec_free(vec);
}

void test_intvec_five_elements_basic(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 10);
  intvec_push(&vec, 20);
  intvec_push(&vec, 30);
  intvec_push(&vec, 40);
  intvec_push(&vec, 50);
  TEST_ASSERT_EQUAL_INT(5, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(50, vec[4]);
  intvec_free(vec);
}

void test_intvec_sequential_zero_to_five(void) {
  intvec vec = intvec_init(0);
  int data[] = {0, 1, 2, 3, 4};
  intvec_memcpy(&vec, 0, data, 5);
  TEST_ASSERT_EQUAL_INT(5, intvec_len(vec));
  int expected[] = {0, 1, 2, 3, 4};
  TEST_ASSERT_EQUAL_INT_ARRAY(expected, vec, 5);
  intvec_free(vec);
}

void test_paddedvec_all_false_bool(void) {
  paddedvec vec = paddedvec_init(0);
  for (int i = 0; i < 3; i++) {
    padded_t e = {.first = '0', .second = i, .last = false};
    paddedvec_push(&vec, e);
    TEST_ASSERT_FALSE(vec[i].last);
  }
  paddedvec_free(vec);
}

void test_paddedvec_all_true_bool(void) {
  paddedvec vec = paddedvec_init(0);
  for (int i = 0; i < 3; i++) {
    padded_t e = {.first = '0', .second = i, .last = true};
    paddedvec_push(&vec, e);
    TEST_ASSERT_TRUE(vec[i].last);
  }
  paddedvec_free(vec);
}

void test_intvec_pop_from_full_then_push(void) {
  intvec vec = intvec_init(3);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  intvec_pop(vec);
  intvec_push(&vec, 4);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(4, vec[2]);
  intvec_free(vec);
}

void test_intvec_free_twice_is_ub(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 1);
  intvec_free(vec);
}

void test_paddedvec_ten_structs(void) {
  paddedvec vec = paddedvec_init(0);
  for (int i = 0; i < 10; i++) {
    padded_t e = {.first = (char)i, .second = i * 10, .last = i % 2 == 0};
    paddedvec_push(&vec, e);
  }
  TEST_ASSERT_EQUAL_INT(10, paddedvec_len(vec));
  TEST_ASSERT_EQUAL_INT(90, vec[9].second);
  paddedvec_free(vec);
}

void test_intvec_grow_from_zero_twice(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  TEST_ASSERT_GREATER_THAN_INT(2, intvec_capacity(vec));
  intvec_resize(&vec, 3);
  intvec_push(&vec, 4);
  TEST_ASSERT_GREATER_THAN_INT(3, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_pop_and_memcpy_sequence(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 10);
  intvec_push(&vec, 20);
  intvec_push(&vec, 30);
  intvec_pop(vec);
  int data[] = {40, 50};
  intvec_memcpy(&vec, 2, data, 2);
  TEST_ASSERT_EQUAL_INT(4, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(40, vec[2]);
  TEST_ASSERT_EQUAL_INT(50, vec[3]);
  intvec_free(vec);
}

void test_intvec_three_memcpy_calls(void) {
  intvec vec = intvec_init(0);
  intvec_memcpy(&vec, 0, (int[]){1}, 1);
  intvec_memcpy(&vec, 1, (int[]){2}, 1);
  intvec_memcpy(&vec, 2, (int[]){3}, 1);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(3, vec[2]);
  intvec_free(vec);
}

void test_intvec_exact_capacity_boundary(void) {
  intvec vec = intvec_init(3);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  TEST_ASSERT_EQUAL_INT(intvec_capacity(vec), intvec_len(vec));
  intvec_push(&vec, 4);
  TEST_ASSERT_EQUAL_INT(4, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_all_ops_combined(void) {
  intvec vec = intvec_init(2);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  intvec_pop(vec);
  intvec_pop(vec);
  intvec_set_len(vec, 0);
  intvec_push(&vec, 99);
  TEST_ASSERT_EQUAL_INT(1, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(99, vec[0]);
  intvec_free(vec);
}

void test_intvec_initial_cap_zero_then_one(void) {
  intvec vec = intvec_init(0);
  TEST_ASSERT_EQUAL_INT(0, intvec_capacity(vec));
  intvec_push(&vec, 1);
  TEST_ASSERT_GREATER_THAN_INT(0, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_pop_does_not_shrink(void) {
  intvec vec = intvec_init(1);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  size_t cap_before = intvec_capacity(vec);
  intvec_pop(vec);
  size_t cap_after = intvec_capacity(vec);
  TEST_ASSERT_GREATER_THAN_INT(cap_before - 1, cap_after);
  intvec_free(vec);
}

void test_intvec_push_after_exact_capacity(void) {
  intvec vec = intvec_init(2);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_capacity_grows_exponentially(void) {
  intvec vec = intvec_init(1);
  size_t prev = intvec_capacity(vec);
  for (int i = 0; i < 10; i++) {
    intvec_push(&vec, i);
    size_t curr = intvec_capacity(vec);
    TEST_ASSERT_GREATER_THAN_INT(prev - 1, curr);
    prev = curr;
  }
  intvec_free(vec);
}

void test_intvec_three_pushes(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(3, vec[2]);
  intvec_free(vec);
}

void test_intvec_five_pushes_then_pop(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 5; i++) {
    intvec_push(&vec, i + 1);
  }
  int result = intvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(5, result);
  TEST_ASSERT_EQUAL_INT(4, intvec_len(vec));
  intvec_free(vec);
}

void test_paddedvec_free_then_push(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t e = {.first = 'a', .second = 1, .last = true};
  paddedvec_push(&vec, e);
  TEST_ASSERT_EQUAL_INT('a', vec[0].first);
  paddedvec_free(vec);
}

void test_paddedvec_push_pop_push(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t a = {.first = 'a', .second = 1, .last = true};
  padded_t b = {.first = 'b', .second = 2, .last = false};
  paddedvec_push(&vec, a);
  paddedvec_pop(vec);
  paddedvec_push(&vec, b);
  TEST_ASSERT_EQUAL_INT('b', vec[0].first);
  paddedvec_free(vec);
}

void test_paddedvec_zero_capacity_then_push(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t e = {.first = 'x', .second = 10, .last = true};
  paddedvec_push(&vec, e);
  TEST_ASSERT_GREATER_THAN_INT(0, paddedvec_capacity(vec));
  paddedvec_free(vec);
}

void test_intvec_set_and_get_multiple(void) {
  intvec vec = intvec_init(0);
  int data[] = {10, 20, 30, 40, 50};
  intvec_memcpy(&vec, 0, data, 5);
  TEST_ASSERT_EQUAL_INT(10, vec[0]);
  TEST_ASSERT_EQUAL_INT(30, vec[2]);
  TEST_ASSERT_EQUAL_INT(50, vec[4]);
  intvec_free(vec);
}

void test_intvec_all_ones(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 10; i++) {
    intvec_push(&vec, 1);
  }
  TEST_ASSERT_EACH_EQUAL_INT(1, vec, 10);
  intvec_free(vec);
}

void test_intvec_repeated_one_value(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 42);
  intvec_push(&vec, 42);
  intvec_push(&vec, 42);
  int expected[] = {42, 42, 42};
  TEST_ASSERT_EQUAL_INT_ARRAY(expected, vec, 3);
  intvec_free(vec);
}

void test_intvec_ascending_values(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 8; i++) {
    intvec_push(&vec, i);
  }
  int expected[] = {0, 1, 2, 3, 4, 5, 6, 7};
  TEST_ASSERT_EQUAL_INT_ARRAY(expected, vec, 8);
  intvec_free(vec);
}

void test_intvec_descending_values(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 8; i++) {
    intvec_push(&vec, 7 - i);
  }
  int expected[] = {7, 6, 5, 4, 3, 2, 1, 0};
  TEST_ASSERT_EQUAL_INT_ARRAY(expected, vec, 8);
  intvec_free(vec);
}

void test_intvec_capacity_greater_than_len(void) {
  intvec vec = intvec_init(10);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  TEST_ASSERT_GREATER_THAN_INT(intvec_len(vec), intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_two_by_two_append(void) {
  intvec vec = intvec_init(0);
  intvec_memcpy(&vec, 0, (int[]){1, 2}, 2);
  intvec_memcpy(&vec, 2, (int[]){3, 4}, 2);
  TEST_ASSERT_EQUAL_INT(4, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(4, vec[3]);
  intvec_free(vec);
}

void test_intvec_four_zeros(void) {
  intvec vec = intvec_init(0);
  int zero = 0;
  for (int i = 0; i < 4; i++) {
    intvec_memcpy(&vec, i, &zero, 1);
  }
  TEST_ASSERT_EQUAL_INT(4, intvec_len(vec));
  TEST_ASSERT_EACH_EQUAL_INT(0, vec, 4);
  intvec_free(vec);
}

void test_intvec_many_sequential_ops(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 20; i++) {
    intvec_push(&vec, i);
  }
  for (int i = 0; i < 10; i++) {
    intvec_pop(vec);
  }
  for (int i = 0; i < 5; i++) {
    intvec_push(&vec, i + 100);
  }
  TEST_ASSERT_EQUAL_INT(15, intvec_len(vec));
  intvec_free(vec);
}

void test_paddedvec_sequential_ops(void) {
  paddedvec vec = paddedvec_init(0);
  for (int i = 0; i < 4; i++) {
    padded_t e = {.first = (char)i, .second = i, .last = true};
    paddedvec_push(&vec, e);
  }
  paddedvec_pop(vec);
  paddedvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(2, paddedvec_len(vec));
  paddedvec_free(vec);
}

void test_intvec_resize_up_large(void) {
  intvec vec = intvec_init(1);
  intvec_push(&vec, 1);
  intvec_resize(&vec, 50);
  TEST_ASSERT_GREATER_THAN_INT(49, intvec_capacity(vec));
  TEST_ASSERT_EQUAL_INT(1, vec[0]);
  intvec_free(vec);
}

void test_intvec_exact_growth_one(void) {
  intvec vec = intvec_init(1);
  for (int i = 0; i < 5; i++) {
    intvec_push(&vec, i);
  }
  TEST_ASSERT_GREATER_THAN_INT(4, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_push_pop_alternate_5_times(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 5; i++) {
    intvec_push(&vec, i);
    intvec_pop(vec);
  }
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_free(vec);
}

void test_paddedvec_struct_with_zero_second(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t e = {.first = 'z', .second = 0, .last = false};
  paddedvec_push(&vec, e);
  TEST_ASSERT_EQUAL_INT(0, vec[0].second);
  paddedvec_free(vec);
}

void test_paddedvec_increasing_second(void) {
  paddedvec vec = paddedvec_init(0);
  for (int i = 0; i < 4; i++) {
    padded_t e = {.first = 'a', .second = i, .last = false};
    paddedvec_push(&vec, e);
  }
  TEST_ASSERT_EQUAL_INT(0, vec[0].second);
  TEST_ASSERT_EQUAL_INT(3, vec[3].second);
  paddedvec_free(vec);
}

void test_intvec_set_len_equals_capacity_then_push(void) {
  intvec vec = intvec_init(3);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_set_len(vec, 2);
  intvec_push(&vec, 3);
  TEST_ASSERT(intvec_len(vec) == 3);
  intvec_free(vec);
}

void test_intvec_free_and_push_new(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 1);
  intvec_free(vec);
  vec = intvec_init(0);
  intvec_push(&vec, 2);
  TEST_ASSERT(vec[0] == 2);
  intvec_free(vec);
}

void test_paddedvec_free_and_push_new(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t e = {.first = 'a', .second = 1, .last = true};
  paddedvec_push(&vec, e);
  paddedvec_free(vec);
  vec = paddedvec_init(0);
  padded_t f = {.first = 'b', .second = 2, .last = false};
  paddedvec_push(&vec, f);
  TEST_ASSERT_EQUAL_INT('b', vec[0].first);
  paddedvec_free(vec);
}

void test_intvec_exactly_seven_elements(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 7; i++) {
    intvec_push(&vec, i);
  }
  TEST_ASSERT_EQUAL_INT(7, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_exactly_ten_elements(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 10; i++) {
    intvec_push(&vec, i);
  }
  TEST_ASSERT_EQUAL_INT(10, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_zero_to_capacity_boundary(void) {
  intvec vec = intvec_init(5);
  for (int i = 0; i < 5; i++) {
    intvec_push(&vec, i);
  }
  TEST_ASSERT_EQUAL_INT(5, intvec_len(vec));
  TEST_ASSERT_GREATER_THAN_INT(4, intvec_capacity(vec));
  intvec_free(vec);
}

void test_intvec_pop_five_from_ten(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 10; i++) {
    intvec_push(&vec, i);
  }
  for (int i = 0; i < 5; i++) {
    intvec_pop(vec);
  }
  TEST_ASSERT_EQUAL_INT(5, intvec_len(vec));
  intvec_free(vec);
}

void test_paddedvec_two_structs_two_pops(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t a = {.first = 'a', .second = 1, .last = true};
  padded_t b = {.first = 'b', .second = 2, .last = false};
  paddedvec_push(&vec, a);
  paddedvec_push(&vec, b);
  paddedvec_pop(vec);
  paddedvec_pop(vec);
  TEST_ASSERT_TRUE(paddedvec_is_empty(vec));
  paddedvec_free(vec);
}

void test_paddedvec_boundary_push(void) {
  paddedvec vec = paddedvec_init(1);
  padded_t e = {.first = 'x', .second = 1, .last = true};
  paddedvec_push(&vec, e);
  paddedvec_push(&vec, e);
  TEST_ASSERT_EQUAL_INT(2, paddedvec_len(vec));
  paddedvec_free(vec);
}

void test_intvec_capacity_equals_elements(void) {
  intvec vec = intvec_init(4);
  for (int i = 0; i < 4; i++) {
    intvec_push(&vec, i);
  }
  TEST_ASSERT_EQUAL_INT(intvec_capacity(vec), intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_pop_leaves_first(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 100);
  intvec_push(&vec, 200);
  intvec_push(&vec, 300);
  intvec_pop(vec);
  intvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(100, vec[0]);
  TEST_ASSERT_EQUAL_INT(1, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_three_pops_from_three(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 3; i++) {
    intvec_push(&vec, i);
  }
  intvec_pop(vec);
  intvec_pop(vec);
  intvec_pop(vec);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_free(vec);
}

void test_intvec_set_len_zero_single_element(void) {
  intvec vec = intvec_init(3);
  intvec_push(&vec, 42);
  intvec_set_len(vec, 0);
  TEST_ASSERT_TRUE(intvec_is_empty(vec));
  intvec_push(&vec, 43);
  TEST_ASSERT_EQUAL_INT(43, vec[0]);
  intvec_free(vec);
}

void test_intvec_first_push_grows(void) {
  intvec vec = intvec_init(0);
  TEST_ASSERT_EQUAL_INT(0, intvec_capacity(vec));
  intvec_push(&vec, 1);
  TEST_ASSERT_GREATER_THAN_INT(0, intvec_capacity(vec));
  intvec_free(vec);
}

void test_paddedvec_first_push_grows(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t e = {.first = 'a', .second = 1, .last = true};
  paddedvec_push(&vec, e);
  TEST_ASSERT_GREATER_THAN_INT(0, paddedvec_capacity(vec));
  paddedvec_free(vec);
}

void test_intvec_memcpy_five_elements(void) {
  intvec vec = intvec_init(0);
  int data[] = {10, 20, 30, 40, 50};
  intvec_memcpy(&vec, 0, data, 5);
  TEST_ASSERT_EQUAL_INT(5, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(50, vec[4]);
  intvec_free(vec);
}

void test_intvec_memcpy_sequential_five(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 5; i++) {
    int value = i * 10;
    intvec_memcpy(&vec, i, &value, 1);
  }
  TEST_ASSERT_EQUAL_INT(5, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(20, vec[2]);
  intvec_free(vec);
}

void test_intvec_six_elements(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 6; i++) {
    intvec_push(&vec, i);
  }
  TEST_ASSERT_EQUAL_INT(6, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_eight_elements(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 8; i++) {
    intvec_push(&vec, i);
  }
  TEST_ASSERT_EQUAL_INT(8, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_nine_elements(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 9; i++) {
    intvec_push(&vec, i);
  }
  TEST_ASSERT_EQUAL_INT(9, intvec_len(vec));
  intvec_free(vec);
}

void test_paddedvec_three_structs(void) {
  paddedvec vec = paddedvec_init(0);
  for (int i = 0; i < 3; i++) {
    padded_t e = {.first = (char)i, .second = i, .last = false};
    paddedvec_push(&vec, e);
  }
  TEST_ASSERT_EQUAL_INT(3, paddedvec_len(vec));
  paddedvec_free(vec);
}

void test_paddedvec_seven_structs(void) {
  paddedvec vec = paddedvec_init(0);
  for (int i = 0; i < 7; i++) {
    padded_t e = {.first = (char)i, .second = i, .last = true};
    paddedvec_push(&vec, e);
  }
  TEST_ASSERT_EQUAL_INT(7, paddedvec_len(vec));
  paddedvec_free(vec);
}

void test_intvec_resize_keeps_first_three(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 3; i++) {
    intvec_push(&vec, i + 10);
  }
  intvec_resize(&vec, 10);
  TEST_ASSERT_EQUAL_INT(10, vec[0]);
  TEST_ASSERT_EQUAL_INT(11, vec[1]);
  TEST_ASSERT_EQUAL_INT(12, vec[2]);
  intvec_free(vec);
}

void test_intvec_resize_then_pop(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_resize(&vec, 5);
  intvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(1, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_set_len_to_three(void) {
  intvec vec = intvec_init(5);
  for (int i = 0; i < 3; i++) {
    intvec_push(&vec, i);
  }
  intvec_set_len(vec, 3);
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_pop_keeps_second(void) {
  intvec vec = intvec_init(0);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  intvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(1, vec[0]);
  TEST_ASSERT_EQUAL_INT(2, vec[1]);
  intvec_free(vec);
}

void test_paddedvec_preserve_multiple_fields(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t a = {.first = 'a', .second = 10, .last = true};
  padded_t b = {.first = 'b', .second = 20, .last = false};
  padded_t c = {.first = 'c', .second = 30, .last = true};
  paddedvec_push(&vec, a);
  paddedvec_push(&vec, b);
  paddedvec_push(&vec, c);
  paddedvec_pop(vec);
  TEST_ASSERT_EQUAL_INT(10, vec[0].second);
  TEST_ASSERT_EQUAL_INT(20, vec[1].second);
  paddedvec_free(vec);
}

void test_intvec_grow_to_twenty(void) {
  intvec vec = intvec_init(0);
  for (int i = 0; i < 20; i++) {
    intvec_push(&vec, i);
  }
  TEST_ASSERT_EQUAL_INT(20, intvec_len(vec));
  TEST_ASSERT_EQUAL_INT(19, vec[19]);
  intvec_free(vec);
}

void test_intvec_capacity_twenty_five(void) {
  intvec vec = intvec_init(25);
  TEST_ASSERT_GREATER_THAN_INT(24, intvec_capacity(vec));
  intvec_free(vec);
}

void test_paddedvec_large_struct_pushed(void) {
  paddedvec vec = paddedvec_init(0);
  padded_t e = {.first = 127, .second = 65536, .last = true};
  paddedvec_push(&vec, e);
  TEST_ASSERT_EQUAL_INT(127, vec[0].first);
  TEST_ASSERT_EQUAL_INT(65536, vec[0].second);
  paddedvec_free(vec);
}

void test_intvec_shrink_to_fit(void) {
  intvec vec = intvec_init(10);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  TEST_ASSERT_GREATER_THAN_INT(3, intvec_capacity(vec));
  intvec_shrink_to_fit(&vec);
  TEST_ASSERT_EQUAL_INT(3, intvec_capacity(vec));
  TEST_ASSERT_EQUAL_INT(3, intvec_len(vec));
  intvec_free(vec);
}

void test_intvec_shrink_to_fit_no_change(void) {
  intvec vec = intvec_init(3);
  intvec_push(&vec, 1);
  intvec_push(&vec, 2);
  intvec_push(&vec, 3);
  TEST_ASSERT_TRUE(intvec_shrink_to_fit(&vec) == false);
  TEST_ASSERT_EQUAL_INT(3, intvec_capacity(vec));
  intvec_free(vec);
}

void test_paddedvec_shrink_to_fit(void) {
  paddedvec vec = paddedvec_init(10);
  padded_t e = {.first = 'a', .second = 1, .last = true};
  paddedvec_push(&vec, e);
  paddedvec_shrink_to_fit(&vec);
  TEST_ASSERT_EQUAL_INT(1, paddedvec_capacity(vec));
  TEST_ASSERT_EQUAL_CHAR('a', vec[0].first);
  paddedvec_free(vec);
}

void test_intvec_shrink_to_fit_empty(void) {
  intvec vec = intvec_init(5);
  intvec_push(&vec, 1);
  intvec_set_len(vec, 0);
  intvec_shrink_to_fit(&vec);
  TEST_ASSERT_EQUAL_INT(0, intvec_capacity(vec));
  intvec_free(vec);
}

void test_rawvec_shrink_to_fit(void) {
  rawvec vec = rawvec_init(10);
  rawvec_push(&vec, 1);
  rawvec_push(&vec, 2);
  rawvec_shrink_to_fit(&vec);
  TEST_ASSERT_EQUAL_INT(2, rawvec_capacity(vec));
  TEST_ASSERT_EQUAL_INT(2, rawvec_len(vec));
  rawvec_free(vec);
}

void test_rawvec_growth_respects_capacity(void) {
  rawvec vec = rawvec_init(0);
  rawvec_push(&vec, 1);
  TEST_ASSERT_GREATER_THAN_INT(0, rawvec_capacity(vec));
  rawvec_push(&vec, 2);
  TEST_ASSERT_GREATER_THAN_INT(1, rawvec_capacity(vec));
  rawvec_push(&vec, 3);
  TEST_ASSERT_GREATER_THAN_INT(2, rawvec_capacity(vec));
  rawvec_free(vec);
}

void test_rawvec_exact_growth_fit(void) {
  rawvec vec = rawvec_init(1);
  rawvec_push(&vec, 1);
  rawvec_push(&vec, 2);
  TEST_ASSERT_GREATER_THAN_INT(1, rawvec_capacity(vec));
  rawvec_free(vec);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_intvec_demo_should_work);
  RUN_TEST(test_paddedvec_demo_should_work);
  RUN_TEST(test_intvec_empty_initial_capacity);
  RUN_TEST(test_intvec_nonzero_initial_capacity);
  RUN_TEST(test_intvec_push_single_element);
  RUN_TEST(test_intvec_push_triggers_resize);
  RUN_TEST(test_intvec_pop_single_element);
  RUN_TEST(test_intvec_pop_all_elements);
  RUN_TEST(test_intvec_memcpy_to_offset_zero);
  RUN_TEST(test_intvec_memcpy_to_end);
  RUN_TEST(test_intvec_memcpy_zero_elements);
  RUN_TEST(test_intvec_set_len_to_zero);
  RUN_TEST(test_intvec_set_len_to_capacity);
  RUN_TEST(test_intvec_resize_up);
  RUN_TEST(test_intvec_resize_down);
  RUN_TEST(test_intvec_resize_to_zero);
  RUN_TEST(test_intvec_remaining_capacity);
  RUN_TEST(test_intvec_is_empty_after_operations);
  RUN_TEST(test_intvec_push_pop_sequence);
  RUN_TEST(test_intvec_large_initial_capacity);
  RUN_TEST(test_intvec_overflow_resize);
  RUN_TEST(test_paddedvec_empty_operations);
  RUN_TEST(test_paddedvec_push_single_struct);
  RUN_TEST(test_paddedvec_memcpy_multiple_structs);
  RUN_TEST(test_paddedvec_pop_struct);
  RUN_TEST(test_intvec_boundary_at_capacity);
  RUN_TEST(test_intvec_set_len_then_push);
  RUN_TEST(test_intvec_multiple_resizes);
  RUN_TEST(test_paddedvec_resize_struct_vector);
  RUN_TEST(test_intvec_append_via_memcpy);
  RUN_TEST(test_intvec_zero_capacity_then_push);
  RUN_TEST(test_intvec_negative_values);
  RUN_TEST(test_intvec_int_min_max);
  RUN_TEST(test_intvec_negative_pop);
  RUN_TEST(test_intvec_data_preservation_after_resize);
  RUN_TEST(test_intvec_interleaved_push_pop);
  RUN_TEST(test_intvec_resize_to_same_size);
  RUN_TEST(test_intvec_set_len_partial_then_append);
  RUN_TEST(test_intvec_multiple_appends_via_memcpy);
  RUN_TEST(test_paddedvec_all_zero_fields);
  RUN_TEST(test_paddedvec_bool_fields);
  RUN_TEST(test_paddedvec_multiple_structs_then_pop);
  RUN_TEST(test_intvec_capacity_exact_match);
  RUN_TEST(test_intvec_pop_preserves_other_elements);
  RUN_TEST(test_intvec_capacity_growth_pattern);
  RUN_TEST(test_intvec_set_len_preserves_data);
  RUN_TEST(test_intvec_overwrite_at_beginning_via_memcpy);
  RUN_TEST(test_intvec_overwrite_in_middle_via_memcpy);
  RUN_TEST(test_intvec_exact_capacity_push);
  RUN_TEST(test_intvec_single_element_operations);
  RUN_TEST(test_intvec_alternating_push_pop_many);
  RUN_TEST(test_intvec_double_elements);
  RUN_TEST(test_intvec_very_small_capacity);
  RUN_TEST(test_intvec_push_does_not_corrupt_other_data);
  RUN_TEST(test_intvec_repeated_resize_down_up);
  RUN_TEST(test_paddedvec_pop_multiple_preserves_others);
  RUN_TEST(test_paddedvec_large_struct_count);
  RUN_TEST(test_paddedvec_set_len_and_append);
  RUN_TEST(test_intvec_many_small_memcpys);
  RUN_TEST(test_intvec_capacity_one_growth);
  RUN_TEST(test_intvec_exact_resize_then_push);
  RUN_TEST(test_intvec_consecutive_push_short);
  RUN_TEST(test_intvec_zero_length_after_resize);
  RUN_TEST(test_intvec_memcpy_single_element);
  RUN_TEST(test_intvec_pop_twice_then_push);
  RUN_TEST(test_paddedvec_pop_preserves_field_values);
  RUN_TEST(test_intvec_set_len_preserves_existing_data);
  RUN_TEST(test_intvec_sequential_growth);
  RUN_TEST(test_intvec_memcpy_no_initial_elements);
  RUN_TEST(test_paddedvec_zero_initial_capacity);
  RUN_TEST(test_intvec_remaining_decreases_with_push);
  RUN_TEST(test_intvec_overwrite_only_using_memcpy);
  RUN_TEST(test_intvec_empty_vec_to_large);
  RUN_TEST(test_intvec_memcpy_increases_len);
  RUN_TEST(test_intvec_pop_clears_len);
  RUN_TEST(test_intvec_capacity_not_shrink_on_pop);
  RUN_TEST(test_paddedvec_free_on_empty);
  RUN_TEST(test_paddedvec_free_after_operations);
  RUN_TEST(test_intvec_one_by_one_push);
  RUN_TEST(test_intvec_pop_order_preserved);
  RUN_TEST(test_intvec_max_capacity_push);
  RUN_TEST(test_intvec_struct_equivalent_access);
  RUN_TEST(test_paddedvec_sequential_append);
  RUN_TEST(test_intvec_set_len_to_exact_then_append);
  RUN_TEST(test_intvec_alternating_set_len_and_push);
  RUN_TEST(test_intvec_five_elements_basic);
  RUN_TEST(test_intvec_sequential_zero_to_five);
  RUN_TEST(test_paddedvec_all_false_bool);
  RUN_TEST(test_paddedvec_all_true_bool);
  RUN_TEST(test_intvec_pop_from_full_then_push);
  RUN_TEST(test_paddedvec_ten_structs);
  RUN_TEST(test_intvec_grow_from_zero_twice);
  RUN_TEST(test_intvec_pop_and_memcpy_sequence);
  RUN_TEST(test_intvec_three_memcpy_calls);
  RUN_TEST(test_intvec_exact_capacity_boundary);
  RUN_TEST(test_intvec_all_ops_combined);
  RUN_TEST(test_intvec_initial_cap_zero_then_one);
  RUN_TEST(test_intvec_pop_does_not_shrink);
  RUN_TEST(test_intvec_push_after_exact_capacity);
  RUN_TEST(test_intvec_capacity_grows_exponentially);
  RUN_TEST(test_intvec_three_pushes);
  RUN_TEST(test_intvec_five_pushes_then_pop);
  RUN_TEST(test_paddedvec_free_then_push);
  RUN_TEST(test_paddedvec_push_pop_push);
  RUN_TEST(test_paddedvec_zero_capacity_then_push);
  RUN_TEST(test_intvec_set_and_get_multiple);
  RUN_TEST(test_intvec_all_ones);
  RUN_TEST(test_intvec_repeated_one_value);
  RUN_TEST(test_intvec_ascending_values);
  RUN_TEST(test_intvec_descending_values);
  RUN_TEST(test_intvec_capacity_greater_than_len);
  RUN_TEST(test_intvec_two_by_two_append);
  RUN_TEST(test_intvec_four_zeros);
  RUN_TEST(test_intvec_many_sequential_ops);
  RUN_TEST(test_paddedvec_sequential_ops);
  RUN_TEST(test_intvec_resize_up_large);
  RUN_TEST(test_intvec_exact_growth_one);
  RUN_TEST(test_intvec_push_pop_alternate_5_times);
  RUN_TEST(test_paddedvec_struct_with_zero_second);
  RUN_TEST(test_paddedvec_increasing_second);
  RUN_TEST(test_intvec_set_len_equals_capacity_then_push);
  RUN_TEST(test_intvec_free_and_push_new);
  RUN_TEST(test_paddedvec_free_and_push_new);
  RUN_TEST(test_intvec_exactly_seven_elements);
  RUN_TEST(test_intvec_exactly_ten_elements);
  RUN_TEST(test_intvec_zero_to_capacity_boundary);
  RUN_TEST(test_intvec_pop_five_from_ten);
  RUN_TEST(test_paddedvec_two_structs_two_pops);
  RUN_TEST(test_paddedvec_boundary_push);
  RUN_TEST(test_intvec_capacity_equals_elements);
  RUN_TEST(test_intvec_pop_leaves_first);
  RUN_TEST(test_intvec_three_pops_from_three);
  RUN_TEST(test_intvec_set_len_zero_single_element);
  RUN_TEST(test_intvec_first_push_grows);
  RUN_TEST(test_paddedvec_first_push_grows);
  RUN_TEST(test_intvec_memcpy_five_elements);
  RUN_TEST(test_intvec_memcpy_sequential_five);
  RUN_TEST(test_intvec_six_elements);
  RUN_TEST(test_intvec_eight_elements);
  RUN_TEST(test_intvec_nine_elements);
  RUN_TEST(test_paddedvec_three_structs);
  RUN_TEST(test_paddedvec_seven_structs);
  RUN_TEST(test_intvec_resize_keeps_first_three);
  RUN_TEST(test_intvec_resize_then_pop);
  RUN_TEST(test_intvec_set_len_to_three);
  RUN_TEST(test_intvec_pop_keeps_second);
  RUN_TEST(test_paddedvec_preserve_multiple_fields);
  RUN_TEST(test_intvec_grow_to_twenty);
  RUN_TEST(test_intvec_capacity_twenty_five);
  RUN_TEST(test_paddedvec_large_struct_pushed);
  RUN_TEST(test_intvec_shrink_to_fit);
  RUN_TEST(test_intvec_shrink_to_fit_no_change);
  RUN_TEST(test_paddedvec_shrink_to_fit);
  RUN_TEST(test_intvec_shrink_to_fit_empty);
  RUN_TEST(test_rawvec_shrink_to_fit);
  RUN_TEST(test_rawvec_growth_respects_capacity);
  RUN_TEST(test_rawvec_exact_growth_fit);
  return UNITY_END();
}
void setUp(void) {}
void tearDown(void) {}
