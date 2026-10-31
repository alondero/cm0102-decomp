#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00521e60(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00521eb0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Preferred-image VA0090b170/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/fullEAX. Scratch200: diagnostics L0/L100.
   Each year is reloaded before its own original helper call. */
u32 CM_THISCALL cm_fn_0090b170(u32 self, volatile u8 mode,
                              volatile u32 count_out, volatile u32 zero_out,
                              volatile u32 optional_out)
{
    u32 scratch[0x200 / 4];
    u32 l = (u32)scratch, p, year, version, count_pointer, zero_pointer;
    if (mode != 0xff) return 0;
    p = optional_out;
    if (p != 0) D(p) = 0;
    count_pointer = count_out;
    zero_pointer = zero_out;
    W(count_pointer) = 5;
    W(zero_pointer) = 0;
    p = cm_fn_00944e46(0x208);
    if (p == 0) {
        cm_fn_00944cff(0x00aa20e0, 0, 0, l + 0x100, l);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x100, l, 0x9c);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521e60(p, 0, 1, 5, 0, 3, year);
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521eb0(p, 0, 0xe, 5, 0, 2, 2, year, 0);
    W(p + 7) = 0x14;
    W(p + 9) = 0;
    W(p + 0xb) = 0;
    W(p + 0xd) = 0x483;
    B(p + 0x17) = 4;
    W(p + 0x18) = 0x20;
    W(p + 0x1a) = 0x10;
    W(p + 0x1c) = 0x20;
    W(p + 0xf) = 0;
    W(p + 0x1e) = 0;
    B(p + 0x20) = 0;
    B(p + 0x21) = 0;
    B(p + 0x22) = 0;
    D(p + 0x5c) = 0;
    D(p + 0x60) = 0xffffffff;
    D(p + 0x64) = 0;

    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521e60(p, 1, 0xf, 5, 0, 3, year);
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521eb0(p, 1, 0x19, 5, 0, 6, 1, year, 0);
    W(p + 0x6f) = 0x1e;
    W(p + 0x71) = 1;
    W(p + 0x73) = 0;
    W(p + 0x75) = 0x83;
    B(p + 0x7f) = 4;
    W(p + 0x80) = 0x10;
    W(p + 0x82) = 8;
    W(p + 0x84) = 0;
    W(p + 0x77) = 0;
    W(p + 0x86) = 0;
    B(p + 0x88) = 0;
    B(p + 0x89) = 0;
    B(p + 0x8a) = 0;
    D(p + 0xc4) = 0;
    D(p + 0xc8) = 0xffffffff;
    D(p + 0xcc) = 0;

    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521e60(p, 2, 0x1a, 5, 0, 0, year);
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521eb0(p, 2, 9, 7, 0, 2, 2, year, 0);
    W(p + 0xd7) = 0x78;
    W(p + 0xd9) = 2;
    W(p + 0xdb) = 0;
    W(p + 0xdd) = 0x83;
    B(p + 0xe7) = 6;
    W(p + 0xe8) = 8;
    W(p + 0xea) = 4;
    W(p + 0xec) = 0;
    W(p + 0xdf) = 0;
    W(p + 0xee) = 0;
    B(p + 0xf0) = 0;
    B(p + 0xf1) = 0;
    B(p + 0xf2) = 0;
    D(p + 0x12c) = 0;
    D(p + 0x130) = 0xffffffff;
    D(p + 0x134) = 0;

    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521e60(p, 3, 0xa, 7, 0, 3, year);
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521eb0(p, 3, 0x17, 7, 0, 2, 2, year, 0);
    W(p + 0x13f) = 0x82;
    W(p + 0x141) = 3;
    W(p + 0x143) = 0;
    W(p + 0x145) = 0x83;
    B(p + 0x14f) = 6;
    W(p + 0x150) = 4;
    W(p + 0x152) = 2;
    W(p + 0x154) = 0;
    W(p + 0x147) = 0;
    W(p + 0x156) = 0;
    B(p + 0x158) = 0;
    B(p + 0x159) = 0;
    B(p + 0x15a) = 0;
    D(p + 0x194) = 0;
    D(p + 0x198) = 0xffffffff;
    D(p + 0x19c) = 0;

    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521e60(p, 4, 0x18, 7, 0, 3, year);
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521eb0(p, 4, 0x16, 9, 0, 6, 1, year, 4);
    D(p + 0x200) = 0xffffffff;
    W(p + 0x1a7) = 0x96;
    W(p + 0x1a9) = 4;
    W(p + 0x1ab) = 0;
    W(p + 0x1ad) = 0x83;
    B(p + 0x1b7) = 6;
    W(p + 0x1b8) = 2;
    W(p + 0x1ba) = 1;
    W(p + 0x1bc) = 0;
    W(p + 0x1af) = 0;
    W(p + 0x1be) = 0;
    B(p + 0x1c0) = 0;
    B(p + 0x1c1) = 0;
    B(p + 0x1c2) = 0;
    D(p + 0x1fc) = 0;
    D(p + 0x204) = 0;
    return p;
}
