#include "cmre.h"
extern void CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2, u32 buffer1, u32 buffer2);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void cm_fn_005e8290(u32 message, u32 buffer, u32 mode);

u32 CM_THISCALL cm_fn_008b4cd0(u32 self, u32 value_ptr)
{
    volatile u8 scratch[0x200];
    u32 delta;
    if (value_ptr == 0u) {
        cm_fn_00944cff(0x00a8ccd0u, 0u, 0u, (u32)&scratch[0x100], (u32)&scratch[0]);
        {
            u32 text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                           (u32)&scratch[0x100], (u32)&scratch[0], 0xa20u);
        }
    } else {
        u32 current = CM_GLOBAL(volatile u32, 0x00ae2364u);
        u32 pointed = *(volatile u32 *)value_ptr;
        delta = current - pointed - 1u;
        if ((s32)delta >= 0 && (s32)delta < 0x10)
            return self + delta * 0x36u + 0x652du;
        cm_fn_00944cff(0x00a8ccd0u, 0u, 0u, (u32)&scratch[0], (u32)&scratch[0x100]);
        {
            u32 text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                           (u32)&scratch[0], (u32)&scratch[0x100], 0xa2au);
        }
    }
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0u);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    return 0u;
}



