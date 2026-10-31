/* Preferred-image VA 0x00908560; image base 0x00400000; pinned target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern void cm_fn_005e8290(const char *, const char *, int);
extern void CM_THISCALL cm_fn_00687430(u32 self, u32 raw_index, u32 row,
                                        u32 zero1, u32 zero2);
extern u32 CM_CDECL cm_fn_00909ec0(u32 row);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_00944cff(u32 message, u32 zero1, u32 zero2,
                                    void *buffer1, void *buffer2);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_THISCALL cm_fn_00908560(u32 self)
{
    volatile u8 scratch[0x200];
    u32 allocation;
    u32 ebx = 0;
    u32 ebp = 0;

    *(volatile u16 *)(self + 0x3e) = 4;
    allocation = (u32)cm_fn_00944e46(0xec);
    *(volatile u32 *)(self + 0xb1) = allocation;
    if (allocation == 0) {
        u32 message;
        (void)cm_fn_00944cff(0x00aa1fb4, 0, 0,
                             (void *)&scratch[0x100], (void *)&scratch[0]);
        message = (u32)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                             message,
                             (u32)&scratch[0x100], (u32)&scratch[0], 0x2fe);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return 0;
    }

    {
        s32 global_count = CM_GLOBAL(volatile s32, 0x00ae235c);
        u32 row = CM_GLOBAL(volatile u32, 0x00ae23b4);
        if (global_count <= 0)
            goto success;

        for (;;) {
            u16 count = *(volatile u16 *)(self + 0x3e);
            s16 signed_count = (s16)count;
            s16 signed_bx = (s16)(u16)ebx;
            s16 signed_bp;
            u32 helper_eax;
            u32 raw_index;

            if (signed_bx >= signed_count)
                break;
            if (*(volatile u32 *)(row + 0x57) ==
                *(volatile u32 *)(self + 4)) {
                helper_eax = cm_fn_00909ec0(row);
                if ((u16)helper_eax == 0x413) {
                    raw_index = (helper_eax & 0xffffff00u) | (u8)ebx;
                    ++ebx;
                    cm_fn_00687430(self, raw_index, row, 0, 0);
                }
            }

            global_count = CM_GLOBAL(volatile s32, 0x00ae235c);
            ++ebp;
            signed_bp = (s16)(u16)ebp;
            row += 0x245;
            if (signed_bp >= global_count)
                break;
        }
    }

success:
    return (u16)ebx == *(volatile u16 *)(self + 0x3e);
}
