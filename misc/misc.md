```c
// uint8_t lshitf(uint8_t x, uint8_t a) {return LSHIFT(x, a&TO_EIGHT);}
// uint8_t sub(uint8_t x, uint8_t a) {return x - a;}
// uint8_t and(uint8_t x, uint8_t a) {return x & a;} // and is bad
// uint8_t or(uint8_t x, uint8_t a) {return x | a;} // or is bad
// uint8_t mul(uint8_t x, uint8_t a) {return x * a;} // mult is bad
```

```c
// #define XCHG(a, b) a = a ^ b; b = a ^ b; a = a ^ b
```

```c
// uint8_t bit = 0;

// bit = IS_ODD(j);
// if (bit_store(bit, state->bitstream) == 1)
//     fwrite(state->bitstream, 1, LEN, out);
```

```c
// ------------------------------------------
// register uint8_t new = 0;
// new = f[index] + index;
// new = phi[new&TO_FOUR](new, f[state->x]);
// new = phi[new&TO_FOUR](new, f[new]);

// a = phi[b](a, c);
// a = phi[a&TO_FOUR](a, f[a]);

// for (register int cnt = 0; cnt < LEN; cnt++)
// {
//     new += f[cnt]; // gather
//     new += f[new]; // mix
// }
// ------------------------------------------
```