#include "utils.h"
#include <string.h>
#include <iostream>
extern "C" {
#include "hsc_include.h"
}

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

bool get_pnt_list(int heap, int list_idx, std::deque<int32_t> &list)
{
    if (list.size() > 0)
        return true;

    void *addr = shitol(heap, list_idx);
    int32_t size = SHMSZ(heap, addr);
    if (size < 1)
        return false;

    size /= 4;
    int32_t *items = reinterpret_cast<int32_t*>(addr);
    int idx;
    std::cout << "list items: " << size << std::endl;
    for (idx=0 ; idx < size ; ++idx)
        std::cout << items[idx] << " ";
    std::cout << std::endl;
    //for (idx=0 ; idx < size ; ++idx)
    for (idx=0 ; items[idx] > 0 ; ++idx)
        list.push_back(items[idx]);

    return true;
}
