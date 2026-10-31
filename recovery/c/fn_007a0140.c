#include "cmre.h"

extern void CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                   void *buffer1, void *buffer2);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(const char *message, const char *buffer, int mode);
extern void CM_THISCALL cm_fn_00687430(void *self, u8 index_value, u32 entry,
                                       u32 zero1, u32 zero2);
extern void CM_FASTCALL cm_fn_00684230(void *self);
extern u32 CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_THISCALL cm_fn_007a0140(volatile u8 *self)
{
    volatile u8 scratch[0x204];
    u32 allocation;

    *(volatile s16 *)(self + 0x3eu) = 4;
    allocation = cm_fn_00944e46(0xecu);
    *(volatile u32 *)(self + 0xb1u) = allocation;
    if (allocation == 0u) {
        cm_fn_00944cff(0x00a6a360u, 0u, 0u,
                       (void *)(scratch + 0x104u), (void *)(scratch + 4u));
        {
            u32 message = (u32)(unsigned long)cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, message,
                           (void *)(scratch + 0x104u), (void *)(scratch + 4u), 0x1e3u);
        }
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
        return 0u;
    }

    {
        u8 index = 0u;
        s16 initial_count = *(volatile s16 *)(self + 0x3eu);
        *(volatile u8 *)scratch = 0u;
        if (initial_count > 0) {
            do {
                u32 table = *(volatile u32 *)(self + 0xeeu);
                s8 signed_index = (s8)index;
                u32 entry = *(volatile u32 *)(table + (u32)(s32)signed_index * 4u);
                u8 helper_index = *(volatile u8 *)scratch;
                cm_fn_00687430((void *)self, helper_index, entry, 0u, 0u);
                index = (u8)(index + 1u);
                s16 count = *(volatile s16 *)(self + 0x3eu);
                *(volatile u8 *)scratch = index;
                if ((s16)(s8)index >= count)
                    break;
            } while (1);
        }
    }

    cm_fn_00684230((void *)self);
    return 1u;
}
