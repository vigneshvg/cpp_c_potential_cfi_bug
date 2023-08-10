#include "lib.h"
#include <stdio.h>

int main(void) {

  using namespace cfi_test;

  // This code fails in C, but not in C++.

  int (*func)(int, const struct CfiTest*) = testFunction;
  const struct CfiTest *c = &kCfiTest;
  func(10, c);


  // The following code does not fail.

  int (*func2)(int, int) = testFunctionWithoutStructParam;
  func2(10, 20);

  fprintf(stderr, "done :)\n");

  return 0;
}
