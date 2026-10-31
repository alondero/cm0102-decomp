#include "cmre.h"
extern void CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2, u32 buffer1, u32 buffer2);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 cm_fn_005e8290(u32 message, u32 buffer, u32 mode);

u32 CM_THISCALL cm_fn_008c1f80(u32 self, s32 index)
{
    volatile u8 scratch[0x200];
    u32 candidate;
    if (index < 0) {
        cm_fn_00944cff(0x00a8d514u, 0u, 0u, (u32)&scratch[0x100], (u32)&scratch[0]);
        {
            u32 text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                           (u32)&scratch[0x100], (u32)&scratch[0], 0xd91u);
        }
    } else if (index >= *(volatile s32 *)(self + 4u)) {
        cm_fn_00944cff(0x00a8d514u, 0u, 0u, (u32)&scratch[0], (u32)&scratch[0x100]);
        {
            u32 text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                           (u32)&scratch[0], (u32)&scratch[0x100], 0xd96u);
        }
    } else {
        u32 table = *(volatile u32 *)self;
        candidate = *(volatile u32 *)(table + (u32)index * 4u);
        if (candidate != 0u && *(volatile u8 *)(candidate + 0x1fu) != 1u) {
            u8 is_six = (u8)(*(volatile u8 *)(candidate + 0x2cu) == 6u);
            *(volatile u8 *)(candidate + 0x2cu) = (u8)(is_six + 7u);
        }
        return candidate;
    }
    u32 boundary_eax = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0u);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    return boundary_eax;
}



