/* Preferred-image VA 0x0053ce90, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 person, u32 club);

u8 CM_CDECL cm_fn_0053ce90(u32 first, u32 second)
{
    u32 base, limit, value, result;
    if (!second) return 0;
    base = D(0x00ae2350u);
    limit = D(0x00ae235cu) - base * 2u;
    value = D(second);
    if ((s32)value >= (s32)limit) return 0;
    limit = D(0x00ae2364u);
    value = D(first);
    if ((s32)value >= (s32)limit) return 0;
    result = cm_fn_004cf5f0(D(0x00ae1050u), first, second);
    if (!result) return 0;
    return (u8)((B(result + 0x4fu) & 15u) - 10u);
}
