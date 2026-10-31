/* Preferred-image VA 008ab670, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u8 CM_THISCALL cm_fn_008ab670(u32 self, s8 slot, volatile u32 output,
                             volatile u32 external)
{
    u8 work[0x200];
    u32 pointer, object, record, index, version;
    u8 cursor, value;
    if (slot < 0 || slot > 10) {
        cm_fn_00944cff(0x00a8b94cu, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x1e2bu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0xffu;
    }
    if (external != 0) {
        if (D(self + 0xde5u) == 0)
            return 0xffu;
    }
    pointer = output;
    D(pointer) = 0;
    object = D(self + 4u);
    if (object == 0)
        return (u8)slot;
    cursor = 0;
    do {
        if (external != 0) {
            index = (u32)(s32)(s8)B(self + 0xdeau);
            index = (u32)(s32)(s8)cursor + index * 20u;
            record = object + index * 446u + 0x4796u;
        } else {
            index = (u32)(s32)(s8)cursor;
            record = self + index * 446u + 0x2735u;
        }
        value = B(record + 0x19u);
        if (value == (u8)slot)
            return cursor;
        if (value == 0xfeu && B(record + 0x1au) == (u8)slot) {
            D(pointer) = 1;
            return cursor;
        }
        cursor = (u8)(cursor + 1u);
    } while ((s8)cursor < 20);
    return 0xffu;
}
