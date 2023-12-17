#ifndef INCLUDE_DUMP_H
#define INCLUDE_DUMP_H

// --------------------------------------
#include "PRNG.h"

// typedef state_t dump_data;

// typedef struct 
// {
//     state_t state;
//     uint64_t stream_len;
// } dump_data;

// void pack(state_t *s, uint8_t f[LEN], uint8_t bitstream[LEN], uint8_t *x, uint8_t *i);
// void parse(state_t *s, uint8_t f[LEN], uint8_t bitstream[LEN], uint8_t *x, uint8_t *i);
// --------------------------------------
void dump(void* x, char* filename, int len);
void load(void* x, char* filename, int len);

// void store
#endif // INCLUDE_DUMP_H