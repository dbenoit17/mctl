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
  *l = (IntList){0};
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
