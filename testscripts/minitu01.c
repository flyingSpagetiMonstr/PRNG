#include "TestU01.h"
#include "PRNG.h"

#include <stdint.h>
#include <stdio.h>

#include <stdlib.h>

#define DUMP_FILE "../dumps/state.dat" // where the generated bits will be stored
    
int main()
{
    if (!load_state(DUMP_FILE)) printf("Failed to load from %s\n", DUMP_FILE);
    // strength = CRUSH;
    
    unif01_Gen* gen = unif01_CreateExternGenBits("PRNG::mini", generator);

    puts("Running Crush for min..."); fflush(stdout);
    bbattery_Crush(gen);

    unif01_DeleteExternGenBits(gen);

    if(!dump_state(DUMP_FILE)) printf("Failed to dump into %s\n", DUMP_FILE);

    return 0;
}
