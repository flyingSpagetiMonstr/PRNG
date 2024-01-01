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

typedef struct _state_t
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
} state_t;

#define G(state) (state->x ^ state->y ^ state->z)

static inline void update(void); 
static inline uint32_t phi(uint32_t a, uint32_t b, uint32_t c);
static inline uint8_t state_f(uint8_t i);
// =========================================================

static state_t 
_s = {0xAABBCCDD, 0xBBCCDDEE, 0xCCDDEEFF}, *state = &_s;

uint32_t generator(void)
{
    update();
    return G(state);
}
// =========================================================

static inline void update(void)
{
    // uint32_t check[3] = {state->x, state->y, state->z};

    uint32_t register 
    a = state->x ^ state->y ^ state->z,
    b = state_f(a - 1),
    c = state_f(a + 1);
    
    for (int cnt = 0; cnt < strength; cnt++)
    {
        a = phi(a, b, c);
        b = phi(b, c, a);
        c = phi(c, a, b);
    }
    state->x = a; state->y = b; state->z = c;

    // if (state->x == check[0] && state->y == check[1] && state->z == check[2])
    // {
    //     puts("!!!!");
    //     for (int i = 0; i < 3; i++)
    //     {
    //         printf("xxx: %08X\n", check[i]);
    //     }
    //     exit(0);
    // }

    // 32
    // 3 * 32

    // 3 * 3 = 9
    // min: 36bit (?)
}

static inline uint32_t phi(uint32_t a, uint32_t b, uint32_t c)
{
    if (TO_02(b))
        a ^= c, a = RSHIFT(a, TO_X32(b));

    a += state_f(TO_32(c)) << TO_03(b);
    a += 1;
    return a;
}

//! \param i: [0, 32)
//! \returns `f[i]`: [0, 8)
static inline uint8_t state_f(uint8_t i)
{
    return ((state->x >> i) & (0b1)) << 2
        ^ ((state->y >> i) & (0b1)) << 1
        ^ ((state->z >> i) & (0b1)) << 0;
}

// =========================================================

// init state as "default state" 
void default_state(void)
{
    state->x = 0xAABBCCDD;
    state->y = 0xBBCCDDEE; 
    state->z = 0xCCDDEEFF;
    return;
}

// use c rand() to initialize state
void rand_state(void)
{
    srand(time(0));
    state->x = (uint32_t)rand();
    state->y = (uint32_t)rand();
    state->z = (uint32_t)rand();
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
    success = load(state, filename, sizeof(*state));
    return success;
}

// dumps state into the given file, return 1 on success and 0 when fail
int dump_state(char *filename)
{
    return dump(state, filename, sizeof(*state));
}