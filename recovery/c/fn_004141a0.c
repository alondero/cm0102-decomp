/* Preferred-image VA 004141a0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller0082ee6e discards EAX before007a1820 overwrites it at007a1827. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_009326e0(u32 filename, u32 mode, u32 first, u32 second, u32 third);
extern void CM_THISCALL cm_fn_0076a380(volatile u8 *self, u32 stream);
extern u32 CM_THISCALL cm_fn_009314e0(u8 *self, u32 stream);
extern void CM_CDECL cm_fn_00932b00(u32 *stream);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

void CM_THISCALL cm_fn_004141a0(u32 self, u32 filename)
{
    u8 work[0x204];
    u8 *name, *extension;
    u32 line, version, stream, table, object, offset;
    if (filename == 0) {
        name = work + 0x104;
        extension = work + 4;
        line = 0x246u;
        goto diagnostic;
    }
    stream = cm_fn_009326e0(filename, 0x00987e18u, 1, 22, 4);
    D((u32)work) = stream;
    if (stream == 0) {
        name = work + 4;
        extension = work + 0x104;
        line = 0x24eu;
        goto diagnostic;
    }
    for (offset = 0; offset < 0x78u; offset += 4) {
        table = D(self + 8u);
        object = D(table + offset);
        if (object != 0) {
            cm_fn_0076a380((volatile u8 *)object, stream);
            stream = D((u32)work);
        }
    }
    object = D(self + 0x10u);
    if (object != 0) {
        cm_fn_009314e0((u8 *)object, stream);
        stream = D((u32)work);
    }
    object = D(self + 0xcu);
    if (object != 0)
        cm_fn_009314e0((u8 *)object, stream);
    cm_fn_00932b00((u32 *)work);
    return;
diagnostic:
    cm_fn_00944cff(0x00987e24u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
}
