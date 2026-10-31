/* Preferred-image VA 007bd700, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007bd700(u32 self, u32 person, u32 allocate)
{
    u8 work[0x200];
    u32 id, row, value, version;
    u8 index = 0, empty = 0xff;
    /* The later native null branch does not guard this initial read. */
    id = D(person);
    do {
        row = self + (u32)(s32)(s8)index * 418u + 4u;
        value = D(row);
        if (id == value)
            return row;
        if (allocate != 0 && empty == 0xff && value == 0xffffffffu)
            empty = index;
        index = (u8)(index + 1u);
    } while ((s8)index < 50);
    if (allocate == 0)
        return 0;
    if (empty == 0xff) {
        cm_fn_00944cff(0x00a701b0u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x210du);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    row = self + (u32)(s32)(s8)empty * 418u + 4u;
    D(row) = person != 0 ? id : 0xffffffffu;
    W(row + 4u) = 0;
    B(row + 0x1a0u) = 0;
    B(row + 0x1a1u) = 0;
    return row;
}
