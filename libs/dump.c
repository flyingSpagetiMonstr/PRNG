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
   else
   {
        printf("Failed to dump into %s, maybe the route doesn't exist.\n", filename);
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
    else
    {
        printf("Failed to load from %s, maybe the route doesn't exist.\n", filename);
    }
}
