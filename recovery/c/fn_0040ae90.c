#include "cmre.h"

/* Preferred-image VA0040AE90/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX. Each selector and date/store tuple is checked.
   Original68A160 forwards year through549F70's WORD mask, and subtracts
   it only into a WORD result; partial-register upper bits are unobserved. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern void CM_CDECL cm_fn_00521e60(u32, u32, u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00521eb0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00521f10(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0068a160(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0068a1c0(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0040ae90(u32 self, u32 selector, u32 count_out,
                              u32 code_out, u32 optional_out)
{
    u8 scratch[0x200];
    u32 L = (u32)scratch, p, i, r, year, name, extension, line, version;
    u8 mode = (u8)selector;
    if (mode == 1 || mode == 2) {
        if (optional_out != 0) D(optional_out) = 1;
        W(count_out) = 3;
        W(code_out) = 0;
        W(code_out) = mode == 1 ? 0x482 : 0x483;
        p = cm_fn_00944e46(0xc3);
        if (p == 0) {
            name = L; extension = L + 0x100; line = 0x103;
            goto error;
        }
        for (i = 0; i != 3; ++i) {
            year = W(self + 0x40);
            cm_fn_0068a160(p, i, 21 + i * 2, 2, 1, 2 + i * 2, 1, year, 1);
            cm_fn_0068a1c0(p, i, 0, 2 + i * 2, 2, 3, 16);
            cm_fn_0068a1c0(p, i, 1, 2 + i * 2, 1, 3, 16);
            cm_fn_0068a1c0(p, i, 2, 0xffffffff, 0xffffffff, 0xffffffff, 0);
        }
        return p;
    }
    if (mode == 0 || mode == 0xff) {
        if (optional_out != 0) D(optional_out) = 0;
        W(count_out) = 2;
        W(code_out) = mode == 0xff ? 0x480 : 0x481;
        p = cm_fn_00944e46(0xd0);
        if (p == 0) {
            name = L; extension = L + 0x100; line = 0xc5;
            goto error;
        }
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521e60(p, 0, 1, 7, 0, 5, year);
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521eb0(p, 0, 23, 7, 0, 2, 2, year, 0);
        W(p + 7) = 10; W(p + 9) = 0; W(p + 0xb) = 0; W(p + 0xd) = 0;
        B(p + 0x17) = 4; W(p + 0x18) = 16; W(p + 0x1a) = 8; W(p + 0x1c) = 16;
        W(p + 0xf) = 0x83; W(p + 0x1e) = 0;
        B(p + 0x20) = 0; B(p + 0x21) = 2; B(p + 0x22) = 0x15;
        D(p + 0x5c) = 0; D(p + 0x60) = 0; D(p + 0x64) = 0;
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521e60(p, 1, 14, 8, 0, 3, year);
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521eb0(p, 1, 20, 8, 0, 2, 2, year, 0);
        r = p + 104;
        W(r + 7) = 20; W(r + 9) = 1; W(r + 0xb) = 0; W(r + 0xd) = 0;
        W(r + 0xf) = 0x83; B(r + 0x17) = 4; W(r + 0x18) = 8; W(r + 0x1a) = 4;
        W(r + 0x1c) = 0; W(r + 0x1e) = 0;
        B(r + 0x20) = 0; B(r + 0x21) = 2; B(r + 0x22) = 7;
        D(r + 0x5c) = 0; D(r + 0x60) = 0; D(r + 0x64) = 0;
        return p;
    }
    if (mode != 3) return 0;
    if (optional_out != 0) D(optional_out) = 0;
    W(count_out) = 3;
    if (code_out != 0) W(code_out) = 0;
    p = cm_fn_00944e46(0x138);
    if (p == 0) {
        name = L + 0x100; extension = L; line = 0x125;
        goto error;
    }
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521e60(p, 0, 26, 2, 1, 0, year);
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521eb0(p, 0, 24, 4, 1, 3, 2, year, 1);
    cm_fn_00521f10(p, 0, 0, 3, 1, 3, 16);
    cm_fn_00521f10(p, 0, 1, 3, 2, 3, 16);
    cm_fn_00521f10(p, 0, 2, 0xffffffff, 0xffffffff, 0xffffffff, 0);
    W(p + 7) = 130; W(p + 9) = 0; W(p + 0xb) = 0; W(p + 0xd) = 0x383;
    B(p + 0x17) = 6; W(p + 0x18) = 4; W(p + 0x1a) = 2; W(p + 0x1c) = 4;
    W(p + 0xf) = 0; W(p + 0x1e) = 0;
    B(p + 0x20) = 0; B(p + 0x21) = 1; B(p + 0x22) = 0;
    D(p + 0x5c) = 0; D(p + 0x60) = 0; D(p + 0x64) = 0;
    /* EAX=4 survives the preceding stores and supplies both date arguments. */
    for (i = 1; i != 3; ++i) {
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521e60(p, i, 25, 4, 1, 4, year);
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521eb0(p, i, 26, 4, 1, 5, i == 1 ? 2 : 1, year, 16);
        r = p + i * 104u;
        W(r + 7) = i == 1 ? 150 : 140;
        W(r + 9) = (u16)i; W(r + 0xb) = 0; W(r + 0xd) = 0x83; W(r + 0xf) = 0;
        B(r + 0x17) = 6; W(r + 0x18) = 2; W(r + 0x1a) = 1;
        W(r + 0x1c) = 0; W(r + 0x1e) = 0;
        B(r + 0x20) = 0; B(r + 0x21) = 1; B(r + 0x22) = 0;
        D(r + 0x5c) = 0; D(r + 0x60) = 0; D(r + 0x64) = 0;
    }
    return p;
error:
    cm_fn_00944cff(0x987a40, 0, 0, (void *)name, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0, 0x9870e8, version, name, extension, line);
    cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
    D(0xb67a34) = 0;
    return 0;
}
