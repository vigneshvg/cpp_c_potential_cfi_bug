#include "lib.h"
#include <stdio.h>

int main(void) {

  // This code fails in C.
  // runtime error: control flow integrity check for type 'int (int, const
  // struct CfiTest *)' failed during indirect function call

  int (*func)(int, const struct CfiTest*) = testFunction;
  const struct CfiTest *c = &kCfiTest;
  func(10, c);

  // The following code should not fail.

  int (*func2)(int, int) = testFunctionWithoutStructParam;
  func2(10, 20);

  fprintf(stderr, "done :)\n");

  return 0;
}
