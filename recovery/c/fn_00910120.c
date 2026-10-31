/* Preferred-image VA 00910120, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00910120(u32 self)
{
    u8 work[0x200];
    u8 mode;
    u32 allocation, version, count, last, offset = 0, i = 0;
    u32 source, entries, value;
    mode = B(self + 0xbau);
    W(self + 0x3eu) = mode == 0 ? 50 : (s8)mode < 3 ? 48 : 46;
    count = (u32)(s32)(s16)W(self + 0x3eu);
    allocation = (u32)cm_fn_00944e46(count * 6u);
    D(self + 0xa7u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x00ad443cu, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x2c5u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0xffffffffu;
    }
    count = (u32)(s32)(s16)W(self + 0x3eu);
    last = D(self + 0xb6u) - 1u;
    if ((s32)count > 0) {
        last *= 4u;
        do {
            entries = D(self + 0xa7u);
            B(entries + offset + 4u) = (s32)i < (s32)count / 2 ? 1 : 2;
            source = D(self + 0xb2u);
            entries = D(self + 0xa7u);
            offset += 6u;
            value = D(source + last);
            last -= 4u;
            D(entries + offset - 6u) = value;
            entries = D(self + 0xa7u);
            ++i;
            B(entries + offset - 1u) = 0;
            source = D(self + 0xb2u);
            D(source + last + 4u) = 0;
            count = (u32)(s32)(s16)W(self + 0x3eu);
        } while ((s32)i < (s32)count);
    }
    return 1;
}
