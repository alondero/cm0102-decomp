#include "cmre.h"

/* Preferred-image VA 004a2b10, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original table 4a2dc8 permutes the final five physical case bodies. */
#define D(a) (*(volatile u32 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

u32 CM_STDCALL cm_fn_004a2b10(volatile u32 selector, u32 output,
                              volatile s32 size)
{
    u8 scratch[0x200];
    u32 l = (u32)scratch, index, target, text, filename, extension, line, version;
    if (output == 0 || size < 0x33) {
        filename = l; extension = l + 0x100; line = 0x84b;
        goto error;
    }
    index = (u32)(s32)(s8)*(volatile u8 *)&selector - 1u;
    B(output) = 0;
    if (index > 0x10) {
        filename = l + 0x100; extension = l; line = 0x8ce;
        goto error;
    }
    target = D(0x004a2dc8u + index * 4u);
    switch (target) {
    case 0x004a2b4c: text = 0x0099cc40; break;
    case 0x004a2b68: text = 0x0098e9cc; break;
    case 0x004a2b84: text = 0x0099cb44; break;
    case 0x004a2ba0: text = 0x0099cb38; break;
    case 0x004a2bbc: text = 0x0099c06c; break;
    case 0x004a2bd8: text = 0x0099cc34; break;
    case 0x004a2bf4: text = 0x0099cc24; break;
    case 0x004a2c10: text = 0x00990eb8; break;
    case 0x004a2c2c: text = 0x0099cc18; break;
    case 0x004a2c48: text = 0x0099caf4; break;
    case 0x004a2c64: text = 0x0099cae8; break;
    case 0x004a2c80: text = 0x0099c04c; break;
    case 0x004a2c9c: text = 0x0099cc08; break;
    case 0x004a2cb8: text = 0x0099cbf8; break;
    case 0x004a2cd4: text = 0x0099cbe8; break;
    case 0x004a2cf0: text = 0x0099cbd8; break;
    case 0x004a2d0c: text = 0x0099cbc8; break;
    default: __builtin_unreachable();
    }
    cm_fn_0066f4e0(output, text);
    return 1;
error:
    cm_fn_00944cff(0x0099ca94, 0, 0, (void *)filename, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
