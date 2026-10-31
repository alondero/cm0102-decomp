/* Preferred-image VA 008972c0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_STDCALL cm_fn_008982b0(u32 club, u32 flag);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u8 CM_THISCALL cm_fn_008972c0(u32 self, u32 club, u32 person, u32 flag)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 line, version, index, id, pointer, slot;
    if (club == 0) {
        name = work + 0x100;
        extension = work;
        line = 0x48au;
        goto diagnostic;
    }
    if (person == 0) {
        name = work;
        extension = work + 0x100;
        line = 0x492u;
        goto diagnostic;
    }
    index = cm_fn_008982b0(club, flag);
    if (index == 0xffffffffu)
        return 0;
    id = D(person);
    pointer = D(self + 0x601u);
    pointer += index * 145u + 2u;
    for (slot = 0; slot < 20; ++slot) {
        if (D(pointer) == id)
            return (u8)slot;
        pointer += 4;
    }
    return 0xff;
diagnostic:
    cm_fn_00944cff(0x00a8b908u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
