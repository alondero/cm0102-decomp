#include "cmre.h"

/* Preferred-image VA 007CA7A0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   L0 starts as a full DWORD count, then only its low WORD is replaced.
   Date helper indices are read as signed WORDs in the original callees. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
#define YEAR ((u32)(s32)(s16)W(self + 0x40))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern void CM_CDECL cm_fn_00521e60(u32, u32, u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00521eb0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00521f10(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_007ca7a0(u32 self, u32 selector, u32 count_out,
                              u32 zero_out, u32 optional_out)
{
    u8 scratch[0x204];
    u32 L = (u32)scratch, p, row, count, value, year;
    u8 index = 0;
    if ((u8)selector != 0xff) return 0;
    count = W(self + 0x40) == 2001 ? 7 : 6;
    D(L) = count;
    if (optional_out != 0) D(optional_out) = 0;
    W(count_out) = (u16)count;
    W(zero_out) = 0;
    p = cm_fn_00944e46(count * 104u);
    if (p == 0) {
        cm_fn_00944cff(0xa785d4, 0, 0, (void *)(L + 0x104), (void *)(L + 4));
        value = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, value, L + 0x104, L + 4, 0x9c);
        cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
        D(0xb67a34) = 0;
        return 0;
    }
    if (W(L) == 7) {
        cm_fn_00521e60(p, 0, 5, 6, 0, 2, YEAR);
        cm_fn_00521eb0(p, 0, 12, 6, 0, 2, 2, YEAR, 0);
        W(p + 7) = 10; W(p + 9) = 0; W(p + 0xb) = 0; W(p + 0xd) = 0;
        B(p + 0x17) = 4; W(p + 0x18) = 4; W(p + 0x1a) = 2; W(p + 0x1c) = 4;
        W(p + 0xf) = 1; W(p + 0x1e) = 0;
        B(p + 0x20) = 0; B(p + 0x21) = 2; B(p + 0x22) = 3;
        D(p + 0x5c) = 0; D(p + 0x60) = 4000; D(p + 0x64) = 2000;
        index = 1;
    }
    cm_fn_00521e60(p, index, 16, 6, 0, 6, YEAR);
    cm_fn_00521eb0(p, index, 19, 6, 0, 2, 2, YEAR, 0);
    row = p + (u32)index * 104u;
    W(row + 9) = index; W(row + 0xb) = 0; W(row + 0xd) = 0; W(row + 0xf) = 1;
    B(row + 0x17) = 4; W(row + 0x18) = 18; W(row + 0x1a) = 9;
    if (index == 0) {
        W(p + 7) = 10; W(p + 0x1c) = 18; W(p + 0x1e) = 0;
    } else {
        W(row + 7) = 20; W(row + 0x1c) = 16; W(row + 0x1e) = 4;
    }
    B(row + 0x20) = 0; B(row + 0x21) = 2; B(row + 0x22) = 3;
    D(row + 0x5c) = 0; D(row + 0x60) = 6000; D(row + 0x64) = 3000;

    year = YEAR;
    ++index;
    W(L) = index;
    cm_fn_00521e60(p, index, 23, 6, 0, 6, year);
    year = YEAR; value = D(L);
    cm_fn_00521eb0(p, value, 26, 6, 0, 2, 2, year, 0);
    value = D(L);
    cm_fn_00521f10(p, value, 0, 0xffffffff, 0xffffffff, 0xffffffff, 0);
    if (index == 1) {
        W(p + 0x6f) = 20; W(p + 0x84) = 9; W(p + 0x86) = 18;
    } else {
        row = p + (u32)index * 104u;
        W(row + 7) = 30; W(row + 0x1c) = 9; W(row + 0x1e) = 20;
    }
    row = p + (u32)index * 104u;
    ++index;
    value = W(L);
    W(row + 9) = (u16)value; W(row + 0xb) = 0; W(row + 0xd) = 0; W(row + 0xf) = 1;
    B(row + 0x17) = 4; W(row + 0x18) = 18; W(row + 0x1a) = 9;
    B(row + 0x20) = 0; B(row + 0x21) = 2; B(row + 0x22) = 7;
    D(row + 0x5c) = 0; D(row + 0x60) = 10000; D(row + 0x64) = 5000;

    year = YEAR; W(L) = index;
    cm_fn_00521e60(p, index, 6, 7, 0, 6, year);
    year = YEAR; value = D(L);
    cm_fn_00521eb0(p, value, 30, 7, 0, 2, 2, year, 0);
    if (index == 2) {
        W(p + 0xd7) = 30; W(p + 0xec) = 7; W(p + 0xee) = 27;
    } else {
        row = p + (u32)index * 104u;
        W(row + 7) = 40; W(row + 0x1c) = 7; W(row + 0x1e) = 29;
    }
    row = p + (u32)index * 104u;
    ++index;
    value = W(L);
    W(row + 9) = (u16)value; W(row + 0xb) = 0; W(row + 0xd) = 0;
    B(row + 0x17) = 4; W(row + 0x18) = 16; W(row + 0x1a) = 8; W(row + 0xf) = 1;
    B(row + 0x20) = 0; B(row + 0x21) = 2; B(row + 0x22) = 7;
    D(row + 0x5c) = 0; D(row + 0x60) = 45000; D(row + 0x64) = 25000;

    year = YEAR; W(L) = index;
    cm_fn_00521e60(p, index, 10, 8, 0, 6, year);
    year = YEAR; value = D(L);
    cm_fn_00521eb0(p, value, 21, 1, 1, 2, 2, year, 0);
    value = W(L);
    row = p + (u32)index * 104u;
    ++index;
    W(row + 7) = 120; W(row + 9) = (u16)value; W(row + 0xb) = 0; W(row + 0xd) = 0;
    B(row + 0x17) = 6; W(row + 0x18) = 8; W(row + 0x1a) = 4; W(row + 0x1c) = 0;
    W(row + 0xf) = 1; W(row + 0x1e) = 0;
    B(row + 0x20) = 0; B(row + 0x21) = 2; B(row + 0x22) = 7;
    D(row + 0x5c) = 0; D(row + 0x60) = 55000; D(row + 0x64) = 30000;

    year = YEAR; W(L) = index;
    cm_fn_00521e60(p, index, 4, 2, 1, 6, year);
    year = YEAR; value = D(L);
    cm_fn_00521eb0(p, value, 21, 2, 1, 2, 2, year, 0);
    value = W(L);
    row = p + (u32)index * 104u;
    ++index;
    W(row + 7) = 130; W(row + 9) = (u16)value; W(row + 0xb) = 0; W(row + 0xd) = 0;
    B(row + 0x17) = 6; W(row + 0x18) = 4; W(row + 0x1a) = 2; W(row + 0x1c) = 0;
    W(row + 0xf) = 1; W(row + 0x1e) = 0;
    B(row + 0x20) = 0; B(row + 0x21) = 2; B(row + 0x22) = 14;
    D(row + 0x5c) = 0; D(row + 0x60) = 65000; D(row + 0x64) = 35000;

    year = YEAR; W(L) = index;
    cm_fn_00521e60(p, index, 5, 3, 1, 3, year);
    year = YEAR; value = D(L);
    cm_fn_00521eb0(p, value, 9, 4, 1, 2, 2, year, 0);
    row = p + (u32)index * 104u;
    W(row + 7) = 150; W(row + 9) = (u16)value; W(row + 0xb) = 0; W(row + 0xd) = 0;
    B(row + 0x17) = 6; W(row + 0x18) = 2; W(row + 0x1a) = 1; W(row + 0x1c) = 0;
    W(row + 0xf) = 1; W(row + 0x1e) = 0;
    B(row + 0x20) = 0; B(row + 0x21) = 2; B(row + 0x22) = 8;
    D(row + 0x5c) = 0; D(row + 0x60) = 120000; D(row + 0x64) = 60000;
    return p;
}
