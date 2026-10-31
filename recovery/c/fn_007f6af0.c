/* Preferred-image VA 0x007f6af0, base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   thiscall, ret 8. The successful store uses incoming ECX.
   Scratch L is entry ESP-0x200; the two error paths swap L and L+0x100. */
#include "cmre.h"

extern u32 cm_fn_00944cff(u32 source, u32 a, u32 b, void *first, void *second);
extern char *cm_fn_0090d130(void);
extern u32 cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void cm_fn_005e8290(const char *title, const char *text, int flags);

u32 CM_THISCALL cm_fn_007f6af0(u32 self, u32 value, u32 record)
{
    u8 scratch[0x200];
    void *first;
    void *second;
    u32 line;
    char *text;
    if (record == 0) {
        u32 index = (u32)(s32)CM_GLOBAL(volatile s16, 0x00b77c7a);
        record = CM_GLOBAL(volatile u32, 0x00b74c26u + index * 0x300u);
    }
    if (record == 0) {
        cm_fn_00944cff(0x00a7b094u, 0, 0, scratch + 0x100, scratch);
        first = scratch + 0x100;
        second = scratch;
        line = 0xb8u;
    } else {
        u8 field = *(volatile u8 *)record;
        u8 global = CM_GLOBAL(volatile u8, 0x00ae2364);
        u32 index = (u8)(field - global + 16u);
        if (index < 16u) {
            *(volatile u32 *)(self + 0xcu + index * 4u) = value;
            return 1;
        }
        cm_fn_00944cff(0x00a7b094u, 0, 0, scratch, scratch + 0x100);
        first = scratch;
        second = scratch + 0x100;
        line = 0xc2u;
    }
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0u,
                   (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
