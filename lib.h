#ifdef __cplusplus
namespace cfi_test {
extern "C" {
#endif

extern const struct CfiTest kCfiTest;

int testFunction(int a, const struct CfiTest* b);

int testFunctionWithoutStructParam(int a, int b);

#ifdef __cplusplus
}
}
#endif
