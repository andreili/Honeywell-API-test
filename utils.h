#ifndef UTILS_H
#define UTILS_H

#include <inttypes.h>
#include <deque>

void __swab(uint8_t *src, uint8_t *dst, int len);
void __intchr (char* dst, short sizeofvalue, char* src, int size_masked);
int ftocstr(char *ds, int dl, char *ss, int sl);

bool get_pnt_list(int heap, int list_idx, std::deque<int32_t> &list);

#endif // UTILS_H
