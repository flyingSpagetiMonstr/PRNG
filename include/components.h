#ifndef INCLUDE_COMPONENTS_H
#define INCLUDE_COMPONENTS_H

// ==================================
// for funtion `phi`: 

//! \defgroup converts "x" to [0, n)
#define TO_32(x) ((x)&0b11111)
#define TO_02(x) ((x>>5)&0b1)
#define TO_X32(x) (((x)>>(6))&0b11111)
#define TO_03(x) ((x)%3)
// #define TO_04(x) ((x)&0b11)

// cyclic rshift of uint32_t
#define RSHIFT(x, n) (((x)>>(n))^((x)<<(32-(n))))

#endif // INCLUDE_COMPONENTS_H