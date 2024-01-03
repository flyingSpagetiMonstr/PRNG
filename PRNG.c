// #include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define PRNG_C

#include "PRNG.h" 
#include "components.h"
// for storing state when program ends, and reload from it later when the generator is restarted.
#include "dump.h" 

enum _strengths strength = NIST;

typedef INT(TWO_N) state_t;

#define LSHIFT_64(x, n) (((x)<<(n))^((x)>>(64-(n))))
#define RSHIFT_64(x, n) (((x)>>(n))^((x)<<(64-(n))))
#define G(state) (((state)>>N) ^ ((state)&Y_MASK))

static inline void update_state(void); 
// =========================================================

static state_t state = _VALUE;

out_t generator(void)
{
    update_state();
    return G(state);
}
// =========================================================

static inline void update_state(void)
{
    state_t register s = 0;
    uint8_t register i = 0;
    uint8_t register bit = 0;
    while (i < TWO_N)
    {
        s += RSHIFT_64(state, i);
        bit ^= state >> i++;
        if (bit&1)
        {
            s ^= LSHIFT_64(state, i);
            bit ^= state >> i++;
            bit ^= state >> i++;
        }
    }
    state = ~s;
    state ^= i + bit;
}
// =========================================================

// init state as "default state" 
void default_state(void)
{
    state = _VALUE;
    return;
}

// use c rand() to initialize state
void rand_state(void)
{
    srand(time(0));
    state = (uint32_t)rand();
    state = (state<<N) ^ (uint32_t)rand();
}

/*!
 * load state from data in file,
 * return 1 on success and 0 when fail
 * @attention if an error occurred in loading (mostly error in file operation), 
 * state will be set to "default state"
 */
int load_state(char *filename)
{
    int success = 0;
    default_state();
    success = load(&state, filename, sizeof(state));
    return success;
}

// dumps state into the given file, return 1 on success and 0 when fail
int dump_state(char *filename)
{
    return dump(&state, filename, sizeof(state));
}