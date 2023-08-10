## Build Instructions

* Build the library:
```
clang++ -shared -fsanitize=cfi-icall -fsanitize-cfi-cross-dso -flto=thin -fno-sanitize-trap=cfi-icall -o libcfitest.so lib.cc
```

* Build the C binary:
```
clang -fsanitize=cfi-icall -fsanitize-cfi-cross-dso -flto=thin -fno-sanitize-trap=cfi-icall -L. -lcfitest -o cfi_test_c cfi.c
```

* Build the C++ binary:
```
clang++ -fsanitize=cfi-icall -fsanitize-cfi-cross-dso -flto=thin -fno-sanitize-trap=cfi-icall -L. -lcfitest -o cfi_test_cc cfi.cc && echo "Running"  && LD_LIBRARY_PATH=`pwd` ./cfi_test_cc
```

## Description

When built with `cfi-icall` enabled, there seems to be a bug when using function
pointers that contain a struct parameter from C code which references a struct
defined in a C++ library.

The almost-similar code when executed as C++ seems to be running fine.

Clang version:
```
Debian clang version 15.0.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

```
$ LD_LIBRARY_PATH=`pwd` ./cfi_test_c
cfi.c:10:3: runtime error: control flow integrity check for type 'int (int, const struct CfiTest *)' failed during indirect function call
(/usr/local/google/home/vigneshv/code/clang_cfi_bug/libcfitest.so+0x3050): note: testFunction defined here
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior cfi.c:10:3 in
```

```
$ LD_LIBRARY_PATH=`pwd` ./cfi_test_cc
... runs without errors ...
```
