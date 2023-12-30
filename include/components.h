#ifndef INCLUDE_COMPONENTS_H
#define INCLUDE_COMPONENTS_H
#include "PRNG-basics.h"

// ==================================
// defines for funtion "PHI"
// xx ... .. .
// #define TO_TWO(x) ((x)&0b1)
// #define TO_FOUR(x) (((x)>>1)&0b11)
#define TO_FOUR(x) ((x)&0b11)
#define TO_EIGHT(x) (((x)>>3)&0b111)
#define TOXFOUR(x) ((x)>>4)
#define TOXEIGHT(x) ((x)>>5)

// cyclic rshift for uint8_t
#define RSHIFT(x, n) (((x)>>(n))^((x)<<(8-(n))))
#define LSHIFT(x, n) (((x)<<(n))^((x)>>(8-(n))))

// _f will be initialized to be state->f.
// Acts as a global variable, providing global accessibility to f.

enum _operations {add /*= 0*/, xor, rshitf, unarys, lshift};

/*!
 * @note x = x [op] a, where op is an operation 
 * @attention there is no "break;" in this switch, 
 * and "case lshift" is out of range so will always be
 * executed
 */
#define PHI(x, op, a) switch (TO_FOUR(op)) {     \
    case add: (x) += (a);                        \
    case rshitf: (x) = RSHIFT((x), TO_EIGHT(a)); \
    case unarys: (x) = TOXFOUR(a)? state->f[x]: ~(x);  \
    case xor: (x) ^= (a);                        \
    case lshift: (x) = LSHIFT((x), TOXEIGHT(a)); \
}
// P(f[] is used) = 9/16

// ==================================
// definitions for iteration of state->x
#define _P 1567 // prime
#define _G 1565 // maximum primitive root
#define _G_MULT(x) (((x)*_G)%_P)
#define GRNG_ITER(x) (_G_MULT(x))
#define COMPRESS(x) ((uint8_t)(x))
#define BOUND(x) ((x)%(_P-1)+1) // convert x to 1 ~ P-1


#endif // INCLUDE_COMPONENTS_H