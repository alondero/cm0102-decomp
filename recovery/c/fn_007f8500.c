/* Preferred-image VA 007f8500, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007f8500(u32 self, u32 input)
{
    u8 work[0x200];
    s32 manager;
    u32 pointer, scouts, id, text;
    u8 slot, total, index, record;
    manager = (s16)W(0x00b77c7au);
    pointer = D(0x00b74c26u + (u32)manager * 768u);
    if (pointer == 0)
        return 0;
    slot = B(pointer);
    total = B(0x00ae2364u);
    slot = (u8)(slot - total + 16u);
    if ((s8)slot < 0 || (s8)slot > 15) {
        cm_fn_00944cff(0x00a7b094u, 0, 0, work + 0x100, work);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x100, work, 0x480u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    scouts = D(self + (u32)(s32)(s8)slot * 4u + 0xcu);
    if (scouts == 0)
        return 0;
    id = D(input);
    index = 0;
    do {
        record = index == 0xffu ? B(scouts) : index;
        if (D(scouts + (u32)(s32)(s8)record * 688u + 0x191u) == id)
            return 1;
        index = (u8)(index + 1u);
    } while ((s8)index < 10);
    return 0;
}
