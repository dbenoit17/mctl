#pragma once

#include <stdarg.h>
#include <stdio.h>

typedef enum TestingT { PASS = 0, FAIL = 1 } TestingT;
typedef void (*UnitTest)(TestingT *);
static inline void test_error(TestingT *t, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "Error: \n\t");
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");
  va_end(args);
}
