#include <stdint.h>
#include "bitstore.h"

uint8_t bit_cache = 0;
uint8_t bit_pos = 7; // 0 ~ 7

// store bit in byte
uint8_t _bit_store(uint8_t bit)
{
    uint8_t full = (bit_pos == 0);
    uint8_t reset = (bit_pos == 0xff);
    if (reset)
    {
        bit_cache = 0;
        bit_pos = 7;
    }

    // bit = !!bit;
    bit_cache += (bit << bit_pos);
    bit_pos--;

    return full;
}

// uint8_t bitstream[BUFF_LEN] = {0};
int stream_pos = 0;
// store byte in array of byte 
uint8_t _byte_store(uint8_t byte, uint8_t bitstream[_BIT_STORE_BUFF_LEN])
{
    bitstream[stream_pos] = byte;
    stream_pos++;
    uint8_t full = (stream_pos == _BIT_STORE_BUFF_LEN);
    stream_pos *= !full;
    return full;
}

// store bit in bitstream[]
uint8_t bit_store(uint8_t bit, uint8_t bitstream[_BIT_STORE_BUFF_LEN])
{
    if (_bit_store(bit) == 1)
    {
        if (_byte_store(bit_cache, bitstream) == 1)
        {
            return 1;
        }
    }
}