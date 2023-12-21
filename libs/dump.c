#include "dump.h"

#include <stdio.h>
#include <string.h>

void dump(void* x, char* filename, int len)
{
   FILE* dst = fopen(filename, "wb");
   if (dst != NULL)
   {
        fwrite(x, len, 1, dst);
        fclose(dst);
   }
}

void load(void* x, char* filename, int len)
{
    FILE* src = fopen(filename, "rb");
    if (src != NULL)
    {
        fread(x, len, 1, src);
        fclose(src);
    }
}
