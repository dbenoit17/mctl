#pragma once

// Array list (vector) of integers
typedef struct IntList {
  int length;
  int capacity;
  int *data;
} IntList;

IntList new_intlist(int capacity);
void append_intlist(IntList *l, int i);
void free_intlist(IntList *l);
