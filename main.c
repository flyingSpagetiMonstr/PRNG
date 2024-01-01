#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "PRNG.h"

#define MILLION 1000000
#define DUMP_FILE "dumps/state.dat" // where the generated bits will be stored

// ===================================================
// options:
#define OUTPUT "sts-2.1.2/data/stream.dat" // where the generated bits will be stored
#define STREAM_LEN (MILLION*1000) // required stream length (by bit)
#define TIME 1 // whether to calculate the time cost
// #define PASS_TO_STDOUT
// ===================================================

#ifdef PASS_TO_STDOUT
#define YIELD(byte) fwrite(&byte, 1, 1, stdout)
// "disable" printing to console:
#define puts(x)
#define printf(...)
#else
#define YIELD(byte) fwrite(&byte, 1, 1, out_file)
#endif

int main()
{
    uint8_t byte = 0; 
    uint32_t cnt = 0, byte_n = STREAM_LEN/8; // generating one byte per round
    clock_t start_time = 0 , end_time = 0;
    FILE *out_file = NULL;

    out_file = fopen(OUTPUT, "wb");

    if (!load_state(DUMP_FILE)) printf("Failed to load from %s\n", DUMP_FILE);
    strength = NIST;
    printf("Config info: \n 1.load_state from: %s\n 2.strength: %d\n", DUMP_FILE, strength);

    puts("Running...");

    if(TIME) start_time = clock();
    // =======================================
    for (cnt = 1; cnt <= byte_n; cnt++)
    {
        byte = generator();
        YIELD(byte);
    }
    // =======================================
    if(TIME) end_time = clock();

    puts("FIN");

    if(TIME) printf("Time cost: %.0f milli seconds\n", ((double)(1000*(end_time - start_time))) / CLOCKS_PER_SEC);
    if(!dump_state(DUMP_FILE)) printf("Failed to dump into %s\n", DUMP_FILE);
    fclose(out_file);
}
