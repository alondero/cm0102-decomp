/* Preferred-image VA 007e4920, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern void CM_THISCALL cm_fn_00417c10(u32 self, void *scratch, u32 source);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_007e4920(u32 source, u32 destination)
{
    u8 work[0x208];
    u8 *name, *extension;
    u32 line, text, pointer, value;
    if (source == 0) {
        name = work + 0x108;
        extension = work + 8;
        line = 0x80u;
        goto diagnostic;
    }
    if (destination == 0) {
        name = work + 8;
        extension = work + 0x108;
        line = 0x88u;
        goto diagnostic;
    }
    pointer = D(source + 4u);
    value = pointer != 0 ? D(pointer + 0x33u) : 0xffffffffu;
    D(destination) = value;
    pointer = D(source + 8u);
    value = pointer != 0 ? D(pointer + 0x33u) : 0xffffffffu;
    D(destination + 4u) = value;
    pointer = D(source + 0xcu);
    value = pointer != 0 ? D(pointer + 0x33u) : 0xffffffffu;
    D(destination + 8u) = value;
    value = D(source);
    D(destination + 0xcu) = value;
    cm_fn_00417c10(destination + 0x10u, work, source + 0x10u);
    return 1;

diagnostic:
    cm_fn_00944cff(0x00a7a088u, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
