#include "cmre.h"

/* Preferred-image VA0090F600/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX. Signed BYTE selectors and fresh BA/year reads.
   68A160 forwards year through549F70's WORD mask; its later subtraction
   contributes only a WORD. Unknown partial-register high bits are unobserved. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern void CM_CDECL cm_fn_00521e60(u32, u32, u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00521eb0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0068a160(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0068a1c0(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

static void group_row(u32 base, u32 index, u32 self,
                      u32 day, u32 month, u32 extra, u32 anchor, u32 width)
{
    u32 year = W(self + 0x40);
    cm_fn_0068a160(base, index, day, month, extra, anchor, width, year, 0);
    cm_fn_0068a1c0(base, index, 0, 0xffffffffu, 0xffffffffu, 0xffffffffu, 0);
}

u32 CM_THISCALL cm_fn_0090f600(u32 self, u32 selector, u32 count_out,
                              u32 code_out, u32 optional_out)
{
    u8 scratch[0x204];
    u32 L = (u32)scratch, p, year, size, count, name, extension, line, version;
    u8 mode = (u8)selector, kind, index;
    if (mode == 0xff) {
        if (optional_out != 0) D(optional_out) = 0;
        W(count_out) = 1;
        W(code_out) = 0;
        p = cm_fn_00944e46(0x68);
        if (p == 0) {
            name = L + 0x104; extension = L + 4; line = 0xde;
            goto error;
        }
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521e60(p, 0, 18, 0, 0, 4, year);
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521eb0(p, 0, 8, 3, 0, 5, 1, year, 0);
        W(p + 7) = 10;
        W(p + 9) = 0;
        W(p + 0xb) = 0;
        W(p + 0xd) = 0x400;
        B(p + 0x17) = 10;
        W(p + 0xf) = 3;
        W(p + 0x1e) = 0;
        B(p + 0x20) = 0;
        B(p + 0x21) = 2;
        B(p + 0x22) = 14;
        D(p + 0x5c) = 0;
        D(p + 0x60) = 0;
        D(p + 0x64) = 0;
        kind = B(self + 0xba);
        if (kind == 0) size = 50;
        else if (kind == 1 || kind == 2) size = 48;
        else size = 46;
        W(p + 0x1a) = (u16)(size / 2);
        W(p + 0x18) = (u16)size;
        W(p + 0x1c) = (u16)size;
        return p;
    }
    kind = B(self + 0xba);
    if ((kind == 0 && (s8)mode < 5) ||
        (kind == 1 && (s8)mode < 4) ||
        (kind == 2 && (s8)mode < 6) ||
        (kind == 3 && (s8)mode < 4)) {
        count = kind == 2 ? 6 : 10;
        D(L) = count;
        if (optional_out != 0) D(optional_out) = 1;
        W(count_out) = (u16)count;
        W(code_out) = (u16)((u32)(s32)(s8)mode + 0x407u);
        if (mode == 5) W(code_out) = 0x472;
        p = cm_fn_00944e46(count * 65u);
        if (p == 0) {
            name = L + 4; extension = L + 0x104; line = 0x126;
            goto error;
        }
        index = 0;
        if (count == 10) {
            group_row(p, index, self, 17, 5, 0, 5, 1);
            index = 1;
        }
        group_row(p, index, self, 8, 6, 0, 6, 1);
        ++index;
        group_row(p, index, self, 27, 0, 1, 5, 1);
        ++index;
        if (W(L) == 10) {
            group_row(p, index, self, 24, 1, 1, 5, 1);
            ++index;
        }
        group_row(p, index, self, 10, 2, 1, 5, 1);
        ++index;
        group_row(p, index, self, 21, 3, 1, 5, 1);
        ++index;
        group_row(p, index, self, 5, 4, 1, 5, 1);
        ++index;
        if (W(L) == 10) {
            group_row(p, index, self, 30, 5, 1, 5, 1);
            ++index;
        }
        group_row(p, index, self, 14, 6, 1, 5, 1);
        ++index;
        if (W(L) == 10)
            group_row(p, index, self, 17, 6, 1, 2, 2);
        return p;
    }
    if (!((kind == 2 && mode == 6) || (kind == 3 && mode == 4))) return 0;
    if (optional_out != 0) D(optional_out) = 0;
    W(count_out) = 1;
    W(code_out) = 0;
    p = cm_fn_00944e46(0x68);
    if (p == 0) {
        name = L + 4; extension = L + 0x104; line = 0x171;
        goto error;
    }
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521e60(p, 0, 29, 8, 1, 0, year);
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521eb0(p, 0, 20, 9, 1, 5, 1, year, 0);
    W(p + 7) = 150;
    W(p + 9) = 0;
    W(p + 0xb) = 0;
    W(p + 0xd) = 0;
    B(p + 0x17) = 10;
    kind = B(self + 0xba);
    size = kind == 2 ? 6 : 4;
    W(p + 0x1a) = (u16)(size / 2);
    W(p + 0x18) = (u16)size;
    W(p + 0x1c) = (u16)size;
    W(p + 0xf) = 3;
    W(p + 0x1e) = 0;
    B(p + 0x20) = 0;
    B(p + 0x21) = 2;
    B(p + 0x22) = 14;
    D(p + 0x5c) = 0;
    D(p + 0x60) = 0;
    D(p + 0x64) = 0;
    return p;
error:
    cm_fn_00944cff(0xad443c, 0, 0, (void *)name, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0, 0x9870e8, version, name, extension, line);
    cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
    D(0xb67a34) = 0;
    return 0;
}
