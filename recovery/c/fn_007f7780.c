/* Preferred-image VA 007f7780, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_007f7780(u32 self, u32 record, u32 owner, u32 input_kind)
{
    u8 scratch[0x200];
    u32 line;
    u8 *name = scratch + 0x100;
    u8 *extension = scratch;
    if (!record) { line = 0x2b7; goto error; }
    if (!owner) {
        u32 index = (u32)(s32)(s16)W(0x00b77c7a);
        owner = D(0x00b74c26u + index * 768u);
        if (!owner) {
            line = 0x2c1;
            name = scratch;
            extension = scratch + 0x100;
            goto error;
        }
    }
    u8 value = B(owner);
    u8 base = B(0x00ae2364);
    u8 index = (u8)(value - base + 16);
    if ((s8)index < 0 || (s8)index > 15) {
        line = 0x2cb;
        name = scratch;
        extension = scratch + 0x100;
        goto error;
    }
    u8 kind = (u8)input_kind;
    u32 offset = (u32)index * 4;
    u32 primary = D(self + offset + 0x68);
    if (record == primary && kind == 2) goto swap;
    u32 secondary = D(self + offset + 0xa8);
    if (record == secondary) {
        if (kind == 1) goto swap;
    } else if (kind == 1) {
        D(self + offset + 0x68) = record;
        return 1;
    }
    D(self + offset + 0xa8) = record;
    return 1;
swap:
    secondary = D(self + offset + 0xa8);
    D(self + offset + 0x68) = secondary;
    D(self + offset + 0xa8) = primary;
    return 1;
error:
    cm_fn_00944cff(0x00a7b094, 0, 0, name, extension);
    u32 text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
