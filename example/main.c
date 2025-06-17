#include "arr_list.h"
#include <stdio.h>

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

  printf("Array list:\n");
  for (int i = 0; i < capacity; i++) {
    printf("%d ", l.data[i]);
  }
  printf("\n");
  free_intlist(&l);
  return 0;
}
