#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define PRNG_C

#include "PRNG.h" 
#include "components.h"

// for storing state when program ends, and reload from it later when the generator is restarted.
#include "dump.h" 

enum _strengths strength = NIST;

// =============================
// 2^N = LEN
#define N 8
#define LEN 256
#define END ((LEN)-1)

// state = {f[], x, i}
typedef struct _state_t
{
    uint8_t f[LEN];
    uint16_t x;
    uint8_t i;
} state_t;

#define CONFINE(x) ((uint8_t)(x))
#define G(s) (s->f[s->i])
// =============================

// mainly updates f[i] and i
static inline void update(void); 
// =========================================================

static state_t _s = {.x = 1}, *state = &_s;
uint8_t generator(void)
{
    update();
    return G(state);
}
// =========================================================

static inline void update(void)
{
    #define F (state->f)

    uint8_t old = F[state->i], new = old;  // old and new values of f[i]
    uint8_t x = 0, y = 0;

    uint8_t register a = state->i;
    uint8_t register b = F[CONFINE(state->x)];
    uint8_t register c = F[CONFINE(a + state->x)];
    uint16_t register cnt = 0;
    for (cnt = 0; cnt < strength; cnt++)
        PHI(a, b, c);
    state->i ^= old^c;

    PHI(a, b, c); x = c;
    PHI(a, b, c); y = c;
    PHI(a, b, c); new ^= c;

    F[x] ^= y;
    F[state->i] = new ^ state->x;

    state->x = GRNG_ITER(state->x);
    #undef F
}


// =========================================================

// init state as "default state" 
void default_state(void)
{
    state->x = 1;
    state->i = END/4;
    for (int i = 0; i < LEN; i++)
    {
        state->f[i] = (uint8_t)(END-1-i);
    }
}

// use c rand() to initialize state
void rand_state(void)
{
    srand(time(0));
    for (int i = 0; i < LEN; i++)
    {
        state->f[i] = (uint8_t)rand();
    }
    state->i = (uint8_t)rand();
    state->x = (uint16_t)rand();
    state->x = BOUND(state->x);
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
    state->x = BOUND(state->x);
    return success;
}

// dumps state into the given file, return 1 on success and 0 when fail
int dump_state(char *filename)
{
    return dump(state, filename, sizeof(*state));
}

// void peak(state_t *state, int stream_len);
// void peak(state_t *state, int stream_len)
// {
//     printf("f[0]: 0x%x\n", state->f[0]);
//     printf("x: %d\n", state->x);
//     printf("i: %d\n", state->i);
//     printf("stream_len: %d\n", stream_len);
// }
