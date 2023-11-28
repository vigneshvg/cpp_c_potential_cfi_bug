## Build Instructions

* Compile the library:
```
clang++ -fsanitize=cfi-icall -flto=thin -fno-sanitize-trap=cfi-icall -c lib.cc
```

* Compile the C binary:
```
clang -fsanitize=cfi-icall -flto=thin -fno-sanitize-trap=cfi-icall -c cfi.c -o cfi_c.o
```

* Compile the C++ binary:
```
clang++ -fsanitize=cfi-icall -flto=thin -fno-sanitize-trap=cfi-icall -c cfi.cc -o cfi_cc.o
```

* Link the C binary:
```
clang++ -fsanitize=cfi-icall -flto=thin -fno-sanitize-trap=cfi-icall cfi_c.o lib.o -o cfi_test_c
```

* Link the C++ binary:
```
clang++ -fsanitize=cfi-icall -flto=thin -fno-sanitize-trap=cfi-icall cfi_cc.o lib.o -o cfi_test_cc
```

Note: The same bug can be reproduced with clang's ubsan (undefined behavior
sanitizer). Simply replace the `-fsanitize=cfi-icall -flto=thin
-fno-sanitize-trap=cfi-icall` with `-fsanitize=function`.

## Description

When built with `cfi-icall` enabled or with ubsan enabled, there seems to be a
bug when using function pointers that contain a struct parameter from C code
which references a struct defined in a C++ library.

The almost-similar code when executed as C++ seems to be running fine.

### -fsanitize=cfi-icall Sample Output

Clang version:
```
Debian clang version 15.0.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

```
$ ./cfi_test_c
cfi.c:12:3: runtime error: control flow integrity check for type 'int (int, const struct CfiTest *)' failed during indirect function call
(/usr/local/google/home/vigneshv/code/clang_cfi_bug/libcfitest.so+0x3050): note: testFunction defined here
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior cfi.c:10:3 in
```

```
$ ./cfi_test_cc
... runs without errors ...
```

The error goes away when `-fsanitize-cfi-icall-generalize-pointers` is used.

The error also goes away when suppressed with `__attribute__((no_sanitize("cfi-icall")))`

### -fsanitize=function Sample Output

The libraries and binaries are compiled and linked with the
`-fsanitize=function` option.

Clang version:
```
clang version 18.0.0 (https://chromium.googlesource.com/a/external/github.com/llvm/llvm-project 74cdb8e6f8c88b97204f540601a553b412d1cd56)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home2/chromium/src/third_party/llvm-build/Release+Asserts/bin
```

```
$ ./cfi_test_c
cfi.c:12:3: runtime error: call to function (unknown) through pointer to incorrect function type 'int (*)(int, const struct CfiTest *)'
(/usr/local/google/home/vigneshv/code/clang_cfi_bug/cfi_test_c+0x2f1d8): note: (unknown) defined here
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior cfi.c:12:3 in
```

```
$ ./cfi_test_cc
... runs without errors ...
```

The error goes away when suppressed with `__attribute__((no_sanitize("function")))`
