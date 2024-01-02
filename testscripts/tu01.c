#include "TestU01.h"
#include "PRNG.h"

#include <stdint.h>
#include <stdio.h>

#include <stdlib.h>

#define DUMP_FILE "../dumps/state.dat" // where the generated bits will be stored

uint32_t trans(void)
{
    uint8_t bytes[4];
    for (uint8_t i = 0; i < 4; i++)
    {
        bytes[i] = generator();
    }
    return *((uint32_t*)bytes);
}
    
int main()
{
    if (!load_state(DUMP_FILE)) printf("Failed to load from %s\n", DUMP_FILE);
    strength = BIG_CRUSH;
    
    unif01_Gen* gen = unif01_CreateExternGenBits("PRNG::generator", trans);

    puts("Running BigCrush..."); fflush(stdout);
    bbattery_BigCrush(gen);

    unif01_DeleteExternGenBits(gen);

    if(!dump_state(DUMP_FILE)) printf("Failed to dump into %s\n", DUMP_FILE);

    return 0;
}
