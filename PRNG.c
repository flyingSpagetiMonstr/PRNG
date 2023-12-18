#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bitstore.h"
#include "PRNG.h"
#include "dump.h"

#define OUTPUT "sts-2.1.2/data/stream.dat"
// #define OUTPUT "sts-2.1.2/data/runtest.dat"
#define DUMP_FILE "dumps/state.dat"
#define INFO "dumps/stream_len.dat"

#define MILLION (1000000)
#define STREAM_LEN (MILLION*100)

enum _init_method {
    zero_state, // = 0
    rand_state,
    load_state
};

// ==================================
// GRNG: Generator Random Number Generator 
#define P 251
#define G 248 // maximum primitive root
#define G_MULT(x) (((x)*G)%P)
#define GRNG_ITER(x) ((uint8_t)G_MULT(x))
// #define BOUND(x) ((x)%(P-1)+1)
// #define GRNG_UPDATE(x) (x) = GRNG_ITER(x)
// ==================================

void update(state_t* state);
void init_state(state_t *state, enum _init_method m);
void peak(state_t *state, int stream_len);

int main()
{
    uint8_t bit = 0;
    uint8_t j = 0;

    state_t _s = {0};
    state_t *state = &_s;
    init_state(state, load_state);
    // init_state(state, zero_state);
    // init_state(state, rand_state);

    FILE *out = fopen(OUTPUT, "wb");
    int ajusted_stream_len = CEIL(STREAM_LEN, LEN*8); 
    
    puts("Generating...");
    #define i (state->i)
    for (uint64_t cnt = 0; cnt < ajusted_stream_len; cnt++)
    {
        j = state->f[i];
        bit = IS_ODD(j);
        
        if (bit_store(bit, state->bitstream) == 1)
            fwrite(state->bitstream, 1, LEN, out);

        update(state); // update f[i]
        i = j;

        if ((cnt % (ajusted_stream_len/10)) == (ajusted_stream_len/10-1))
            printf(".");
    }
    #undef i
    puts("FIN");

    printf("Dumping state into %s...\n", DUMP_FILE);
    dump(state, DUMP_FILE, sizeof(*state));

    fclose(out);
}

void update(state_t* state)
{
    #define f (state->f)
    #define index (state->i)

    register uint8_t new = 0;
    for (register int cnt = 0; cnt < LEN; cnt++)
    {
        new += f[f[cnt]] >> 1;
    }

    new += state->x;
    state->x = GRNG_ITER(state->x);

    uint8_t i = new;
    uint8_t j = i;
    while (new == f[index])
    {
        i += state->bitstream[index];
        i += (i == j);
        j = i;
        new += i;
    }

    f[index] = new;
    #undef index
    #undef f
}


void init_state(state_t *state, enum _init_method m)
{
    state->x = G;
    switch (m)
    {
    case zero_state:
        break;

    case rand_state:
        puts("Seeding state...");
        srand(time(0));
        for (int i = 0; i < LEN; i++)
        {
            state->f[i] = (uint8_t)rand();
        }
        break;

    case load_state:
        // if nothing to load, behavior would be same as zero_state
        printf("Loading state from %s...\n", DUMP_FILE);
        load(state, DUMP_FILE, sizeof(*state));
        break;

    default:
        puts("No such option.");
        break;
    }
    int stream_len = 0;
    load(&stream_len, INFO, sizeof(stream_len));
    stream_len += STREAM_LEN;
    // peak(state, stream_len); exit(0);
    dump(&stream_len, INFO, sizeof(stream_len));
}

void peak(state_t *state, int stream_len)
{
    printf("f[0]: 0x%x\n", state->f[0]);
    printf("bitstream[0]: 0x%x\n", state->bitstream[0]);
    printf("x: %d\n", state->x);
    printf("i: %d\n", state->i);
    printf("stream_len: %d\n", stream_len);
}