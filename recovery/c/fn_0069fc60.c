/* Preferred-image VA 0x0069fc60, base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ret 4, receiver in ECX. All six direct callers test AL only.
   Scratch L is entry ESP-0x200; buffers are L and L+0x100. */
#include "cmre.h"

extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 self);
extern u32 cm_fn_00944cff(u32 source, u32 a, u32 b, void *first, void *second);
extern char *cm_fn_0090d130(void);
extern u32 cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void cm_fn_005e8290(const char *title, const char *text, int flags);

u8 CM_THISCALL cm_fn_0069fc60(u32 self, u32 item)
{
    u8 scratch[0x200];
    u32 list = *(volatile u32 *)(self + 8u);
    u32 first = *(volatile u32 *)(list + 2u);
    u32 saved = *(volatile u32 *)(list + 10u);
    u32 index = 0;
    *(volatile u32 *)(list + 10u) = first;
    list = *(volatile u32 *)(self + 8u);
    if (*(volatile s16 *)list > 0) {
        for (;;) {
            u32 node = cm_fn_0068d1b0(list);
            if (node == 0) {
                char *text;
                list = *(volatile u32 *)(self + 8u);
                *(volatile u32 *)(list + 10u) = saved;
                cm_fn_00944cff(0x009f18a4u, 0, 0,
                               scratch + 0x100, scratch);
                text = cm_fn_0090d130();
                cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                               scratch + 0x100, scratch, 0x3724u);
                cm_fn_005e8290((const char *)0x009870e0u,
                               (const char *)0x00ae24d0u, 0);
                CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
                return 0;
            }
            first = *(volatile u32 *)(node + 6u);
            if (first == *(volatile u32 *)item) {
                list = *(volatile u32 *)(self + 8u);
                *(volatile u32 *)(list + 10u) = saved;
                return 1;
            }
            list = *(volatile u32 *)(self + 8u);
            index += 1u;
            if ((s32)index >= (s32)*(volatile s16 *)list)
                break;
        }
    }
    list = *(volatile u32 *)(self + 8u);
    *(volatile u32 *)(list + 10u) = saved;
    return 0;
}
