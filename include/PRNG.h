#ifndef INCLUDE_PRNG_H
#define INCLUDE_PRNG_H

#include <stdint.h>

// 2^N = LEN
#define N 8
#define LEN 256
#define END ((LEN)-1)

#define _INT(bit) uint ##bit## _t 
#define INT(x) _INT(x)
// uint8_t == uint8_t

#define IS_ODD(x) ((x)&1)
#define CEIL(x, n) ((x) + ((n) - (x)%(n)))

// cyclic shift for uint8_t
#define RSHIFT(x, n) ((uint8_t)(((x)>>(n))^((x)<<(8-(n))))) 
#define LSHIFT(x, n) ((uint8_t)(((x)<<(n))^((x)>>(8-(n))))) 


// state = {f[], bitstream[], x, i}
typedef struct _state_t
{
    uint8_t f[LEN];
    uint8_t bitstream[LEN];
    uint8_t x;
    uint8_t i;
    // uint64_t stream_len;
} state_t;

#endif // INCLUDE_PRNG_H