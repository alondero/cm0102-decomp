#include "cmre.h"

/* Preferred-image VA 005bd160, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   L is ESP after SUB 204: first two scan counters L0, diagnostics L4/L104.
   The four scans retain mutable bounds and three separate array reloads. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

u32 CM_THISCALL cm_fn_005bd160(u32 self)
{
    u8 scratch[0x204];
    u32 l = (u32)scratch, selected = 0, pass, row, offset, scan;
    u32 pointer, group, expected, limit, array, bound, version;

    W(self + 0x3e) = 0x36;
    array = (u32)cm_fn_00944e46(0x144);
    D(self + 0xa7) = array;
    if (array == 0) {
        cm_fn_00944cff(0x009ccb98, 0, 0,
                       (void *)(l + 0x104), (void *)(l + 4));
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                       l + 0x104, l + 4, 0x146);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0xffffffffu;
    }
    for (pass = 0; pass < 4; ++pass) {
        bound = D(0x00ae235c);
        row = D(0x00ae23b4);
        scan = 0;
        if (pass < 2) D(l) = 0;
        if ((s32)bound <= 0) continue;
        offset = selected * 6u;
        for (;;) {
            if (pass == 1 && (s32)offset >= 0x60) break;
            pointer = D(row + 0x57);
            if (pointer != 0) {
                group = D(pointer);
                if (pass < 2) {
                    expected = D(0x009cf5d4);
                    if (group != expected) goto next;
                    pointer = D(row + 0x5b);
                    if (pointer == 0) goto next;
                    expected = D(0x009cf5d8);
                    group = D(pointer);
                    if (pass == 0) {
                        if (group == expected) goto next;
                        limit = W(self + 0x40) == 0x7d1 ? 16u : 14u;
                        if ((s32)selected >= (s32)limit) goto next;
                    } else if (group != expected) goto next;
                } else {
                    expected = D(pass == 2 ? 0x009cf5d0 : 0x009cf5cc);
                    if (group != expected) goto next;
                    limit = (u32)(s32)(s16)W(self + 0x3e);
                    if ((s32)selected >= (s32)limit) goto next;
                }
                array = D(self + 0xa7);
                ++selected;
                D(array + offset) = row;
                array = D(self + 0xa7);
                B(array + offset + 4) = 0;
                array = D(self + 0xa7);
                B(array + offset + 5) = 0;
                offset += 6;
                limit = (u32)(s32)(s16)W(self + 0x3e);
                if (selected == limit) break;
            }
next:
            if (pass < 2) scan = D(l);
            bound = D(0x00ae235c);
            ++scan;
            row += 0x245;
            if (pass < 2) D(l) = scan;
            if ((s32)(s16)scan >= (s32)bound) break;
        }
    }
    limit = (u32)(s32)(s16)W(self + 0x3e);
    return selected == limit;
}
