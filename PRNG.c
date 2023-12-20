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
// P: 1567, G_most = 1565
#define P 1567
#define G 1565 // maximum primitive root
#define G_MULT(x) (((x)*G)%P)
#define GRNG_ITER(x) (G_MULT(x))
#define COMPRESS(x) ((uint8_t)(x))
// ==================================
#define OP_N (4) // 2^{2}
#define TO_TWO (0b1)
#define TO_FOUR (0b11)
#define TO_EIGHT (0b111)

state_t *auxiliary = NULL;
uint8_t add(uint8_t x, uint8_t a) {return x + a;}
uint8_t xor(uint8_t x, uint8_t a) {return x ^ a;} 
uint8_t rshitf(uint8_t x, uint8_t a) {return RSHIFT(x, a&TO_EIGHT);}
uint8_t unarys(uint8_t x, uint8_t a) {return (a&TO_TWO)? ~x: auxiliary->f[x];}

typedef uint8_t (*operation)(uint8_t, uint8_t);
operation phi[OP_N] = {add, xor, rshitf, unarys};
#define PHI(x) (phi[(x)&TO_FOUR])
// ==================================

void update(state_t* state);
void init_state(state_t *state, enum _init_method m);
void peak(state_t *state, int stream_len);

int main()
{
    uint8_t j = 0;

    state_t _s = {0};
    state_t *state = &_s;
    auxiliary = state;

    init_state(state, load_state);
    // init_state(state, zero_state);
    // init_state(state, rand_state);

    FILE *out = fopen(OUTPUT, "wb");
    int ajusted_stream_len = CEIL(STREAM_LEN, LEN*8); 
    ajusted_stream_len /= 8;

    puts("Generating...");
    clock_t start_time = clock();
    #define i (state->i)
    for (uint64_t cnt = 0; cnt < ajusted_stream_len; cnt++)
    {
        j = state->f[i];
        update(state); // mainly updates f[i]
        i = j;

        fwrite(&j, 1, 1, out);
        if ((cnt % (ajusted_stream_len/10)) == (ajusted_stream_len/10-1))
            printf(".");
    }
    #undef i
    clock_t end_time = clock();
    puts("FIN");

    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Elapsed time: %.6f seconds\n", elapsed_time);

    printf("Dumping state into %s...\n", DUMP_FILE);
    dump(state, DUMP_FILE, sizeof(*state));

    fclose(out);
}

void update(state_t* state)
{
    #define f (state->f)
    #define index (state->i)

    uint8_t old = f[index];

    uint8_t new = 0;
    uint8_t register a = index;
    uint8_t register b = f[COMPRESS(a + state->x)];
    uint8_t register c = f[COMPRESS(state->x)];
    for (int cnt = 0; cnt < 22; cnt++)
    {
        a = PHI(b)(a, c);
        b = PHI(c)(b, f[a]);
        c = PHI(f[a])(c, b);
    }
    
    uint8_t B = 0, C = 0;
    B = PHI(a)(f[c], b|1);
    C = PHI(a)(f[b], c|1);
    f[b] = B;
    f[c] = C;

    new = a;

    new += state->x;
    state->x = GRNG_ITER(state->x);

    new += (new == old);

    f[index] = new;

    #undef index
    #undef f
}


void init_state(state_t *state, enum _init_method m)
{
    int stream_len = 0;
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
        // if nothing to load or load error (error in file operation),
        // behavior would be same as zero_state
        printf("Loading state from %s...\n", DUMP_FILE);
        load(state, DUMP_FILE, sizeof(*state));
        load(&stream_len, INFO, sizeof(stream_len));
        break;

    default:
        puts("No such option.");
        break;
    }
    stream_len += STREAM_LEN;
    // peak(state, stream_len); exit(0);
    dump(&stream_len, INFO, sizeof(stream_len));
}

void peak(state_t *state, int stream_len)
{
    printf("f[0]: 0x%x\n", state->f[0]);
    printf("x: %d\n", state->x);
    printf("i: %d\n", state->i);
    printf("stream_len: %d\n", stream_len);
}