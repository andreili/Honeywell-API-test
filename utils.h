#ifndef UTILS_H
#define UTILS_H

#include <inttypes.h>

void __swab(uint8_t *src, uint8_t *dst, int len);
void __intchr (char* dst, short sizeofvalue, char* src, int size_masked);
int ftocstr(char *ds, int dl, char *ss, int sl);

#endif // UTILS_H
