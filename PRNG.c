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
// #define STREAM_LEN (MILLION*100*10)
// #define STREAM_LEN (100000000*10)
// #define STREAM_LEN (100000000*3)
// #define STREAM_LEN (100000000/0xff)
// 0x5f0ff9f; 0x11767580;
// #define STREAM_LEN (100000)

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
#define OP_N (4) // 2^{2}
#define TO_FOUR (0b11)
#define TO_EIGHT (0b111)
uint8_t and(uint8_t x, uint8_t a) {return x ^ a;}
uint8_t or(uint8_t x, uint8_t a) {return x | a;}
// uint8_t add(uint8_t x, uint8_t a) {return x + a;}
// uint8_t sub(uint8_t x, uint8_t a) {return x - a;}
uint8_t mul(uint8_t x, uint8_t a) {return x * a;}
uint8_t rshitf(uint8_t x, uint8_t a) {return RSHIFT(x, a&TO_EIGHT);}
// uint8_t rshitf(uint8_t x, uint8_t a) {return LSHIFT(x, a&COMPRESS);}
// uint8_t map(uint8_t x, uint8_t a) {return ... ;}
typedef uint8_t (*operation)(uint8_t, uint8_t);
operation phi[OP_N] = {and, or, mul, rshitf};
// ==================================

void update(state_t* state);
void init_state(state_t *state, enum _init_method m);
void peak(state_t *state, int stream_len);

int main()
{
//     uint8_t x[] = {0x7F, 0x6F}; 
//     char buff[9] = {0};
//     for (int i = 0; i < 2; i++)
//     {
//         printf("R: %8s ", itoa(RSHIFT(x[i],1), buff, 2));
//         printf("L: %8s ", itoa(LSHIFT(x[i],1), buff, 2));
//     }
// exit(0);

    uint8_t bit = 0;
    uint8_t j = 0;

    state_t _s = {0};
    state_t *state = &_s;
    init_state(state, load_state);
    // init_state(state, zero_state);
    // init_state(state, rand_state);

    FILE *out = fopen(OUTPUT, "wb");
    int ajusted_stream_len = CEIL(STREAM_LEN, LEN*8); 

    // ajusted_stream_len /= 8;
    
    puts("Generating...");
    #define i (state->i)
    for (uint64_t cnt = 0; cnt < ajusted_stream_len; cnt++)
    {
        j = state->f[i];
        bit = IS_ODD(j);

        // ##########################################
        // if (bit_store(bit, state->bitstream) == 1)
        //     fwrite(state->bitstream, 1, LEN, out);

        update(state); // update f[i]
        fwrite(&j, 1, 1, out);
        i = j;

        if ((cnt % (ajusted_stream_len/10)) == (ajusted_stream_len/10-1))
            printf(".");
    }
    #undef i
    puts("FIN");

    printf("Dumping state into %s...\n", DUMP_FILE);
    // pack(state, f, bitstream, &i, &x);
    dump(state, DUMP_FILE, sizeof(*state));

    fclose(out);
}

void update(state_t* state)
{
    #define f (state->f)
    #define index (state->i)

    // uint8_t* mix = (uint8_t*)state;
    // ----------------------------------

    // register uint8_t new = 0;
    register uint8_t new = f[index];

    // new = phi[new&(OP_N-1)](new, state->x); // 100 - 1 = 11
    new = phi[new&TO_FOUR](new, f[state->x]);
    new = phi[new&TO_FOUR](new, f[new]);

    for (register int cnt = 0; cnt < LEN; cnt++)
    {
        new += f[cnt]; // gather
        new += f[new]; // mix
        // new += cnt;
        // new <- new+f[cnt] + f[new+f[cnt]]

        // new += f[(uint8_t)(f[cnt]+new)];

        // new += f[f[cnt]]>>1;
    }
    // ----------------------------------

    new += state->x;
    state->x = GRNG_ITER(state->x);

    // ###########################
    uint8_t i = new;
    uint8_t j = i;
    while (new == f[index])
    {
        new += i;
        i += 0; // i += state->bitstream[index];
        i += (i == j);
        j = i;
    }
    // ###########################

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