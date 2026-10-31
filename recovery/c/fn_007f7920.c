#include "cmre.h"
/* Preferred-image VA 0x007f7920, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Index arithmetic wraps in AL and reads only the low byte of 00ae2364. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_007f7920(u32 self, u8 selector)
{
    u8 scratch[512];
    u8 *filename, *extension;
    u32 index, manager, line;
    u8 value, base;
    char *version;
    index = (u32)(s32)(s16)W(0x00b77c7a);
    manager = D(0x00b74c26 + index * 768u);
    if (!manager) {
        filename = scratch + 256;
        extension = scratch;
        line = 0x2fa;
    } else {
        value = B(manager);
        base = B(0x00ae2364);
        value = (u8)(value - base);
        value = (u8)(value + 16);
        if ((s8)value >= 0 && (s8)value <= 15) {
            index = (u32)(s32)(s8)value;
            if (selector == 1) return D(self + index * 4u + 0x68);
            return D(self + index * 4u + 0xa8);
        }
        filename = scratch;
        extension = scratch + 256;
        line = 0x304;
    }
    cm_fn_00944cff(0x00a7b094, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
