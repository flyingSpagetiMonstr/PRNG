#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "PRNG-basics.h"
#include "components.h"
#include "dump.h" // for storing the ending state and reload from it later when the program restarts.

#define STRENGTH ((uint8_t)7) // 3*7 + 2 = 23 | *9/16 =~ 13 f[]
#define G(s) (s->f[s->i])

// mainly updates f[i] and i
static inline void update(void); 

// =========================================================
static state_t _s = {0}, *state = &_s;
uint8_t generator(void)
{
    update();
    return G(state);
}
// =========================================================

static inline void update(void)
{
    #define F (state->f)

    uint8_t register old = F[state->i];
    uint8_t register A = 0, B = 0, s = 0;

    uint8_t register new = 0; // new value of f[i]
    uint8_t register i_new = 0; // new value of i

    uint8_t register a = state->i;
    uint8_t register b = F[COMPRESS(a + state->x)];
    uint8_t register c = F[COMPRESS(state->x)];

    for (uint8_t register i = 0; i < STRENGTH; i++)
    {
        PHI(a, b, c); 
        PHI(b, c, a); 
        PHI(c, a, b); 
    }

    new = c;
    new += state->x;
    new += (new == old);

    s = F[c];

    i_new = (state->i)^old;
    PHI(i_new, s, a^b);

    A = F[a]; B = F[b]; 
    PHI(A, s, b);
    PHI(B, s, a);

    // assigning new values: 
    F[a] = B; F[b] = A;
    F[state->i] = new;
    state->i = i_new;
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
