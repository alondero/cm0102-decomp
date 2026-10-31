/* Preferred-image VA 007f82f0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller00482d72 overwrites full EAX at00482d77. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_007b3d50(u32 self, u32 value);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

void CM_THISCALL cm_fn_007f82f0(u32 self, u32 person)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 line, version, index, pointer, row, value;
    u8 first, total, period;
    if (person == 0) {
        name = work + 0x100;
        extension = work;
        line = 0x415u;
        goto diagnostic;
    }
    index = (u32)(s32)(s16)W(0x00b77c7au);
    pointer = D(0x00b74c26u + index * 768u);
    if (pointer == 0)
        return;
    first = B(pointer);
    total = B(0x00ae2364u);
    period = (u8)(first - total + 16u);
    if ((s8)period < 0 || (s8)period > 15) {
        name = work;
        extension = work + 0x100;
        line = 0x427u;
        goto diagnostic;
    }
    pointer = D(self + (u32)(s32)(s8)period * 4u + 0xcu);
    if (pointer == 0)
        return;
    index = (u32)(s32)(s8)B(pointer);
    row = pointer + index * 688u;
    if (D(row + 0x191u) != 0xfffffffeu)
        return;
    value = D(person);
    cm_fn_007b3d50(row + 1u, value);
    return;
diagnostic:
    cm_fn_00944cff(0x00a7b094u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
}
