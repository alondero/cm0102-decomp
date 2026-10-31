/* Preferred-image VA 00897590, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_008982b0(u32 self, u32 object, u32 mode);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u8 CM_THISCALL cm_fn_00897590(u32 self, u32 object, u32 mode, u32 flag)
{
    u8 work[0x200];
    u32 text, row, table, clubs, id, expected, actual;
    u8 index = 0;
    if (object == 0) {
        cm_fn_00944cff(0x00a8b908u, 0, 0, work + 0x100, work);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x100, work, 0x4f5u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    row = cm_fn_008982b0(self, object, mode);
    if (row == 0xffffffffu)
        return 0;
    table = D(self + 0x601u);
    row *= 145u;
    clubs = D(0x00ae23bcu);
    do {
        id = D(table + row + (u32)(s32)(s8)index * 4u + 2u);
        if (id != 0xffffffffu) {
            expected = D(table + row + 0x52u);
            actual = D(clubs + id * 110u);
            if (actual == expected)
                return index;
        }
        index = (u8)(index + 1u);
    } while ((s8)index < 20);
    return flag != 0 ? 0xffu : 0;
}
