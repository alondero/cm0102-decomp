/* Preferred-image VA 0x0092D0F0; image base 0x00400000; pinned target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern void cm_fn_005e8290(const char *, const char *, int);
extern void CM_FASTCALL cm_fn_00684230(u32 self);
extern void CM_THISCALL cm_fn_00687430(u32 self, u8 raw_index, u32 row,
                                        u32 zero1, u32 zero2);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_00944cff(u32 message, u32 zero1, u32 zero2,
                                    void *buffer1, void *buffer2);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_THISCALL cm_fn_0092d0f0(u32 self, u32 records)
{
    volatile u8 scratch[0x204];
    u32 allocation;

    *(volatile u16 *)(self + 0x3e) = 4;
    allocation = (u32)cm_fn_00944e46(0xec);
    *(volatile u32 *)(self + 0xb1) = allocation;
    if (allocation == 0) {
        u32 message;
        (void)cm_fn_00944cff(0x00ad5888, 0, 0,
                             (void *)&scratch[0x104],
                             (void *)&scratch[4]);
        message = (u32)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                             message, (u32)&scratch[0x104],
                             (u32)&scratch[4], 0x2b1);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return 0;
    }

    {
        u8 scan_index = 0;
        u8 matches = 0;
        scratch[3] = scan_index;
        for (;;) {
            s8 signed_matches = (s8)matches;
            s8 signed_scan;
            u32 entry;
            u8 flag;

            if (signed_matches >= 4)
                break;
            entry = records + (u32)(s32)(s8)scan_index * 5u;
            flag = *(volatile u8 *)(entry + 4);
            if (flag == 1) {
                u32 id = *(volatile u32 *)entry;
                u32 base = CM_GLOBAL(volatile u32, 0x00ae23b4);
                u32 row = base + id + id * 580u;
                u8 call_index = matches;
                ++matches;
                cm_fn_00687430(self, call_index, row, 0, 0);
                scan_index = scratch[3];
            }
            scan_index = (u8)(scan_index + 1u);
            scratch[3] = scan_index;
            signed_scan = (s8)scan_index;
            if (signed_scan >= 0x20)
                break;
        }
    }

    cm_fn_00684230(self);
    return 1;
}
