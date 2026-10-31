#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                  u32 buffer1, u32 buffer2);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(const char *message, const char *buffer,
                                   int mode);

u32 CM_THISCALL cm_fn_00915d40(u32 self)
{
    volatile u8 scratch[0x200];
    u16 count = (u16)(0x18 - (s32)*(volatile s8 *)(self + 0xbbu));
    u32 bytes = (u32)((s32)(s16)count * 6);
    u32 allocation;
    u32 index;

    *(volatile u16 *)(self + 0x3eu) = count;
    allocation = (u32)(unsigned long)cm_fn_00944e46(bytes);
    *(volatile u32 *)(self + 0xa7u) = allocation;
    if (allocation != 0u) {
        index = 0u;
        if ((s16)*(volatile u16 *)(self + 0x3eu) > 0) {
            do {
                u32 source = *(volatile u32 *)(self + 0xb2u);
                u32 destination = *(volatile u32 *)(self + 0xa7u);
                u32 value = *(volatile u32 *)(source + index * 4u);
                *(volatile u32 *)(destination + index * 6u) = value;
                destination = *(volatile u32 *)(self + 0xa7u);
                *(volatile u8 *)(destination + index * 6u + 4u) = 0u;
                destination = *(volatile u32 *)(self + 0xa7u);
                *(volatile u8 *)(destination + index * 6u + 5u) = 0u;
                ++index;
            } while ((s32)index < (s32)(s16)*(volatile u16 *)(self + 0x3eu));
        }
        return 1u;
    }

    (void)cm_fn_00944cff(0x00ad4850u, 0u, 0u,
                         (u32)(unsigned long)&scratch[0x100],
                         (u32)(unsigned long)&scratch[0]);
    allocation = (u32)(unsigned long)cm_fn_0090d130();
    (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, allocation,
                         (u32)(unsigned long)&scratch[0x100],
                         (u32)(unsigned long)&scratch[0], 0x350u);
    cm_fn_005e8290((const char *)0x009870e0u,
                   (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    return 0xffffffffu;
}
