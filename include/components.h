#ifndef INCLUDE_COMPONENTS_H
#define INCLUDE_COMPONENTS_H

// ==================================
// defines for funtion "PHI"
#define TO_L_FOUR(x) ((x)&0b11)
#define TO_L_EIGHT(x) ((x)&0b111)
#define TO_H_EIGHT(x) ((x)>>5)
#define TO_M_EIGHT(x) (((x)>>2)&0b111)

// cyclic rshift for uint8_t
#define RSHIFT(x, n) (((x)>>(n))^((x)<<(8-(n))))
#define LSHIFT(x, n) (((x)<<(n))^((x)>>(8-(n))))

// enum _operations {
//     add, rshift,
//     and, mul,
//     xor, lshift,
//     or, not
// };

enum _operations {_NOT, _ADD, _RSHIFT, _XOR};

/*!
 * @attention there is no "break;" in this switch
 */
#define _PHI(a, b, c)  switch (TO_L_FOUR(b)) {    \
    case _NOT: (a) = ~(a);                        \
    case _ADD: (a) += (c);                        \
    case _RSHIFT: (a) = RSHIFT(a, TO_M_EIGHT(c)); \
    case _XOR: (a) ^= state->f[c];                \
}

// case and: (a) &= (c);                                         \ 
// case mul: (a) *= (c + (c == 0));                              \ 
// case or: (a) |= (c); a += state->x;                           \ 
// case rshift: (a) = RSHIFT(a, TO_M_EIGHT(c));                  \ 
// sub, or+and (?)

#define PHI(a ,b, c) { \
    _PHI(a, b, c);     \
    _PHI(b, c, a);     \
    _PHI(c, a, b);     \
}

// ==================================
// definitions for iteration of state->x
#define _P 1567 // prime
#define _G 1565 // maximum primitive root
#define _G_MULT(x) (((x)*_G)%_P)
#define GRNG_ITER(x) (_G_MULT(x))
#define BOUND(x) ((x)%(_P-1)+1) // convert x to 1 ~ P-1

#endif // INCLUDE_COMPONENTS_H