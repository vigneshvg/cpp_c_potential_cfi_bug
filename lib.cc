#include "lib.h"

namespace cfi_test {

struct CfiTest {
  int x;
  int y;
};

const struct CfiTest kCfiTest = {10, 10};

int testFunction(int a, const struct CfiTest* b) {
    return a + b->x + b->y;
}

int testFunctionWithoutStructParam(int a, int b) {
    return a + b;
}

}
