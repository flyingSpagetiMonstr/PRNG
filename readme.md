# PRNG

A Pseudorandom Number Generator

Not using any existing cryptographic algorithms or PRNGs

Update: it has become a CSPRNG (Cryptographically secure pseudorandom number generator) now, if I didn't get it wrong. 

---

Randomness has passed tests of [NIST SP 800-22](https://csrc.nist.gov/pubs/sp/800/22/r1/upd1/final) (*A Statistical Test Suite for Random and Pseudorandom Number Generators for Cryptographic Applications*)

For the test report, see `report\1000-test-finalAnalysisReport.txt`

---

There is an explanation on mechanisms of [previous versions](#encapsuled), but the approaches and thoughts are constantly evolving.

Detailed changes and corresponding explanations across different branches/versions can be found in `log.md`.

# Efficiency and Randomness

Still under develop/improvement.

Space cost: several bits.

Time cost: able to generate one million bits in several milliseconds, still passing NIST and TestU01 Crush and ...

# Active Branches

## alpha
latest version, currently developing, using several bits as state

## encapsuled
latest veriosn of using `uint8_t f[256]` as part of state

passed NIST, dieharder, TestU01 Crush

was running BigCrush but it was too slow so I terminated it, the incomplete report is also stored in `report/` of this branch


### `explain.md`
For details on how/why it works, see `explain.md` (in simplified Chinese) on branch `hide-i`, and refer to codes on `hide-i` too.
### note
Seeing from now, approaches in `hide-i` is quite dumb, and thoughts in `explain.md` are also very immature. 

## min

# Inactive Branches
archived, no longer wotking on
- master 
- mini
- neg-optm
- optm
- hide-i
- dieharder-adapted
- bit (initial version)

# Mannual
-  compiling with gcc
-  modify this line 
   ```c
   #define OUTPUT "sts-2.1.2/data/stream.dat" // where the generated bits will be stored
   ```
-  run 'make' to compile and run it
