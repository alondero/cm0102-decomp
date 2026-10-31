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

/* Preferred-image VA00633300/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/fullEAX; scratch200 diagnostics L0/L100.
   The final row starts with +1ba/+1af before +1a7, as in the original. */
u32 CM_THISCALL cm_fn_00633300(u32 self, volatile u8 mode,
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
        cm_fn_00944cff(0x009e9e14, 0, 0, l + 0x100, l);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x100, l, 0xe4);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521e60(p, 0, 0x19, 5, 0, 4, year);
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521eb0(p, 0, 0x1a, 5, 0, 5, 1, year, 0);
    W(p + 7) = 0xa;
    W(p + 9) = 0;
    W(p + 0xb) = 0;
    W(p + 0xd) = 4;
    B(p + 0x17) = 7;
    W(p + 0x18) = 0x28;
    W(p + 0x1a) = 0x14;
    W(p + 0x1c) = 0x28;
    W(p + 0xf) = 7;
    W(p + 0x1e) = 0;
    B(p + 0x20) = 0;
    B(p + 0x21) = 2;
    B(p + 0x22) = 4;
    D(p + 0x5c) = 0;
    D(p + 0x60) = 0;
    D(p + 0x64) = 0;

    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521e60(p, 1, 1, 6, 0, 3, year);
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521eb0(p, 1, 3, 6, 0, 5, 1, year, 0);
    W(p + 0x6f) = 0x14;
    W(p + 0x71) = 1;
    W(p + 0x73) = 0;
    W(p + 0x75) = 0;
    W(p + 0x77) = 3;
    B(p + 0x7f) = 7;
    W(p + 0x80) = 0x20;
    W(p + 0x82) = 0x10;
    W(p + 0x84) = 0xc;
    W(p + 0x86) = 0x28;
    B(p + 0x88) = 0;
    B(p + 0x89) = 2;
    B(p + 0x8a) = 7;
    D(p + 0xc4) = 0;
    D(p + 0xc8) = 0;
    D(p + 0xcc) = 0;

    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521e60(p, 2, 0xb, 6, 0, 6, year);
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521eb0(p, 2, 0x11, 6, 0, 5, 1, year, 0);
    W(p + 0xd7) = 0x1e;
    W(p + 0xd9) = 2;
    W(p + 0xdb) = 0;
    W(p + 0xdd) = 0;
    W(p + 0xdf) = 3;
    B(p + 0xe7) = 7;
    W(p + 0xe8) = 0x18;
    W(p + 0xea) = 0xc;
    W(p + 0xec) = 8;
    W(p + 0xee) = 0x34;
    B(p + 0xf0) = 0;
    B(p + 0xf1) = 2;
    B(p + 0xf2) = 7;
    D(p + 0x12c) = 0;
    D(p + 0x130) = 0;
    D(p + 0x134) = 0;

    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521e60(p, 3, 0x19, 6, 0, 6, year);
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521eb0(p, 3, 0x1c, 6, 0, 2, 2, year, 0);
    W(p + 0x13f) = 0x82;
    W(p + 0x141) = 3;
    W(p + 0x143) = 0;
    W(p + 0x145) = 0;
    B(p + 0x14f) = 8;
    W(p + 0x150) = 0xc;
    W(p + 0x152) = 6;
    W(p + 0x154) = 0;
    W(p + 0x147) = 3;
    W(p + 0x156) = 0;
    B(p + 0x158) = 0;
    B(p + 0x159) = 2;
    B(p + 0x15a) = 7;
    D(p + 0x194) = 0;
    D(p + 0x198) = 0;
    D(p + 0x19c) = 0;

    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521e60(p, 4, 5, 7, 0, 3, year);
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521eb0(p, 4, 0xa, 7, 0, 1, 2, year, 0);
    W(p + 0x1ba) = 3;
    W(p + 0x1af) = 3;
    W(p + 0x1a7) = 0x96;
    W(p + 0x1a9) = 4;
    W(p + 0x1ab) = 0;
    W(p + 0x1ad) = 0;
    B(p + 0x1b7) = 8;
    W(p + 0x1b8) = 6;
    W(p + 0x1bc) = 0;
    W(p + 0x1be) = 0;
    B(p + 0x1c0) = 0;
    B(p + 0x1c1) = 2;
    B(p + 0x1c2) = 0xe;
    D(p + 0x1fc) = 0;
    D(p + 0x200) = 0;
    D(p + 0x204) = 0;
    return p;
}
