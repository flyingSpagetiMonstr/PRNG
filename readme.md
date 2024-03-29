# PRNG

A Pseudorandom Number Generator

Not using any existing cryptographic algorithms or PRNGs

---

Randomness is tested under:
1. [NIST SP 800-22](https://csrc.nist.gov/pubs/sp/800/22/r1/upd1/final) (*A Statistical Test Suite for Random and Pseudorandom Number Generators for Cryptographic Applications*)
2. dieharder
3. TestU01

For test reports, see `report/`

---
For how the latest version works, see `others/实验报告.pdf` (in Chinese).

There is also an explanation on mechanisms of [previous versions](#encapsuled).

Detailed changes and corresponding explanations across different branches/versions can be found in `others/log.md`.

# Mannual
-  compiling with gcc
-  modify this line 
   ```c
   #define OUTPUT "sts-2.1.2/data/stream.dat" // where the generated bits will be stored
   ```
-  run 'make' to compile and run it

# Efficiency and Randomness

Still under develop/improvement.

Space cost: several bits.

Time cost: able to generate one million bits in several milliseconds (about 0.1 GB/s).

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
