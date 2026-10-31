#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00918dd0(u32 self, u8 mode);
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

u32 CM_THISCALL cm_fn_009190d0(u32 self)
{
    volatile u8 scratch[0x204];
    u16 count = (u16)cm_fn_00918dd0(self, 0xffu);
    s32 signed_count = (s32)(s16)count;
    s32 byte_count = (s32)(signed_count * 0x3b);
    u32 allocation;

    *(volatile u16 *)(self + 0x3eu) = count;
    allocation = (u32)(unsigned long)cm_fn_00944e46((u32)byte_count);
    *(volatile u32 *)(self + 0xb1u) = allocation;
    if (allocation == 0u) {
        (void)cm_fn_00944cff(0x00ad4a6cu, 0u, 0u,
                             (u32)(unsigned long)&scratch[0x104],
                             (u32)(unsigned long)&scratch[4]);
        {
            char *message = cm_fn_0090d130();
            (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                                 (u32)(unsigned long)message,
                                 (u32)(unsigned long)&scratch[0x104],
                                 (u32)(unsigned long)&scratch[4], 0x2b2u);
        }
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return 0u;
    }

    {
        count = *(volatile u16 *)(self + 0x3eu);
        u8 index = 0u;
        *(volatile u8 *)&scratch[0] = index;
        *(volatile u16 *)(self + 0xa7u) =
            (u16)(((u16)(count - 1u)) * *(volatile u16 *)(self + 0x3cu));
        if ((s16)count > 0) {
            do {
                u32 table = *(volatile u32 *)(self + 0xf7u);
                u32 row = *(volatile u32 *)(table + (s32)(s8)index * 4u);
                u32 local_value = *(volatile u32 *)&scratch[0];
                cm_fn_00687430(self, local_value, row, 0u, 0u);
                index = (u8)(index + 1u);
                s16 limit = (s16)*(volatile u16 *)(self + 0x3eu);
                *(volatile u8 *)&scratch[0] = index;
                if ((s16)(s8)index >= limit)
                    break;
            } while (1);
        }
    }
    cm_fn_00684230(self);
    return 1u;
}
