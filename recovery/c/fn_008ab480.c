#include "cmre.h"

/* Preferred-image VA 0x008ab480, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008968f0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

u32 CM_THISCALL cm_fn_008ab480(u32 self, u32 index, u32 alternate)
{
    u8 scratch[0x200];
    u32 base, lane, slot, id, first, second, third, date, person, input, last;
    char *version;

    if ((s8)index < 0 || (s8)index > 19) {
        cm_fn_00944cff(0x00a8b94c, 0, 0, scratch + 0x100, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      scratch + 0x100, scratch, 0x1dde);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    if (alternate != 0) {
        base = D(self + 0xde5);
        if (base == 0) return 0;
    }
    if ((s32)D(self + 0x10) > -1) {
        if (alternate != 0) lane = (u32)(s32)(s8)B(self + 0xdea);
        else lane = (u32)(s32)(s8)B(self + 0xde9);
        base = self + lane * 0x625u;
        slot = base + (u32)(s32)(s8)index * 0x49u + 0x1a8;
        if (B(slot + 0x1d) == 0xfb) return 0;
        id = D(slot);
        if ((s32)id < 0) return 0;
        if ((s32)id >= (s32)D(0x00ae2364)) return 0;
        third = D(slot + 0x10);
        date = slot + 0x14;
        second = D(slot + 0xc);
        first = D(slot + 8);
        base = D(0x00ae23bc);
        person = base + id * 110u;
        if (cm_fn_0053e990(person, first, second, third, date) == 0) return 0;
        id = D(slot);
        base = D(0x00ae23bc);
        return base + id * 110u;
    }
    base = D(self + 4);
    if (base != 0) {
        if (alternate != 0) {
            lane = (u32)(s32)(s8)B(self + 0xdea);
            slot = base + (lane * 20u + (u32)(s32)(s8)index) * 446u + 0x4796;
        } else {
            slot = self + (u32)(s32)(s8)index * 446u + 0x2735;
        }
        if (B(slot + 0x19) == 0xfb) return 0;
        return D(slot + 0x69);
    }
    if (alternate != 0) return D(self + (u32)(s32)(s8)index * 4u + 0xdf0);
    last = D(self + 8);
    input = D(self);
    base = D(0x00de1d08);
    return cm_fn_008968f0(base, input, index, last);
}
