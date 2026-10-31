#include "cmre.h"

/* Preferred-image VA 00551240, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch204: scan L0, diagnostic buffers L4/L104. The first two passes
   continue after filling their cap; only passes three/four break on equality. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

u32 CM_THISCALL cm_fn_00551240(u32 self)
{
    u8 scratch[0x204];
    u32 l = (u32)scratch, selected = 0, pass, row, offset, scan;
    u32 pointer, league, group, expected, limit, array, bound, version;
    W(self + 0x3e) = 0x5c;
    array = (u32)cm_fn_00944e46(0x228);
    D(self + 0xa7) = array;
    if (array == 0) {
        cm_fn_00944cff(0x009b8dfc, 0, 0, (void *)(l + 0x104), (void *)(l + 4));
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x104, l + 4, 0x15a);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0xffffffffu;
    }
    for (pass = 0; pass < 4; ++pass) {
        bound = D(0x00ae235c);
        if (pass == 1) league = D(0x009cf2c0);
        row = D(0x00ae23b4);
        scan = 0;
        if (pass < 2) D(l) = 0;
        if ((s32)bound <= 0) continue;
        offset = selected * 6u;
        for (;;) {
            pointer = D(row + 0x57);
            if (pass == 0) league = D(0x009cf2c0);
            if (pointer != 0) {
                group = D(pointer);
                expected = D(pass == 0 ? 0x009cf764 : pass == 1 ? 0x009cf5b4 :
                             pass == 2 ? 0x009cf5b0 : 0x009cf5ac);
                if (group != expected) goto next;
                if (pass < 2) {
                    expected = D(0x00ae23a8) + league * 0x122u;
                    pointer = D(row + 0x53);
                    if (pointer != expected) goto next;
                    if (pass == 0) {
                        if ((s32)offset >= 0x120) goto next;
                    } else {
                        limit = (u32)(s32)(s16)W(self + 0x3e);
                        if ((s32)selected >= (s32)limit) goto next;
                    }
                } else {
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
                if (pass >= 2) {
                    limit = (u32)(s32)(s16)W(self + 0x3e);
                    if (selected == limit) break;
                }
            }
next:
            if (pass < 2) {
                scan = D(l);
                bound = D(0x00ae235c);
                ++scan;
                row += 0x245;
                D(l) = scan;
            } else {
                ++scan;
                row += 0x245;
                D(l) = scan;
                bound = D(0x00ae235c);
            }
            if ((s32)(s16)scan >= (s32)bound) break;
            if (pass == 1) league = D(0x009cf2c0);
            if (pass >= 2) scan = D(l);
        }
    }
    limit = (u32)(s32)(s16)W(self + 0x3e);
    return selected == limit;
}
