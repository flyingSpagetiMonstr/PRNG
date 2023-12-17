#include "PRNG.h"
#include "dump.h"

#include <stdio.h>
#include <string.h>
    
// void pack(state_t *s, uint8_t f[LEN], uint8_t bitstream[LEN], uint8_t *x, uint8_t *i)
// {
//     memcpy(s->f, f, sizeof(s->f));
//     memcpy(s->bitstream, bitstream, sizeof(s->bitstream));
//     s->x = *x;
//     s->i = *i;
// }

// void parse(state_t *s, uint8_t f[LEN], uint8_t bitstream[LEN], uint8_t *x, uint8_t *i)
// {
//     memcpy(f, s->f, sizeof(s->f));
//     memcpy(bitstream, s->bitstream, sizeof(s->bitstream));
//     *x = s->x;
//     *i = s->i;
// }

// void peak(char* filename)
// {
//     dump_data _x;
//     dump_data *x = & _x;
//     load(x, filename);
//     printf("%d", x->stream_len);
// }

void dump(void* x, char* filename, int len)
{
   FILE* dst = fopen(filename, "wb");
   fwrite(x, len, 1, dst);
   fclose(dst);
}

void load(void* x, char* filename, int len)
{
    FILE* src = fopen(filename, "rb");
    fread(x, len, 1, src);
    fclose(src);
}
