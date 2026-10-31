#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u32 CM_CDECL cm_fn_0094e5d8(u32 key)
{
    u32 count = D(0x00ad95d0);
    u32 first = D(0x00ad9550);
    u32 position = 0x00ad9550;
    u32 end = 0x00ad9550u + count * 12u;
    if (first != key) {
        do {
            position += 12u;
            if (position >= end) break;
        } while (D(position) != key);
    }
    if (position >= end) return 0;
    if (D(position) == key) return position;
    return 0;
}
