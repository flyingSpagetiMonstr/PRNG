# PRNG

NIST SP 800-22

currently two versions: 
- generating by bit: bit
- generating by byte: master


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

for how it works, see /misc/explain.md