# Single-header dynamic array in C

This is a generic dynamic array (a.k.a. vector) implementation in C. It works
how I want it to work. You probably won't like it.

Each line, bug, and typo is artisanal C code written by me. AI was allowed to generate tests and spell check this README.

## Features

Two things are "notable" about this vector implementation.

1. You explicitly instantiate a full vector type + API for the desired element type.
2. The vector type is just a pointer to the first element in the array. You can
   think of this as if the vector transparently "decays" to just a pointer to the
   elements. Like how `char[]` "decays" to `char*`. Except the vector is _actually_
   just a pointer.

```c
// Define this before including, controlling where you'd like the _actual_ implementation to be.
#define VECTOR_INCLUDE_IMPLEMENTATION
#include "vector.h"

VECTOR_IMPL(int, intvec)  // Defines a vector of int, typedef'd as 'intvec'.

intvec v = intvec_init(69);
intvec_push(&v, 420);
assert(*v == 420);
assert(intvec_len(v) == 1);
assert(intvec_capacity(v) == 69);
```

## Implementation

Consider the first few things the `VECTOR_IMPL(some_type_t, vector_type_alias)` macro does:

```c
#define VECTOR_IMPL(element_type, typealias)                                   \
  typedef element_type *typealias;                                             \
                                                                               \
  static inline size_t typealias##_len(typealias vec) {                        \
    return rawvec_len(vec) / sizeof(element_type);                             \
  }                                                                            \
  // Many more functions.
```

This is designed so that you can do `VECTOR_IMPL` in several translation units
without problems with multiple definitions. `static inline` allows defining the
same function multiple places because it is inherently local to the unit it is
defined in. And because the vector "type" is just an alias for a pointer to the
element, the alias can be defined multiple places.

Second, because the vector type has to be just a plain pointer to the data
in order for this to work, where do we put the metadata (i.e. element count and
capacity) that we need in order to have a functioning vector type? The answer:
Behind the pointer.

That is, we allocate space for two `size_t` in addition to whatever space we
want for the elements, and we let the vector pointer point to the byte after
the size and capacity.

```
[  count | capacity | 00 01 02 03 04 05 06 07 08 09 0a 0b -- -- -- -- ]
 (  12  ) (   16   )  .......... element data ...........  ↑ unused
                      ↑
                      rawvec pointer → first element
```

To a select few, this is known as ["the shadow trick"](https://www.youtube.com/watch?v=gtk3RZHwJUA).

The macros and `static inline` functions all resolve to `rawvec` operations.
The `rawvec` is a vector of just bytes, and all the other vector types you
define are just type-specific wrappers around this primitive. You can use
`rawvec` directly if you want to.

Together, `static inline` + this little trick allows defining type-specific vector types
where and when you need them, whilst behaving just like a plain C array that
you can pass to anything that expects a pointer.

I think this is neat, and about as nice as a generic vector implementation can
be to use in C. This is my vector type. There are many like it, but this one is
mine. You don't need to like it.
