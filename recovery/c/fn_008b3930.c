#include "cmre.h"
/* Preferred-image VA 0x008b3930, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_008b3930(u32 self, u32 value, u32 kind)
{
    volatile u8 scratch[0x200];
    u32 current = CM_GLOBAL(volatile u32, 0x00ae2364);
    u32 pointed = *(volatile u32 *)value;
    u32 delta = current - pointed - 1u;
    u32 text, index;
    s8 selected;
    if ((s32)delta >= 0 && (s32)delta < 16) {
        selected = (s8)kind;
        if (selected < 0 || selected >= 10) selected = 1;
        index = (u32)(s32)selected + delta * 10u + 7u;
        return self + index * 155u;
    }
    cm_fn_00944cff(0x00a8ccd0, 0, 0,
                   (u32)(scratch + 0x100), (u32)scratch);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                   (u32)(scratch + 0x100), (u32)scratch, 0x68f);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
