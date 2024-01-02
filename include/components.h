#ifndef INCLUDE_COMPONENTS_H
#define INCLUDE_COMPONENTS_H

#include "PRNG.h"

// ==================================
#if N == 32
#define TWO_N 64
#define _VALUE 0xaabbccdd11223344
#define Y_MASK 0x00000000ffffffff

#define TO_N TO_32 // N-bit to log_{2}{N}-bit
#define TO_2N TO_64 // N-bit to log_{2}{2*N}-bit

// different (L, R) shifts for N-bit and 2N-bit
#define SHIFT_N RSHIFT_32
#define SHIFT_2N LSHIFT_64
#elif N == 16
#define TWO_N 32
#define _VALUE 0xaabbccdd
#define Y_MASK 0x0000ffff
#define TO_N TO_16
#define TO_2N TO_32
#define SHIFT_N LSHIFT_16
#define SHIFT_2N RSHIFT_32
#endif

// ==================================

#define COUPON_32 129.871846 
// #define COUPON 150.284131 /*coupon(36)*/ 
// #define COUPON 214.022264 /*coupon(48)*/ 
// #define COUPON 303.609018 /*coupon(64)*/

// x(64, DEMAND) # 52.49300449925992
// ==================================

// 32-bit to 6-bit
#define TO_64(x) ((x)>>(32-6))
// cyclic lshift of uint64_t
#define LSHIFT_64(x, n) (((x)<<(n))^((x)>>(64-(n))))

// to 5-bit
#define TO_32(x) ((x)&0b11111)
// cyclic rshift of uint32_t
#define RSHIFT_32(x, n) (((x)>>(n))^((x)<<(32-(n))))

// 16-bit to 4-bit
#define TO_16(x) ((x)>>(16-4))
// cyclic lshift of uint16_t
#define LSHIFT_16(x, n) (((x)<<(n))^((x)>>(16-(n))))

#endif // INCLUDE_COMPONENTS_H