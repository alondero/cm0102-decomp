/* Preferred-image VA 00766f50, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_00932c30(u32 buffer, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00766f50(u32 self, u32 stream)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 result, text, line, description;
    result = cm_fn_00932c30(self + 0x1du, 11, 12, stream);
    if (result != 12) {
        name = work + 0x100;
        extension = work;
        line = 0x6eu;
        description = 0x00a5abb0u;
        goto diagnostic;
    }
    result = cm_fn_00932c30(self + 0xa1u, 1, 1, stream);
    if (result == 1)
        return 1;
    name = work;
    extension = work + 0x100;
    line = 0x75u;
    description = 0x00a5ab78u;
diagnostic:
    cm_fn_00944cff(0x00a5abe4u, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009a8120u, text, name, extension, line, description);
    cm_fn_0066f4e0(0x00ae28bcu, 0x009af3e0u);
    cm_fn_0066f4e0(0x00ae26c4u, 0x009af334u, 0x00ae24d0u);
    cm_fn_005e8290(0x00ae28bcu, 0x00ae26c4u, 0);
    return 0;
}
