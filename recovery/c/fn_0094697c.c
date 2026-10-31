/* Preferred-image VA 0x0094697c, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
typedef u32 (CM_STDCALL *convert_fn)(u32 input, u32 output);
extern s32 CM_CDECL cm_fn_0094c490(u32 year, u32 month, u32 day, u32 hour,
                                   u32 minute, u32 second, u32 flag);

s32 CM_CDECL cm_fn_0094697c(u32 input)
{
    u8 scratch[24];
    u32 second, minute, hour, day, month, year;
    if (!D(input) && !D(input + 4u)) return -1;
    if (!((convert_fn)D(0x0096718cu))(input, (u32)(scratch + 16))) return -1;
    if (!((convert_fn)D(0x00967188u))((u32)(scratch + 16), (u32)scratch)) return -1;
    second = *(volatile u16 *)(scratch + 12);
    minute = *(volatile u16 *)(scratch + 10);
    hour = *(volatile u16 *)(scratch + 8);
    day = *(volatile u16 *)(scratch + 6);
    month = *(volatile u16 *)(scratch + 2);
    year = *(volatile u16 *)scratch;
    return cm_fn_0094c490(year, month, day, hour, minute, second, 0xffffffffu);
}
