**12.28** 

no longer loops in 3*PHI

f[a] changed to a

```c
// for (int cnt = 0; cnt < 22; cnt++)
{
    PHI(a, b, c); 
    PHI(b, c, a); 
    PHI(c, a, b); 
}
```

removing break;s in switch and rearranged the operations

```c
#define PHI(x, op, a) switch (TO_FOUR(op)) {     \
    case add: (x) += (a);                        \
    case unarys: (x) = TO_TWO(a)? _f[x]: ~(x);   \
    case rshitf: (x) = RSHIFT((x), TO_EIGHT(a)); \
    case xor: (x) ^= (a);                        \
}
```
---

**12.29** 

added default to PHI, in order to do more operations in one switch

commands in default will always be executed

```c
#define PHI(x, op, a) switch (TO_FOUR(op)) {     \
    case add: (x) += (a);                        \
    case unarys: (x) = TO_TWO(a)? _f[x]: ~(x);   \
    case rshitf: (x) = RSHIFT((x), TO_EIGHT(a)); \
    case sub: (x) -= (a);                        \
    default /*lshift , xor*/:                    \
    (x) = LSHIFT((x), TO_XEIGHT(a)); x ^= a;     \
}
```

---

**12.30** 

encapsuled, re-structured

running by compiling to libprng.a to be linked.

---

**12.31** 

Running TestU01 Crush 

and testing for miminum "strength" to pass NIST tests 

---

**1.1** 

Passed TestU01 Crush

Runing BigCrush

branch `mini`:

using `uint16_t x, y, z` as state

branch `min`:

using `uint64_t x` as state

**1.2** 

BigCrush is too slow, stashed

branch `alpha`: 

using `uint64_t` as state

speed: several seconds / million bits

**1.3** 

`alpha`:

passed NIST 1,000,000 * 1000

passed TestU01 Crush, running BigCrush

---
