#ifndef INCLUDE_BITSTORE_H
#define INCLUDE_BITSTORE_H

#include <stdint.h>

// --------------------------------------
#include "PRNG.h"
#define _BIT_STORE_BUFF_LEN LEN
// --------------------------------------

// extern uint8_t bitstream[BUFF_LEN];
uint8_t bit_store(uint8_t bit, uint8_t bitstream[_BIT_STORE_BUFF_LEN]);

#endif // INCLUDE_BITSTORE_H