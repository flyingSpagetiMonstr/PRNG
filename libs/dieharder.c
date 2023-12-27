#include "dieharder.h"

static struct _dieharder_buffer 
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
