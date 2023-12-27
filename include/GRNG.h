#ifndef INCLUDE_GRNG_H
#define INCLUDE_GRNG_H

// definitions for iteration of state->x
#define _P 1567 // prime
#define _G 1565 // maximum primitive root
#define _G_MULT(x) (((x)*_G)%_P)
#define GRNG_ITER(x) (_G_MULT(x))
#define COMPRESS(x) ((uint8_t)(x))
#define BOUND(x) ((x)%(_P-1)+1) // convert x to 1 ~ P-1

#endif // INCLUDE_GRNG_H