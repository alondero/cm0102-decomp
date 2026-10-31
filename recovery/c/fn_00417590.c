/* Preferred-image VA 00417590, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_00932c30(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00417590(u32 self, u32 file)
{
    u8 scratch[0x200];
    u32 line;
    u8 *name = scratch;
    u8 *extension = scratch + 0x100;
    u32 result = cm_fn_00932c30(self + 4, 4, 1, file);
    if (result != 1) {
        line = 0x67;
        name = scratch + 0x100;
        extension = scratch;
        goto error;
    }
    u16 count = W(self + 4);
    if (count) {
        u32 table = D(self);
        result = cm_fn_00932c30(table, 0xe, (u32)(s32)(s16)count, file);
        s32 expected = (s16)W(self + 4);
        if (result != (u32)expected) {
            line = 0x71;
            goto error;
        }
    }
    result = cm_fn_00932c30(self + 8, 2, 16, file);
    if (result != 16) {
        line = 0x7a;
        goto error;
    }
    result = cm_fn_00932c30(self + 0x38, 8, 1, file);
    if (result == 1) return result;
    line = 0x81;
error:
    cm_fn_00944cff(0x009880b0, 0, 0, name, extension);
    u32 text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}

