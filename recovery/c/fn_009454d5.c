#include "cmre.h"

u32 CM_CDECL cm_fn_009454d5(u32 first, u32 second, u32 count)
{
    if (first != second) {
        while (count != 0) {
            u8 from_second = *(volatile u8 *)second;
            u8 from_first = *(volatile u8 *)first;
            *(volatile u8 *)first = from_second;
            ++first;
            *(volatile u8 *)second = from_first;
            ++second;
            --count;
        }
    }
    return first;
}
