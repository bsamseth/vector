#pragma once
#include <stdbool.h>
#include <stddef.h>

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

// Like `memmove(&ptr[offset], source, n)` while ensuring the vector has enough
// space.
//
// Undefined if offset exceeds the current length.
//
// Returns true if the vector was moved during resizing.
bool rawvec_memmove(rawvec *ptr, size_t offset, const void *source, size_t n);

// Shorthand for `rawvec_memmove` with `offset = rawvec_len(ptr)`.
bool rawvec_extend(rawvec *ptr, const void *source, size_t n);

// Shrink the capacity to match the current length.
//
// Returns true if the vector was moved during resizing, or false if the
// capacity already matches the length.
bool rawvec_shrink_to_fit(rawvec *ptr);
