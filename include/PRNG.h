#ifndef INCLUDE_PRNG_H
#define INCLUDE_PRNG_H

#include <stdint.h>
// ==================================
uint8_t generator(void);

void default_state(void);
void rand_state(void);
int load_state(char *filename);
int dump_state(char *filename);

// "strength" that can pass certain tests, default is `NIST` 
enum _strengths {
    // this strength failed NIST tests
    NIST_FAIL = 0, 
    // minimum strength to pass NIST tests
    NIST = 1, 
    // this strength failed TestU01 Crush (@matrix_rank, p-value = 9e-4, on win, without -lgmp)
    CRUSH_FAIL = 5, 
    // able to pass TestU01 Crush
    CRUSH = 10, 
    // testing
    BIG_CRUSH = 10 
};
// ==================================

#ifndef PRNG_C
extern enum _strengths strength;
#endif

// ==================================
#endif // INCLUDE_PRNG_H