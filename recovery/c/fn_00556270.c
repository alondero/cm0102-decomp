/* Preferred-image VA 00556270, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The final DEC of the four-iteration counter leaves EAX=0 on return. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007e82e0(u32, u32, u32);
extern void CM_THISCALL cm_fn_00556d00(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00556270(u32 self, u32 record,
                             u32 unused __attribute__((unused)), u32 excluded)
{
    u8 scratch[0x208];
    u32 number = D(record);
    u32 base = D(self + 0x14);
    u32 row = number * 29u + base;
    u32 offset = 0;
    for (u32 remaining = 4; remaining; --remaining, offset += 5) {
        u8 slot = B(row + offset);
        if (slot == 0xff) continue;
        u32 table = D(self);
        u32 entry = D(table + (u32)(s32)(s8)slot * 4u);
        u32 a = D(entry + 0x17);
        if (a != 0xffffffffu) {
            if (excluded) {
                u32 p = D(excluded + 0x53);
                if (p && D(p) == a) continue;
            }
        } else {
            u32 b = D(entry + 0x1b);
            if (b == 0xffffffffu) continue;
            if (excluded) {
                u32 p = D(excluded + 0x53);
                if (p) {
                    p = D(p + 0x71);
                    if (p && D(p) == b) continue;
                }
            }
        }
        number = D(record);
        u32 worker = cm_fn_007e82e0(entry, number, 0);
        if (!worker) continue;
        u16 amount = W(worker + 6);
        if ((s16)amount > -1) {
            u32 pushed = (worker & 0xffff0000u) | amount;
            number = D(record);
            cm_fn_00556d00(self, number, pushed);
        }
        if (B(row + 0x1c)) {
            cm_fn_00944cff(0x009b8f18, 0, 0, scratch + 0x108, scratch + 8);
            u32 text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                           scratch + 0x108, scratch + 8, 0x239);
            cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
            D(0x00b67a34) = 0;
        }
        number = D(record);
        u32 position = number * 29u + offset;
        base = D(self + 0x14);
        B(base + position) = 0xff;
        base = D(self + 0x14);
        D(base + position + 1) = 0xffffffffu;
        D(worker) = 0xffffffffu;
    }
    return 0;
}
