/* Preferred-image VA 0x00929AA0; image base 0x00400000; pinned target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern void cm_fn_005e8290(const char *, const char *, int);
extern void CM_FASTCALL cm_fn_00684230(u32 self);
extern void CM_THISCALL cm_fn_00687430(u32 self, u8 local_value, u32 row,
                                        u32 zero1, u32 zero2);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_00944cff(u32 message, u32 zero1, u32 zero2,
                                    void *buffer1, void *buffer2);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern void cm_fn_00945333(u32 *destination, u32 *source, u32 size, void *table);

u32 CM_THISCALL cm_fn_00929aa0(u32 self)
{
    volatile u8 scratch[0x204];
    u32 allocation;

    *(volatile u16 *)(self + 0x3e) = 4;
    allocation = (u32)cm_fn_00944e46(0xec);
    *(volatile u32 *)(self + 0xb1) = allocation;
    if (allocation == 0) {
        u32 message;
        (void)cm_fn_00944cff(0x00ad5764, 0, 0,
                             (void *)&scratch[0x104], (void *)&scratch[4]);
        message = (u32)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                             message, (u32)&scratch[0x104],
                             (u32)&scratch[4], 0x220);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return 0;
    }

    {
        u32 source = *(volatile u32 *)(self + 0xf2);
        u8 index = 0;
        s16 limit;

        cm_fn_00945333((u32 *)(*(volatile u32 *)(self + 0xee)),
                       (u32 *)source, 4,
                       (void *)0x0092b400u);
        limit = (s16)*(volatile u16 *)(self + 0x3e);
        scratch[0] = index;
        if (limit > 0) {
            do {
                s32 signed_index = (s8)index;
                u32 row_address = *(volatile u32 *)(self + 0xee) +
                                  (u32)(signed_index * 12);
                u32 row = *(volatile u32 *)row_address;
                u8 local_value = scratch[0];

                cm_fn_00687430(self, local_value, row, 0, 0);
                index = (u8)(index + 1u);
                limit = (s16)*(volatile u16 *)(self + 0x3e);
                scratch[0] = index;
            } while ((s16)(s8)index < limit);
        }
    }

    cm_fn_00684230(self);
    return 1;
}
