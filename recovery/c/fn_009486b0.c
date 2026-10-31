/* WIP: original stack-probe fault address differs, case 11 at O0/O2.
   Preferred-image VA 009486b0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0094ed85(u32, u32, u32, u32, u32, u32, u32);
extern s32 CM_CDECL cm_fn_0094c0fe(u32, u32, const char *, s32, void *, s32, u32, s32);

u32 CM_CDECL cm_fn_009486b0(void)
{
    u8 scratch[0x514];
    u32 cp = D(0x00de4408);
    u32 target = D(0x00967128);
    u32 result = ((u32 (CM_STDCALL *)(u32, void *))target)(cp, scratch + 0x500);
    if (result == 1) {
        for (u32 i = 0; i < 256; ++i) B((u32)scratch + 0x400 + i) = (u8)i;
        u8 first = B((u32)scratch + 0x506);
        B((u32)scratch + 0x400) = 0x20;
        if (first) {
            u32 end_address = (u32)scratch + 0x507;
            do {
                u32 last = B(end_address);
                if ((u32)first <= last) {
                    u32 address = (u32)scratch + 0x400 + first;
                    u32 count = last - first + 1;
                    u32 dwords = count >> 2;
                    for (u32 i = 0; i < dwords; ++i, address += 4) D(address) = 0x20202020;
                    for (u32 i = 0; i < (count & 3); ++i, ++address) B(address) = 0x20;
                }
                end_address += 2;
                first = B(end_address - 1);
            } while (first);
        }
        u32 locale = D(0x00de4624);
        cp = D(0x00de4408);
        cm_fn_0094ed85(1, (u32)scratch + 0x400, 256, (u32)scratch, cp, locale, 0);
        cp = D(0x00de4408);
        locale = D(0x00de4624);
        cm_fn_0094c0fe(locale, 0x100, (const char *)scratch + 0x400, 256,
                       scratch + 0x300, 256, cp, 0);
        cp = D(0x00de4408);
        locale = D(0x00de4624);
        cm_fn_0094c0fe(locale, 0x200, (const char *)scratch + 0x400, 256,
                       scratch + 0x200, 256, cp, 0);
        for (u32 i = 0; i < 256; ++i) {
            u16 flags = W((u32)scratch + i * 2);
            if (flags & 1) {
                B(0x00de4521u + i) |= 0x10;
                u8 value = B((u32)scratch + 0x300 + i);
                B(0x00de4420u + i) = value;
            } else if (flags & 2) {
                B(0x00de4521u + i) |= 0x20;
                u8 value = B((u32)scratch + 0x200 + i);
                B(0x00de4420u + i) = value;
            } else {
                B(0x00de4420u + i) &= 0;
            }
        }
    } else {
        for (u32 i = 0; i < 256; ++i) {
            if (i >= 0x41 && i <= 0x5a) {
                B(0x00de4521u + i) |= 0x10;
                B(0x00de4420u + i) = (u8)(i + 0x20);
            } else if (i >= 0x61 && i <= 0x7a) {
                B(0x00de4521u + i) |= 0x20;
                B(0x00de4420u + i) = (u8)(i - 0x20);
            } else {
                B(0x00de4420u + i) &= 0;
            }
        }
    }
    return 256;
}
