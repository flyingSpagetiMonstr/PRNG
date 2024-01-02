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

// typedef struct _state_t
// {
//     out_t x;
//     out_t y;
// } state_t;

typedef INT(TWO_N) state_t;

#define X(state) ((state)>>N)
#define Y(state) ((state)&Y_MASK)

#define G(state) (X(state) + Y(state))

static inline void update(void); 
static inline out_t phi(out_t a, out_t b);
static inline out_t state_f(out_t i);
// =========================================================

static state_t state = _VALUE;
// _s = {0xAABBCCDD, 0xBBCCDDEE, 0xCCDDEEFF}, *state = &_s;

out_t generator(void)
{
    update();
    return G(state);
}
// =========================================================

static inline void update(void)
{
    out_t register a = X(state) ^ Y(state);
    out_t register b = state_f(a);
    
    for (int cnt = 0; cnt < strength; cnt++)
    {
        a = phi(a, b);
        b = phi(b, a);
    }
    state = a;
    state = (state<<N) ^ b;
    state = ~state;
}

static inline out_t phi(out_t a, out_t b)
{
    a += state_f(b);
    // a = SHIFT_N(a, TO_N(b));
    return a;
}

static inline out_t state_f(out_t i)
{
    return X(SHIFT_2N(state, TO_2N(i)));
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
    state = (state<<N) + (uint32_t)rand();
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