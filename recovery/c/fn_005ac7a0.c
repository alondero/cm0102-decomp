/* Preferred-image VA 0x005ac7a0, base 0x00400000; CM 3.9.68
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Year upper-half deadness: 005aa2d0 compares/stores BX, recursively
   forwards it, and otherwise forwards EBX only to 0054cfe0, which masks
   ECX with 0xffff at 0054cfe4. 005a9cf0 likewise stores AX and forwards
   the year only to that helper (005a9e53..005a9e62). */
#include "cmre.h"
extern u8 CM_CDECL cm_fn_0048dd70(u32, u32);
extern u32 CM_THISCALL cm_fn_005aa2d0(u32, u16);
extern u32 CM_THISCALL cm_fn_005a9cf0(u32, u16, u32, u32);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define R16(a) CM_GLOBAL(volatile u16, (a))
#define R32(a) CM_GLOBAL(volatile u32, (a))
#define R8(a) CM_GLOBAL(volatile u8, (a))

u32 CM_THISCALL cm_fn_005ac7a0(u32 self, u32 record)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    u32 result = 0;
    u32 previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095979e;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    if (!record) goto done;
    for (;;) {
        u16 year = R16(record + 0x28);
        if (year == R16(self + 0x1130)) break;
        u32 next = R32(self + 0x112c);
        if (next) {
            if (R16(next + 0x1130) != year) {
                cm_fn_005aa2d0(next, year);
                self = R32(self + 0x112c);
                continue;
            }
        } else {
            u32 allocation = (u32)cm_fn_00944cf1(0x1184);
            frame.state = 0;
            u32 created = 0;
            if (allocation) {
                year = R16(record + 0x28);
                created = cm_fn_005a9cf0(allocation, year, self, 0);
            }
            frame.state = 0xffffffffu;
            R32(self + 0x112c) = created;
            if (!created) {
                cm_fn_00944cff(0x009cc208, 0, 0, scratch + 0x104, scratch + 4);
                u32 text = cm_fn_0090d130();
                cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                               scratch + 0x104, scratch + 4, 0x7cf);
                cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
                R32(0x00b67a34) = 0;
                goto done;
            }
        }
        self = R32(self + 0x112c);
    }
    {
        u32 p = R32(record + 0x14);
        s32 index = (s8)cm_fn_0048dd70(p, record);
        u32 row = (u32)R16(record + 0x2a) * 3;
        row += R8(record + 0x3f);
        u32 table = R32(self + row * 4);
        u32 list = R32(table + (u32)index * 4);
        u32 value = R32(list + 2);
        R32(list + 0xa) = value;
        result = cm_fn_0068d1b0(list);
        while (result) {
            u32 i;
            for (i = 0; i < 0x4f; ++i) {
                u8 a = R8(result + i);
                u8 b = R8(record + i);
                if (a != b) break;
            }
            if (i == 0x4f) goto done;
            result = cm_fn_0068d1b0(list);
        }
    }
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
