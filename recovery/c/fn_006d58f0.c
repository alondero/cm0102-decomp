#include "cmre.h"

u8 CM_THISCALL cm_fn_006d58f0(volatile u8 *self)
{
    s8 first = (s8)self[0x8ea8];
    s8 second = (s8)self[0x8ea7];
    if (second >= 2 && second <= 6) {
        if (first >= 0 && first <= 1)
            return 1;
        if (first >= 10 && first <= 11)
            return 0xff;
    }
    return 0;
}
