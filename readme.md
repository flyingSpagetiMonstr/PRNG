# PRNG

A Pseudorandom Number Generator

Not using any existing cryptographic algorithms or PRNGs

Update: it has become a CSPRNG (Cryptographically secure pseudorandom number generator) now, if I didn't get it wrong. 

---

Randomness has passed tests of [NIST SP 800-22](https://csrc.nist.gov/pubs/sp/800/22/r1/upd1/final) (*A Statistical Test Suite for Random and Pseudorandom Number Generators for Cryptographic Applications*)

For the test report, see `report\1000-test-finalAnalysisReport.txt`

---

For details on how/why it works, see `explain.md` (in simplified Chinese) on branch `hide-i`, and refer to codes on `hide-i` too.

As for codes in the default/current branch, mechannisms are almost same as `hide-i`, only same optimizations. Detailed changes and corresponding explanations can be found in log.md.

# Efficiency and Randomness

Still under develop/optimizing.

The space cost is a constant: 256+1+2 bytes, i.e. the size of `*state`.

Time cost: able to generate one million bits in several milliseconds now, and still passing NIST and dieharder tests.


# Mannual
-  compiling with gcc
-  modify this line 
   ```c
   #define OUTPUT "sts-2.1.2/data/stream.dat" // where the generated bits will be stored
   ```
-  run 'make' to compile and run it
