#ifndef INCLUDE_PRNG_H
#define INCLUDE_PRNG_H

#include <stdint.h>

// ==================================
#define N 32
// #define N 16

#define _INT(x) uint##x##_t 
#define INT(x) _INT(x)

typedef INT(N) out_t;
// ==================================
out_t generator(void);

void default_state(void);
void rand_state(void);
int load_state(char *filename);
int dump_state(char *filename);

// "strength" that can pass certain tests, default is `NIST` 
enum _strengths {
    NIST = 52, // able to pass NIST tests
    MAX = (uint16_t)2*130
};
// ==================================

#ifndef PRNG_C
extern enum _strengths strength;
// extern uint16_t strength;
#endif

// ==================================
#endif // INCLUDE_PRNG_H