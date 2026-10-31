#include "cmre.h"

extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in, const u8 *src, s16 count);

u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_007779b0(
    u32 incoming_eax, u32 incoming_edx, u32 self, u32 first, u32 second)
{
    volatile u32 record[2];
    (void)incoming_edx;
    if (first == 0) return incoming_eax;
    if (second == 0) return 0;
    record[1] = second;
    record[0] = first;
    return (u32)cm_fn_0068d0d0((volatile u8 *)(self + 0x28u), first, (const u8 *)record, 8);
}
