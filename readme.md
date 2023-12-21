# PRNG

A Pseudorandom Number Generator in C

Not using any existing cryptographic algorithms or PRNGs

Randomness is tested under the test suit of NIST SP 800-22

For how it works, see explain.md

currently two versions: 
- on branch "master"
  - current version, generating byte by byte
  - test results are stored in /reports
- on branch "bit"
  - legacy version, generating bit by bit, and the uniformity is not so good

compiling with:  
```
  Using built-in specs.
  COLLECT_GCC=C:\MinGW\bin\gcc.exe
  COLLECT_LTO_WRAPPER=c:/mingw/bin/../libexec/gcc/x86_64-w64-mingw32/9.2.0/lto-wrapper.exe
  Target: x86_64-w64-mingw32
  Configured with: ../src/configure --enable-languages=c,c++ --build=x86_64-w64-mingw32 --host=x86_64-w64-mingw32 --target=x86_64-w64-mingw32 --disable-multilib --prefix=/c/temp/gcc/dest --with-sysroot=/c/temp/gcc/dest --disable-libstdcxx-pch --disable-libstdcxx-verbose --disable-nls --disable-shared --disable-win32-registry --with-tune=haswell --enable-threads=posix --enable-libgomp
  Thread model: posix
  gcc version 9.2.0 (GCC)
```