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
