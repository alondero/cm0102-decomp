#include "cmre.h"

extern u32 CM_CDECL cm_fn_004afb40(u32, u32);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00945333(u32, u32, u32, u32);

#define M32(a) (*(volatile u32 *)(u32)(a))
#define M8(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_0063dfe0(u32 self)
{
    u8 scratch[512];
    u32 p, n, idx, entry, a, b, t, offset, tail;
    M32(self + 0xf2) = 0x5a;
    p = (u32)cm_fn_00944e46(0x168);
    M32(self + 0xee) = p;
    if (!p) {
        cm_fn_00944cff(0x9ea204, 0, 0, scratch + 0x100, scratch);
        {
            char *s = cm_fn_0090d130();
            cm_fn_00944c9f(0xae24d0, 0x9870e8, s, scratch + 0x100, scratch, 0x322);
        }
        cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
        M32(0xb67a34) = 0;
        return 0;
    }
    a = M32(0xae2350);
    entry = M32(0xae23b4);
    b = M32(0xae235c);
    n = b - a * 2;
    idx = 0;
    tail = 0;
    if ((s32)n > 0) {
        do {
            p = M32(entry + 0x53);
            if (p && M32(p) == M32(0x9cf360)) {
                p = M32(entry + 0x57);
                if (p) {
                    t = M32(p);
                    if (t == M32(0x9cf578) || t == M32(0x9cf57c) ||
                        t == M32(0x9cf580) || t == M32(0x9cf584) ||
                        t == M32(0x9cf588)) {
                        p = M32(self + 0xee);
                        M32(p + tail * 4) = entry;
                        ++tail;
                        if (tail == M32(self + 0xf2)) break;
                    }
                }
            }
            a = M32(0xae2350);
            b = M32(0xae235c);
            ++idx;
            n = b - a * 2;
            entry += 0x245;
        } while ((s32)idx < (s32)n);
    }
    tail = 0x50;
    offset = 0;
    do {
        p = M32(self + 0xee);
        entry = M32(p + offset);
        a = M32(entry + 0x5b);
        if (a) {
            b = M32(a);
            if (b == M32(0x9cf574) ||
                ((b = M32(a), (b == M32(0x9cf578) || b == M32(0x9cf57c))) &&
                 (t = M8(entry + 0x5f), t == 3 || t == 4 || t == 5))) {
                u32 slot = p + (u32)((s32)(s16)tail * 4);
                ++tail;
                t = M32(slot);
                M32(slot) = entry;
                p = M32(self + 0xee);
                M32(p + offset) = t;
                if ((s32)(s16)tail >= (s32)M32(self + 0xf2)) break;
            }
        }
        offset += 4;
    } while ((s32)offset < 0x140);
    p = M32(self + 0xee);
    cm_fn_004afb40(p, 0x50);
    p = M32(self + 0xee);
    cm_fn_00945333(p, 0x50, 4, 0x53fe50);
    cm_fn_004afb40(M32(self + 0xee), 10);
    cm_fn_004afb40(M32(self + 0xee) + 0x28, 10);
    cm_fn_004afb40(M32(self + 0xee) + 0x50, 10);
    cm_fn_004afb40(M32(self + 0xee) + 0x78, 10);
    cm_fn_004afb40(M32(self + 0xee) + 0xa0, 10);
    cm_fn_004afb40(M32(self + 0xee) + 0xc8, 10);
    cm_fn_004afb40(M32(self + 0xee) + 0xf0, 10);
    cm_fn_004afb40(M32(self + 0xee) + 0x118, 10);
    return 1;
}
