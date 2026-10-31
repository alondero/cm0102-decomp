/* Preferred-image VA 0059b3b0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole direct caller discards EAX; its next callee writes EAX before use. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_009326e0(u32 path, u32 mode, u32 one, u32 value, u32 four);
extern u32 CM_CDECL cm_fn_00932e10(u32 buffer, u32 size, u32 count, u32 stream);
extern void CM_CDECL cm_fn_00932b00(void *stream);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

void CM_THISCALL cm_fn_0059b3b0(u32 self, u32 input)
{
    u8 work[0x204];
    u8 *name, *extension;
    u32 handle, count, buffer, result, line, text;
    if (input == 0)
        return;
    handle = cm_fn_009326e0(input, 0x009c87b0u, 1, 0x16, 4);
    *(volatile u32 *)work = handle;
    if (handle == 0) {
        name = work + 0x104;
        extension = work + 4;
        line = 0x115eu;
        goto diagnostic;
    }
    count = D(0x00ae235cu);
    buffer = D(self);
    result = cm_fn_00932e10(buffer, 0x167u, count, handle);
    count = D(0x00ae235cu);
    if (result != count) {
        name = work + 4;
        extension = work + 0x104;
        line = 0x1166u;
        goto diagnostic;
    }
    cm_fn_00932b00(work);
    return;

diagnostic:
    cm_fn_00944cff(0x009c8784u, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
}
