#ifndef INCLUDE_COMPONENTS_H
#define INCLUDE_COMPONENTS_H
#include "PRNG.h"

// ==================================
// defines for funtion "PHI"
// ... .. .
#define TO_TWO(x) ((x)&0b1)
#define TO_FOUR(x) (((x)>>1)&0b11)
#define TO_EIGHT(x) (((x)>>3)&0b111)

// cyclic rshift for uint8_t
#define RSHIFT(x, n) (((x)>>(n))^((x)<<(8-(n))))

uint8_t *_f = NULL; 
// _f will be initialized to be state->f.
// Acts as a global variable, providing global accessibility to f.

enum _operations {add /*= 0*/, xor, rshitf, unarys};

/*! @note a = a [b] c, where b is an operation */
#define PHI(a, b, c) switch (TO_FOUR(b)) {             \
    case add: (a) += (c);                        break;\
    case xor: (a) ^= (c);                        break;\
    case rshitf: (a) = RSHIFT((a), TO_EIGHT(c)); break;\
    case unarys: (a) = TO_TWO(c)? _f[a]: ~(a);   break;\
    default:                                     break;\
}

// ==================================
// definitions for iteration of state->x
#define _P 1567 // prime
#define _G 1565 // maximum primitive root
#define _G_MULT(x) (((x)*_G)%_P)
#define GRNG_ITER(x) (_G_MULT(x))
#define COMPRESS(x) ((uint8_t)(x))
#define BOUND(x) ((x)%(_P-1)+1) // convert x to 1 ~ P-1


#endif // INCLUDE_COMPONENTS_H