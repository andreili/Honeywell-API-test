#include "utils.h"
#include <string.h>

void __swab(uint8_t *src, uint8_t *dst, int len)
{
    for (int i=0 ; i<len/2 ; ++i)
    {
        int idx = i * 2;
        dst[idx + 0] = src[idx + 1];
        dst[idx + 1] = src[idx + 0];
    }
}

void __intchr (char* dst, short sizeofvalue, char* src, int size_masked)
{
    int size = size_masked & 0xff;
    if (size >= 0)
    {
        if (size > sizeofvalue)
            size = sizeofvalue;

        if (size % 2)
        {
            __swab((uint8_t*)src, (uint8_t*)dst, size - 1);
            dst[size - 1] = src[size + 2];
        }
        else
            __swab((uint8_t*)src, (uint8_t*)dst, size);
        if (size < sizeofvalue)
            memset(&dst[size], ' ', sizeofvalue - size);
    }
}

int ftocstr(char *ds, int dl, char *ss, int sl)
{
    char *p;

    for (p = ss + sl; --p >= ss && *p == ' '; ) ;
    sl = p - ss + 1;
    dl--;
    ds[0] = 0;
    if (sl > dl)
        return 1;
    while (sl--)
        (*ds++ = *ss++);
    *ds = '\0';
    return 0;
}

