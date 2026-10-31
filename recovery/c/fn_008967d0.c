/* Preferred-image VA 008967d0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_008982b0(u32 self, u32 object, u32 mode);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008967d0(u32 self, u32 object, s16 index, u32 mode)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 line, text, row, table, first, second, maximum;
    if (object == 0) {
        name = work + 0x100;
        extension = work;
        line = 0x2a5u;
        goto diagnostic;
    }
    if (index < 0)
        goto bad_index;
    first = D(self + 0x5fdu);
    second = D(self + 0x5f5u);
    maximum = first + second - 1u;
    if ((s32)index > (s32)maximum)
        goto bad_index;
    row = cm_fn_008982b0(self, object, mode);
    if (row == 0xffffffffu)
        return 0;
    table = D(self + 0x601u);
    W(table + row * 145u) = (u16)index;
    return 1;

bad_index:
    name = work;
    extension = work + 0x100;
    line = 0x2adu;
diagnostic:
    cm_fn_00944cff(0x00a8b908u, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
