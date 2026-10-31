#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                  u32 buffer1, u32 buffer2);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(const char *message, const char *buffer,
                                   int mode);
extern void CM_THISCALL cm_fn_00687430(u32 self, u32 arg1, u32 arg2,
                                      u32 arg3, u32 arg4);
extern void CM_FASTCALL cm_fn_00684230(u32 self);

u32 CM_THISCALL cm_fn_00928060(u32 self)
{
    volatile u8 scratch[0x204];
    u32 allocation;

    *(volatile u16 *)(self + 0x3eu) = 4u;
    allocation = (u32)(unsigned long)cm_fn_00944e46(0xecu);
    *(volatile u32 *)(self + 0xb1u) = allocation;
    if (allocation != 0u) {
        s16 count = (s16)*(volatile u16 *)(self + 0x3eu);
        u8 index = 0u;
        *(volatile u8 *)&scratch[0] = index;
        if (count > 0) {
            do {
                u32 table = *(volatile u32 *)(self + 0xeeu);
                u32 local_value = *(volatile u32 *)&scratch[0];
                u32 row = *(volatile u32 *)(table + (s32)(s8)index * 8u);
                s16 live_count;
                cm_fn_00687430(self, local_value, row, 0u, 0u);
                table = *(volatile u32 *)(self + 0xeeu);
                index = (u8)(index + 1u);
                *(volatile u32 *)(table + (s32)(s8)(u8)(index - 1u) * 8u) = 0u;
                live_count = (s16)*(volatile u16 *)(self + 0x3eu);
                *(volatile u8 *)&scratch[0] = index;
                if ((s16)(s8)index >= live_count)
                    break;
            } while (1);
        }
        cm_fn_00684230(self);
        return 1u;
    }

    (void)cm_fn_00944cff(0x00ad572cu, 0u, 0u,
                         (u32)(unsigned long)&scratch[0x104],
                         (u32)(unsigned long)&scratch[4]);
    allocation = (u32)(unsigned long)cm_fn_0090d130();
    (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, allocation,
                         (u32)(unsigned long)&scratch[0x104],
                         (u32)(unsigned long)&scratch[4], 0x181u);
    cm_fn_005e8290((const char *)0x009870e0u,
                   (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    return 0u;
}
