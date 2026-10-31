/* Preferred-image VA 005ab230, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Year upper16 is dead in 005aa2d0 and 005a9cf0: their full-width
   forwarding reaches 0054cfe0, which masks with 0xffff at0054cfe4.
   The comparisons/stores in both year callees use only WORD values. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_005aa2d0(u32, u16);
extern u32 CM_THISCALL cm_fn_005a9cf0(u32, u16, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u8 CM_CDECL cm_fn_0048dd70(u32, u32);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32, u32, s16);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005ab230(u32 self, u32 record)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x404];
    u32 previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095968e;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    u32 result, line;
    u8 *name, *extension;
    u16 year = W(record + 0x28);
    while (year != W(self + 0x1130)) {
        u32 next = D(self + 0x112c);
        if (next) {
            if (W(next + 0x1130) != year) cm_fn_005aa2d0(next, year);
        } else {
            u32 allocation = (u32)cm_fn_00944cf1(0x1184);
            frame.state = 0;
            u32 child = 0;
            if (allocation) {
                year = W(record + 0x28);
                child = cm_fn_005a9cf0(allocation, year, self, 0);
            }
            frame.state = 0xffffffffu;
            D(self + 0x112c) = child;
            if (!child) {
                name = scratch + 0x304;
                extension = scratch + 0x104;
                line = 0x3ff;
                goto error;
            }
        }
        self = D(self + 0x112c);
        year = W(record + 0x28);
    }
    u32 competition = D(record + 0x14);
    s32 index = (s8)cm_fn_0048dd70(competition, record);
    u32 row = (u32)W(record + 0x2a) * 3u;
    row += B(record + 0x3f);
    u32 table = D(self + row * 4);
    u32 list = D(table + (u32)index * 4);
    result = (u32)cm_fn_0068d0d0(list, record, 0x4f);
    if (result) goto done;
    name = scratch + 4;
    extension = scratch + 0x204;
    line = 0x3eb;
error:
    cm_fn_00944cff(0x009cc208, 0, 0, name, extension);
    u32 text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
