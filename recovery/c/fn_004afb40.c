/* Preferred-image VA 0x004afb40, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0090cfc0(u32 limit);

u32 CM_CDECL cm_fn_004afb40(u32 array, u32 raw_count)
{
    s32 count = (s16)raw_count;
    u32 limit = (u32)(count * 3);
    u16 iteration = 0;
    if ((s32)limit > 0) {
        do {
            u32 first = cm_fn_0090cfc0((u32)count);
            u32 second = cm_fn_0090cfc0((u32)count);
            u32 second_addr = array + (u32)(s32)(s16)second * 4u;
            u32 first_addr = array + (u32)(s32)(s16)first * 4u;
            u32 second_value = D(second_addr);
            u32 first_value = D(first_addr);
            ++iteration;
            D(first_addr) = second_value;
            D(second_addr) = first_value;
        } while ((s32)(s16)iteration < (s32)limit);
    }
    return limit;
}
