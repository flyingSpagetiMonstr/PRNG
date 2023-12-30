#ifndef INCLUDE_PRNG_BASICS_H
#define INCLUDE_PRNG_BASICS_H

// ==================================
#include <stdint.h>
// 2^N = LEN
#define N 8
#define LEN 256
#define END ((LEN)-1)

// state = {f[], x, i}
typedef struct _state_t
{
    uint8_t f[LEN];
    uint16_t x;
    uint8_t i;
} state_t;

// ==================================
#endif // INCLUDE_PRNG_BASICS_H