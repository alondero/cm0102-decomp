/* Preferred-image VA 0069eb50, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

extern u8 CM_CDECL cm_fn_00544820(u32 *object);
extern u32 CM_CDECL cm_fn_0053b900(u32 *object);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 object);

u8 CM_THISCALL cm_fn_0069eb50(u32 self, u32 object)
{
    u32 value, index, base, high;

    if (object == 0)
        return 0;
    if (cm_fn_00544820((u32 *)object) == 0)
        return 0;
    if (cm_fn_0053b900((u32 *)object) != 0)
        return 0;
    base = D(0x00ae2a38u);
    value = cm_fn_005a0590(base, object);
    index = D(object);
    base = D(self);
    if ((s16)W(base + index * 73u + 8u) >= 3000)
        return 0;
    if (value == 0)
        return 0;
    high = D(value + 4u);
    if ((s32)high > 0)
        return 0;
    if ((s32)high < 0)
        return 1;
    /* CMP DWORD [EAX],0 / JAE always takes the exit, after the read. */
    (void)D(value);
    return 0;
}
