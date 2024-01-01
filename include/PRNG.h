#ifndef INCLUDE_PRNG_H
#define INCLUDE_PRNG_H

#include <stdint.h>
// ==================================
uint32_t generator(void);

void default_state(void);
void rand_state(void);
int load_state(char *filename);
int dump_state(char *filename);

// "strength" that can pass certain tests, default is `NIST` 
enum _strengths {
    NIST = 10, // able to pass NIST tests
    CRUSH = 12, // not tested yet
};
// ==================================

#ifndef PRNG_C
extern enum _strengths strength;
#endif

// ==================================
#endif // INCLUDE_PRNG_H