/* Preferred-image VA 004a5010, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_STDCALL cm_fn_004a5010(s8 selector, u32 destination, s32 size)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 line, text;
    if (destination == 0 || size < 50) {
        name = work;
        extension = work + 0x100;
        line = 0xdf0u;
        goto diagnostic;
    }
    B(destination) = 0;
    if (selector == 1) {
        cm_fn_0066f4e0(destination, 0x0099ccd0u);
        return 1;
    }
    if (selector == 2) {
        cm_fn_0066f4e0(destination, 0x0099ccdcu);
        return 1;
    }
    name = work + 0x100;
    extension = work;
    line = 0xe0au;

diagnostic:
    cm_fn_00944cff(0x0099ca94u, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
