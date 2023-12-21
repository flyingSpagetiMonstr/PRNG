# PRNG

A Pseudorandom Number Generator

Not using any existing cryptographic algorithms or PRNGs

Randomness has passed tests of [NIST SP 800-22](https://csrc.nist.gov/pubs/sp/800/22/r1/upd1/final) (*A Statistical Test Suite for Random and Pseudorandom Number Generators for Cryptographic Applications*)

For details on how/why it works, see explain.md (in simplified Chinese)

There is no rigid mathematical proof to guarantee the randomness, but it is not done by magic or revelation

currently two versions: 
- on branch "master"
  - current version, generating byte by byte
  - test results are stored in /reports
- on branch "bit"
  - legacy version, generating bit by bit, and the uniformity is not so good

# Mannual
0. compiling with  
   ```
    Using built-in specs.
    COLLECT_GCC=C:\MinGW\bin\gcc.exe
    COLLECT_LTO_WRAPPER=c:/mingw/bin/../libexec/gcc/x86_64-w64-mingw32/9.2.0/lto-wrapper.exe
    Target: x86_64-w64-mingw32
    Configured with: ../src/configure --enable-languages=c,c++ --build=x86_64-w64-mingw32 --host=x86_64-w64-mingw32 --target=x86_64-w64-mingw32 --disable-multilib --prefix=/c/temp/gcc/dest --with-sysroot=/c/temp/gcc/dest --disable-libstdcxx-pch --disable-libstdcxx-verbose --disable-nls --disable-shared --disable-win32-registry --with-tune=haswell --enable-threads=posix --enable-libgomp
    Thread model: posix
    gcc version 9.2.0 (GCC)
   ```
1. modify this line 
   ```c
   #define OUTPUT "sts-2.1.2/data/stream.dat" // where the generated bits will be stored
   ```
2. run 'make' to compile and run it
