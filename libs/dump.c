#include "dump.h"

#include <stdio.h>
#include <string.h>

int dump(void* x, char* filename, int len)
{
   FILE* dst = fopen(filename, "wb");
   if (dst != NULL)
   {
        fwrite(x, len, 1, dst);
        fclose(dst);
        return 1;
   }
   else
   {
        return 0;
        // printf("Failed to dump into %s, maybe the route doesn't exist.\n", filename);
   }
}

int load(void* x, char* filename, int len)
{
    FILE* src = fopen(filename, "rb");
    if (src != NULL)
    {
        fread(x, len, 1, src);
        fclose(src);
        return 1;
    }
    else
    {
        return 0;
        // printf("Failed to load from %s, maybe the route doesn't exist.\n", filename);
    }
}
