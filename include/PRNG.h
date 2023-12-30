#ifndef INCLUDE_PRNG_H
#define INCLUDE_PRNG_H

#include <stdint.h>
// ==================================
uint8_t generator(void);

void default_state(void);
void rand_state(void);
int load_state(char *filename);
int dump_state(char *filename);
// ==================================
#endif // INCLUDE_PRNG_H