/* WIP: error-path memory mismatch after three failed invocations; freeze for review. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_00944cf1(u32 size);
extern void CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2, u32 buffer1, u32 buffer2);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void cm_fn_005e8290(u32 message, u32 buffer, u32 mode);

u32 CM_THISCALL cm_fn_008ae180(u32 self, u16 count_arg)
{
    volatile u8 scratch[0x200];
    *(volatile u16 *)(self + 4u) = count_arg;
    *(volatile u32 *)(self + 0x4b6u) = 0u;

    u32 i;
    for (i = 0u; i < 0x14u; ++i) {
        *(volatile u32 *)(self + 0x2d8u + i * 4u) = 0xffffffffu;
        *(volatile u32 *)(self + 0x288u + i * 4u) = 0xffffffffu;
    }

    s32 signed_count = *(volatile s16 *)(self + 4u);
    u32 allocation = cm_fn_00944cf1(((u32)signed_count << 2));
    *(volatile u32 *)self = allocation;
    if (allocation == 0u) {
        cm_fn_00944cff(0x00a8c6d4u, 0u, 0u, (u32)&scratch[0x100], (u32)&scratch[0]);
        {
            u32 text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                           (u32)&scratch[0x100], (u32)&scratch[0], 0x7cu);
        }
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0u);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    }

    s32 index = 0;
    if ((s16)*(volatile u16 *)(self + 4u) > 0) {
        for (;;) {
            u32 data = *(volatile u32 *)self;
            s32 signed_index = (s16)(u16)index;
            ++index;
            *(volatile u32 *)(data + (u32)signed_index * 4u) = 0xffffffffu;
            if ((s16)(u16)index >= *(volatile s16 *)(self + 4u))
                break;
        }
    }
    return self;
}




