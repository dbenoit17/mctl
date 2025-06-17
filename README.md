# mctl: Minimal C Testing Library

## Usage

Tests are defined in `_MCTL_MODE_TEST` blocks as functions matching the signature:

```void Test\S+(TestingT *t)```

The script `generate_tests.sh *.c` will extract tests via ctags and output a test driver.
Build the driver with the `-D_MCTL_MODE_TEST`.

## Example:

The following example implements a test for an array list of integers.

``` arr_list.c
#include "arr_list.h"
#include <stdlib.h>

IntList new_intlist(int capacity) {
  IntList l;
  l.length = 0;
  l.capacity = capacity;
  l.data = malloc(sizeof(int) * capacity);
  return l;
}

void free_intlist(IntList *l) {
  free(l->data);
  *l = (IntList) {0};
}

void append_intlist(IntList *l, int i) {
  if (l->length >= l->capacity) {
    l->capacity *= 2;
    int *new_data = malloc(sizeof(int) * l->capacity);
    for (int j = 0; j < l->length; j++) {
      new_data[j] = l->data[j];
    }
    free(l->data);
    l->data = new_data;
  }
  l->data[l->length] = i;
  l->length++;
}

#ifdef _MCTL_MODE_TEST
#include "testing.h"
void TestIntListAppend(TestingT *t) {
  IntList l = new_intlist(256);
  if (l.capacity != 256) {
    test_error(t, "Bad capacity: have %d, want %d", l.capacity, 256);
  }
  for (int i = 0; i < 512; i++) {
    append_intlist(&l, i);
  }
  if (l.capacity != 512) {
    test_error(t, "Bad capacity: have %d, want %d", l.capacity, 512);
  }
  if (l.data[511] != 511) {
    test_error(t, "Bad data: have %d, want %d", l.data[511], 511);
  }
  append_intlist(&l, 512);
  if (l.capacity != 1024) {
    test_error(t, "Bad capacity: have %d, want %d", l.capacity, 1024);
  }
  free_intlist(&l);
}
#endif
```

```arr_list.h
#pragma once

// Array list of integers
typedef struct IntList {
  int length;
  int capacity;
  int *data;
} IntList;

IntList new_intlist(int capacity);
void append_intlist(IntList *l, int i);
void free_intlist(IntList *l);
```

```main.c
#include <stdio.h>
#include "arr_list.h"

#ifndef _MCTL_MODE_TEST
int main(void) {
#else
int _main(void) {
#endif
  int capacity = 512;
  IntList l = new_intlist(capacity);
  for (int i = 0; i < capacity; i++) {
    append_intlist(&l, i);
  }

  for (int i = 0; i < capacity; i++) {
    printf("%d ", l.data[i]);
  }
  printf("\n");
  free_intlist(&l);
  return 0;
}
```

```Makefile
# Path to the test library
MCTL = ..

# Name of the main application
MAIN = main

all: $(MAIN)

# The following section builds intermediate object files
# in a _build directory.
OBJDIR ?= _build
SRCDIR ?= .
SRCS += $(shell ls *.c)
OBJS += $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
$(OBJDIR):
	@mkdir -p $(OBJDIR)
$(OBJS): $(OBJDIR)/%.o: %.c $(OBJDIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

# Build the main application.
# Intermediate objects are linked from _build.
$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(IFLAGS) $(LDFLAGS) $(OBJS) -o $(MAIN)

# Build the tests using mctl
TEST_GENERATOR = $(MCTL)/generate_tests.sh
TEST_DRIVER = _test/_test_main_.c
TEST_MAIN = test.main

# Run the test generator and output the driver file.
$(TEST_DRIVER): $(TEST_GENERATOR) *.c
	bash $(TEST_GENERATOR) *.c > $(TEST_DRIVER)

# This target builds the test driver.  Do not invoke the target directly.
# Use `make test`, which will call this after setting up the test parameters.
$(TEST_MAIN): $(OBJS) $(TEST_DRIVER)
	$(CC) $(CFLAGS) $(IFLAGS) $(LDFLAGS) $(OBJS) $(TEST_DRIVER) -o $(TEST_MAIN)

# This is the Makefile frontend target for building the tests.
# We set the following parameters:
#    CFLAGS=-D_MCTL_MODE_TEST 	 : Includes the test functions in the build.
#    OBJDIR=_test		 : Override object directory to _test instead of _build.
#				   This eliminates object dependency conflicts.
#    IFLAGS="$(IFLAGS) -I$(MCTL)": Include the location of the testing.h header.
.PHONY: test
test:
	$(MAKE) $(TEST_MAIN) CFLAGS=-D_MCTL_MODE_TEST OBJDIR=_test IFLAGS="$(IFLAGS) -I$(MCTL)"
.PHONY: run_test
run_test: test
	./$(TEST_MAIN)

clean:
	-rm -rf _build _test $(TEST_MAIN) $(MAIN)
```
