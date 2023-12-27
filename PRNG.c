#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "PRNG.h"
#include "dump.h" // for storing the ending state and reload from it later when the program restarts.

#define OUTPUT "sts-2.1.2/data/stream.dat" // where the generated bits will be stored
#define DUMP_FILE "dumps/state.dat" // where the data of ending state will be dumped into
#define INFO "dumps/stream_len.dat" // where the infomation of stream_len will be stored

#define MILLION (1000000) 
#define STREAM_LEN (MILLION*100) // required stream length (by bit)

#ifndef DIE_HARDER
#define YIELD(byte) fwrite(&byte, 1, 1, out_file)
#else
#define YIELD(byte) pass_to_dieharder(byte)
// "disable" printing to console:
#define puts(x)
#define printf(...)
#endif

// ==================================
// definitions for iteration of state->x
#define P 1567 // prime
#define G 1565 // maximum primitive root
#define G_MULT(x) (((x)*G)%P)
#define GRNG_ITER(x) (G_MULT(x))
#define COMPRESS(x) ((uint8_t)(x))
#define BOUND(x) ((x)%(P-1)+1) // convert x to 1 ~ P-1

// ==================================
// three methods to init state
enum _init_method {
    default_state, // = 0
    rand_state,
    load_state
};
// ==================================
// defines for funtion "PHI"
// ... .. .
#define TO_TWO(x) ((x)&0b1)
#define TO_FOUR(x) (((x)>>1)&0b11)
#define TO_EIGHT(x) (((x)>>3)&0b111)

// cyclic rshift for uint8_t
#define RSHIFT(x, n) (((x)>>(n))^((x)<<(8-(n))))

uint8_t *_f = NULL; 
// _f will be initialized to be state->f.
// Acts as a global variable, providing global accessibility to f.

enum _operations {add /*= 0*/, xor, rshitf, unarys};

/*! @note a = a [b] c, where b is an operation */
#define PHI(a, b, c) switch (TO_FOUR(b)) {             \
    case add: (a) += (c);                        break;\
    case xor: (a) ^= (c);                        break;\
    case rshitf: (a) = RSHIFT((a), TO_EIGHT(c)); break;\
    case unarys: (a) = TO_TWO(c)? _f[a]: ~(a);   break;\
    default:                                     break;\
}

// ==================================

/*! @note mainly updates f[i] and i*/
void update(state_t* state); 

void init_state(state_t *state, enum _init_method m);
void peak(state_t *state, int stream_len);
void pass_to_dieharder(uint8_t byte);

int main()
{
    state_t _s = {0};
    state_t *state = &_s;
    _f = state->f;
    
    init_state(state, load_state);
    
    FILE *out_file = fopen(OUTPUT, "wb");

    uint32_t byte_n = STREAM_LEN/8; // generating one byte per round

    puts("Generating...");
    uint8_t byte = 0;
    clock_t start_time = clock();
    #define g(s) (s->f[s->i])
    #define yield(x) YIELD(x)
    // while(1)
    for (uint32_t cnt = 1; cnt <= byte_n ; cnt++)
    {
        byte = g(state);
        update(state);
        // yield(byte);
    }
    #undef yield
    #undef g 
    clock_t end_time = clock();
    puts("FIN");

    double time_cost = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time cost: %.6f seconds\n", time_cost);

    printf("Dumping state into %s...\n", DUMP_FILE);
    if(!dump(state, DUMP_FILE, sizeof(*state)))
        printf("Failed to dump into %s, maybe the route doesn't exist.\n", DUMP_FILE);

    fclose(out_file);
}

void update(state_t* state)
{
    #define f (state->f)

    uint8_t old = f[state->i];

    uint8_t new = 0; // new value of f[i]
    uint8_t i_new = 0; // new value of i

    uint8_t register a = state->i;
    uint8_t register b = f[COMPRESS(a + state->x)];
    uint8_t register c = f[COMPRESS(state->x)];
    for (int cnt = 0; cnt < 22; cnt++)
    {
        PHI(a, b, c);
        PHI(b, c, f[a]);
        PHI(c, f[a], b);
    }

    new = a;
    new += state->x;
    new += (new == old);

    i_new = (state->i)^old;
    PHI(i_new, a, (b^c)&0xfe); // mask out unarys:f

    uint8_t B = f[b], C = f[c];
    PHI(B, a, c&0xfe);
    PHI(C, a, b&0xfe);
    f[b] = B;
    f[c] = C;

    f[state->i] = new;
    state->i = i_new;
    state->x = GRNG_ITER(state->x);

    #undef f
}


void init_state(state_t *state, enum _init_method m)
{
    int stream_len = 0; 
    // stream_len records the accumulated bit length generated since
    // last reset of state (i.e. initializing from default_state or rand_state) 

    // default_state:
    state->x = G;
    state->i = END/4;
    for (int i = 0; i < LEN; i++)
    {
        state->f[i] = (uint8_t)(END-1-i);
    }

    switch (m)
    {
    case default_state:
        // doing nothing, since already initialized above 
        break;

    case rand_state:
        // use c rand() to initialize state
        puts("Seeding state...");
        srand(time(0));
        for (int i = 0; i < LEN; i++)
        {
            state->f[i] = (uint8_t)rand();
        }
        state->i = (uint8_t)rand();
        state->x = (uint16_t)rand();
        state->x = BOUND(state->x);
        break;

    case load_state:
        // load state from previous dumped state
        printf("Loading state from %s...\n", DUMP_FILE);
        if(!load(state, DUMP_FILE, sizeof(*state)))
            printf("Failed to load from %s, maybe the route doesn't exist.\n", DUMP_FILE);
        if(!load(&stream_len, INFO, sizeof(stream_len)))
            printf("Failed to load from %s, maybe the route doesn't exist.\n", INFO);;
        // if nothing to load or load error (error in file operation),
        // behavior would be same as default_state
        break;

    default:
        puts("No such option, initializing by default_state");
        break;
    }
    stream_len += STREAM_LEN; // update stream_len

    // peak(state, stream_len); exit(0);
    if (!dump(&stream_len, INFO, sizeof(stream_len))) // store stream_len into file
        printf("Failed to dump into %s, maybe the route doesn't exist.\n", INFO);
}

void peak(state_t *state, int stream_len)
{
    printf("f[0]: 0x%x\n", state->f[0]);
    printf("x: %d\n", state->x);
    printf("i: %d\n", state->i);
    printf("stream_len: %d\n", stream_len);
}

struct _dieharder_buffer 
{
    uint8_t bytes[4];
    uint8_t i;
} dieharder_buffer = {0};

void pass_to_dieharder(uint8_t byte)
{
    dieharder_buffer.bytes[dieharder_buffer.i] = byte;
    if (dieharder_buffer.i == 3)
    {
        fwrite(dieharder_buffer.bytes, 1, 4, stdout);
        dieharder_buffer.i = 0;
    }
    else
    {
        dieharder_buffer.i++;
    }
}