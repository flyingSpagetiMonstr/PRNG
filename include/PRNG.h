#ifndef INCLUDE_PRNG_H
#define INCLUDE_PRNG_H

#include <stdint.h>
// ==================================
// 2^N = LEN
#define N 8
#define LEN 256

// state = {f[], x, i}
typedef struct _state_t
{
    uint8_t f[LEN];
    uint16_t x;
    uint8_t i;
} state_t;


// ==================================
#endif // INCLUDE_PRNG_H