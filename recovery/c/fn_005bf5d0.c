#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern void CM_CDECL cm_fn_00521e60(u32, u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00521eb0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005bf5d0(u32 self, u32 stage, u32 first_out, u32 second_out, u32 optional_out)
{
    u8 scratch[0x20c];
    u32 allocated, index, mask, year, text;
    if ((u8)stage != 0xff)
        return 0;
    W((u32)scratch) = 0x200;
    W((u32)scratch + 2) = 0x400;
    W((u32)scratch + 4) = 0x800;
    W((u32)scratch + 6) = 0x1000;
    W((u32)scratch + 8) = 0x2000;
    W((u32)scratch + 10) = 0x4000;
    if (optional_out != 0)
        D(optional_out) = 0;
    W(first_out) = 1;
    W(second_out) = 0;
    allocated = (u32)cm_fn_00944e46(0x68);
    if (allocated == 0) {
        cm_fn_00944cff(0x009ccc48, 0, 0, (u32)scratch + 0x10c, (u32)scratch + 0xc);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x10c, (u32)scratch + 0xc, 0x97);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    year = (u32)(s32)(s16)W(self + 0x40);
    cm_fn_00521e60(allocated, 0, 5, 6, 0, 5, year);
    if (W(self + 0x40) == 0x7d1) {
        cm_fn_00521eb0(allocated, 0, 0x18, 6, 0, 5, 2, 0x7d1, 0x400);
    } else {
        index = cm_fn_0090cfc0(6);
        mask = (u32)(s32)(s16)W((u32)scratch + index * 2u);
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521eb0(allocated, 0, 0x18, 6, 0, 5, 2, year, mask);
    }
    W(allocated + 7) = 0;
    W(allocated + 9) = 0;
    W(allocated + 0xb) = 0;
    W(allocated + 0xd) = 1;
    B(allocated + 0x17) = 6;
    W(allocated + 0x18) = 2;
    W(allocated + 0x1a) = 1;
    W(allocated + 0x1c) = 2;
    W(allocated + 0xf) = 0;
    W(allocated + 0x1e) = 0;
    B(allocated + 0x20) = 0;
    B(allocated + 0x21) = 1;
    B(allocated + 0x22) = 0;
    D(allocated + 0x5c) = 0xea60;
    D(allocated + 0x60) = 0;
    D(allocated + 0x64) = 0;
    return allocated;
}
