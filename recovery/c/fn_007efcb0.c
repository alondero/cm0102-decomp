#include "cmre.h"

/* Preferred-image VA 007EFCB0, base 00400000, pinned 3.9.68 image.
   Each date tuple and field write follows its own original row. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
#define YEAR ((u32)(s32)(s16)W(self + 0x40))
#define N ((s32)(s8)B(self + 0xb2))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern void CM_CDECL cm_fn_00521e60(u32, u32, u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00521eb0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_007efcb0(u32 self, u32 selector, u32 count_out,
                              u32 zero_out, u32 optional_out)
{
    u8 scratch[0x200];
    u32 L = (u32)scratch, p, version;
    s32 n;
    if ((u8)selector != 0xff) return 0;
    if (optional_out != 0) D(optional_out) = 0;
    W(count_out) = 6;
    W(zero_out) = 0;
    p = cm_fn_00944e46(0x270);
    if (p == 0) {
        cm_fn_00944cff(0xa7af38, 0, 0, (void *)(L + 0x100), (void *)L);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, version, L + 0x100, L, 0xa0);
        cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
        D(0xb67a34) = 0;
        return 0;
    }
    cm_fn_00521e60(p, 0, 10, 6, 0, 6, YEAR);
    cm_fn_00521eb0(p, 0, 31, 6, 0, 5, 1, YEAR, 0);
    W(p + 7) = 10; W(p + 9) = 0; W(p + 0xb) = 0; W(p + 0xd) = 3;
    B(p + 0x17) = 4;
    if (W(self + 0x40) == 1999) {
        n = N; W(p + 0x18) = (u16)(n * 2 + 16);
        n = N; W(p + 0x1a) = (u16)(n + 8);
        n = N; W(p + 0x1c) = (u16)(n * 2 + 16);
    } else {
        n = N; W(p + 0x18) = (u16)(n * 2 + 20);
        n = N; W(p + 0x1a) = (u16)(n + 10);
        n = N; W(p + 0x1c) = (u16)(n * 2 + 20);
    }
    W(p + 0x1e) = 0; W(p + 0xf) = 0;
    B(p + 0x20) = 0; B(p + 0x21) = 1; B(p + 0x22) = 0;
    D(p + 0x5c) = 0; D(p + 0x60) = 0; D(p + 0x64) = 0;

    cm_fn_00521e60(p, 1, 1, 7, 0, 6, YEAR);
    cm_fn_00521eb0(p, 1, 17, 7, 0, 1, 2, YEAR, 0);
    W(p + 0x6f) = 20; W(p + 0x71) = 1; W(p + 0x73) = 0;
    W(p + 0x75) = 0x403; W(p + 0x77) = 0; B(p + 0x7f) = 4;
    /* The year is read before the first signed byte on both branches. */
    version = W(self + 0x40);
    n = N;
    W(p + 0x80) = (u16)((16 - n) * 2);
    if (version == 1999) {
        n = N; W(p + 0x82) = (u16)(16 - n);
        n = N; W(p + 0x84) = (u16)((8 - n) * 3);
        n = N; W(p + 0x86) = (u16)(n * 2 + 16);
    } else {
        n = N; W(p + 0x82) = (u16)(16 - n);
        n = N; W(p + 0x84) = (u16)(22 - n * 3);
        n = N; W(p + 0x86) = (u16)(n * 2 + 20);
    }
    B(p + 0x88) = 0; B(p + 0x89) = 1; B(p + 0x8a) = 0;
    D(p + 0xc4) = 0; D(p + 0xc8) = 0; D(p + 0xcc) = 0;

    cm_fn_00521e60(p, 2, 19, 7, 0, 3, YEAR);
    cm_fn_00521eb0(p, 2, 22, 8, 0, 2, 2, YEAR, 0);
    W(p + 0xd7) = 30; W(p + 0xd9) = 2; W(p + 0xdb) = 0;
    W(p + 0xdd) = 3; W(p + 0xdf) = 0; B(p + 0xe7) = 4;
    W(p + 0xe8) = 16; W(p + 0xea) = 8;
    n = N; W(p + 0xec) = (u16)n;
    if (W(self + 0x40) == 1999) {
        n = N; W(p + 0xee) = (u16)(40 - n);
    } else {
        n = N; W(p + 0xee) = (u16)(42 - n);
    }
    B(p + 0xf0) = 0; B(p + 0xf1) = 1; B(p + 0xf2) = 0;
    D(p + 0x12c) = 0; D(p + 0x130) = 0xffffffff; D(p + 0x134) = 0;

    cm_fn_00521e60(p, 3, 23, 8, 0, 3, YEAR);
    cm_fn_00521eb0(p, 3, 1, 11, 0, 2, 2, YEAR, 0);
    W(p + 0x13f) = 120; W(p + 0x141) = 3; W(p + 0x143) = 0;
    W(p + 0x145) = 3; B(p + 0x14f) = 6; W(p + 0x150) = 8;
    W(p + 0x152) = 4; W(p + 0x154) = 0; W(p + 0x147) = 0;
    W(p + 0x156) = 0; B(p + 0x158) = 0; B(p + 0x159) = 1; B(p + 0x15a) = 4;
    D(p + 0x194) = 0; D(p + 0x198) = 0xffffffff; D(p + 0x19c) = 0;

    /* Original EAX=3 survives the preceding stores to the next date call. */
    cm_fn_00521e60(p, 4, 2, 11, 0, 3, YEAR);
    cm_fn_00521eb0(p, 4, 16, 1, 1, 2, 2, YEAR, 40);
    W(p + 0x1a7) = 130; W(p + 0x1a9) = 4; W(p + 0x1ab) = 0;
    W(p + 0x1ad) = 3; B(p + 0x1b7) = 6; W(p + 0x1b8) = 4;
    W(p + 0x1ba) = 2; W(p + 0x1bc) = 0; W(p + 0x1af) = 0;
    W(p + 0x1be) = 0; B(p + 0x1c0) = 0; B(p + 0x1c1) = 1; B(p + 0x1c2) = 0;
    D(p + 0x1fc) = 0; D(p + 0x200) = 0xffffffff; D(p + 0x204) = 0;

    cm_fn_00521e60(p, 5, 17, 1, 1, 3, YEAR);
    cm_fn_00521eb0(p, 5, 19, 2, 1, 6, 1, YEAR, 4);
    D(p + 0x268) = 0xffffffff;
    W(p + 0x20f) = 150; W(p + 0x211) = 5; W(p + 0x213) = 0;
    W(p + 0x215) = 3; B(p + 0x21f) = 6; W(p + 0x220) = 2;
    W(p + 0x222) = 1; W(p + 0x224) = 0; W(p + 0x217) = 0;
    W(p + 0x226) = 0; B(p + 0x228) = 0; B(p + 0x229) = 1; B(p + 0x22a) = 0;
    D(p + 0x264) = 0; D(p + 0x26c) = 0;
    return p;
}
