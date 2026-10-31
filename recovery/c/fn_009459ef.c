#include "cmre.h"

unsigned long long CM_CDECL __attribute__((regparm(2)))
cm_fn_009459ef(u32 incoming_eax, u32 incoming_edx,
               u32 begin, volatile u32 end)
{
    u32 cursor = begin;
    unsigned long long result = ((unsigned long long)incoming_edx << 32) | incoming_eax;

    while (cursor < end) {
        u32 target = *(volatile u32 *)cursor;
        result = (result & 0xffffffff00000000ull) | target;
        if (target != 0u)
            result = ((unsigned long long (CM_CDECL *)(void))target)();
        cursor += 4u;
    }
    return result;
}
