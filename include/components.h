#ifndef INCLUDE_COMPONENTS_H
#define INCLUDE_COMPONENTS_H

#include "PRNG.h"

// ==================================
#if N == 32
#define TWO_N 64
#define _VALUE 0xaabbccdd11223344
#define Y_MASK 0x00000000ffffffff

#elif N == 16
#define TWO_N 32
#define _VALUE 0xaabbccdd
#define Y_MASK 0x0000ffff
#endif


#endif // INCLUDE_COMPONENTS_H