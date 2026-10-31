/* Preferred-image VA 0x00925B30; image base 0x00400000; pinned target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern void cm_fn_005e8290(const char *, const char *, int);
extern int CM_THISCALL cm_fn_00682f70(u32 self, u32 value);
extern void CM_THISCALL cm_fn_00687430(u32 self, u8 index, u32 row,
                                        u32 zero1, u32 zero2);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_00944cff(u32 message, u32 zero1, u32 zero2,
                                    void *buffer1, void *buffer2);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_THISCALL cm_fn_00925b30(u32 self)
{
    volatile u8 scratch[0x200];
    u32 allocation;
    u8 matches = 0;
    u32 index = 0;

    *(volatile u16 *)(self + 0x3e) = 0x12;
    allocation = (u32)cm_fn_00944e46(0x426);
    *(volatile u32 *)(self + 0xb1) = allocation;
    if (allocation == 0) {
        u32 message;
        (void)cm_fn_00944cff(0x00ad56b8, 0, 0,
                             (void *)&scratch[0x100], (void *)&scratch[0]);
        message = (u32)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                             message, (u32)&scratch[0x100],
                             (u32)&scratch[0], 0x194);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return 0;
    }

    {
        s32 global_count = CM_GLOBAL(volatile s32, 0x00ae235c);
        u32 row = CM_GLOBAL(volatile u32, 0x00ae23b4);

        if (global_count > 0) {
            for (;;) {
                u32 pointer = *(volatile u32 *)(row + 0x57);
                if (pointer != 0) {
                    u32 pointed_value = *(volatile u32 *)pointer;
                    u32 compare_value = CM_GLOBAL(volatile u32, 0x009cf94c);
                    if (pointed_value == compare_value) {
                        u8 call_index = matches;
                        ++matches;
                        cm_fn_00687430(self, call_index, row, 0, 0);
                        if ((s16)(s8)matches ==
                            *(volatile s16 *)(self + 0x3e))
                            break;
                    }
                }
                global_count = CM_GLOBAL(volatile s32, 0x00ae235c);
                ++index;
                row += 0x245;
                if ((s32)index >= global_count)
                    break;
            }
        }
    }

    if ((s16)(s8)matches != *(volatile s16 *)(self + 0x3e))
        return 0;

    *(volatile u32 *)(self + 0x28) =
        (u32)cm_fn_00682f70(self, 0x2710);
    *(volatile u8 *)(self + 0xe1) = 1;
    return 1;
}
