/* Preferred-image VA 0x00896570, base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   thiscall, ret 8. Signed word index; all address arithmetic wraps at 32 bits.
   Scratch L is entry ESP-0x200, buffers L and L+0x100. */
#include "cmre.h"

extern u32 cm_fn_00944cff(u32 source, u32 a, u32 b, void *first, void *second);
extern char *cm_fn_0090d130(void);
extern u32 cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void cm_fn_005e8290(const char *title, const char *text, int flags);

u32 CM_THISCALL cm_fn_00896570(u32 self, s16 index, u32 first_only)
{
    u8 scratch[0x200];
    if (index >= 0) {
        u32 first = *(volatile u32 *)(self + 0x5fdu);
        u32 total = first;
        if (first_only == 0)
            total = *(volatile u32 *)(self + 0x5f5u) + first;
        if ((s32)index < (s32)total) {
            u32 offset = (u32)(s32)index;
            u32 base;
            if ((s32)offset < (s32)first) {
                offset *= 0x18e3u;
                base = *(volatile u32 *)(self + 0x5f9u);
            } else {
                offset = (offset - first) * 0x18e3u;
                base = *(volatile u32 *)(self + 0x5f1u);
            }
            return offset + base;
        }
    }
    cm_fn_00944cff(0x00a8b908u, 0, 0, scratch + 0x100, scratch);
    {
        char *text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                       scratch + 0x100, scratch, 0x265u);
    }
    cm_fn_005e8290((const char *)0x009870e0u,
                   (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
