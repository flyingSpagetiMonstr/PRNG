#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "PRNG.h"

#define MILLION 1000000UL
#define DUMP_FILE "dumps/state.dat" // where the generated bits will be stored

// ===================================================
// options:
#define OUTPUT "testscripts/data/stream.dat" // where the generated bits will be stored
#define STREAM_LEN (MILLION*1UL) // required stream length (by bit)
#define TIME 1 // whether to calculate the time cost
// #define PASS_TO_STDOUT
// ===================================================

#ifdef PASS_TO_STDOUT
#define YIELD(out) fwrite(&out, sizeof(out), 1, stdout)
// "disable" printing to console:
#define puts(x)
#define printf(...)
#else
#define YIELD(out) fwrite(&out, sizeof(out), 1, out_file)
#endif

int main()
{
    out_t out = 0; 
    uint64_t cnt = 0, out_n = STREAM_LEN/N; // generating 32 bit per round
    clock_t start_time = 0 , time_cost = 0;
    FILE *out_file = NULL;

    out_file = fopen(OUTPUT, "wb");
    if (out_file == NULL) printf("Failed to open output file: %s\n", OUTPUT);

    if (!load_state(DUMP_FILE)) printf("Failed to load from %s\n", DUMP_FILE);
    // rand_state();
    strength = NIST;
    
    printf("=============================================\n"
        "Info: \n"
        " 0.STREAM_LEN: %lu MILLION bits\n"
        " 1.load_state from: %s\n"
        " 3.state size: %d-bit (type: uint%d_t)\n"
        " 4.TIME: %d, YIELD: %d\n",
        STREAM_LEN/MILLION, DUMP_FILE, 2*N, 2*N, TIME, !TIME);

    puts("Running...");

    // =======================================
    if(TIME) start_time = clock();
    for (cnt = 1; cnt <= out_n; cnt++)
    {
        out = generator();
        if (!TIME) YIELD(out);
    }
    if(TIME) time_cost = clock() - start_time;
    // =======================================

    puts("FIN");

    if(TIME) printf("Time cost: %.0f milli seconds\n", ((double)(1000*(time_cost))) / CLOCKS_PER_SEC);
    if(!dump_state(DUMP_FILE)) printf("Failed to dump into %s\n", DUMP_FILE);
    fclose(out_file);
}
