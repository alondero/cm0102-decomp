/* Preferred-image VA 0x00418ff0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern void CM_THISCALL cm_fn_004197c0(u32 self, u32 item);
u8 CM_THISCALL cm_fn_00418ff0(u32 self, u32 item)
{
    u32 index, table;
    if (!item)
        return 0;
    index = CM_GLOBAL(volatile u32, item);
    if ((s32)index >= (s32)CM_GLOBAL(volatile u32, 0x00ae2364u))
        return 0;
    table = CM_GLOBAL(volatile u32, self + 0xcu);
    if (CM_GLOBAL(volatile u8, table + index) & 2u) {
        u32 receiver = CM_GLOBAL(volatile u32, 0x00ae1050u);
        u32 result = cm_fn_004cf5a0(receiver, item);
        CM_GLOBAL(volatile u32, 0x00ae1080u) = result;
        if (!result) {
            cm_fn_004197c0(self, item);
        } else {
            u32 id = CM_GLOBAL(volatile u32, result + 4u);
            u32 base = CM_GLOBAL(volatile u32, 0x00ae23b4u);
            if (base + id * 581u == 0)
                cm_fn_004197c0(self, item);
        }
    }
    table = CM_GLOBAL(volatile u32, self + 0xcu);
    index = CM_GLOBAL(volatile u32, item);
    return CM_GLOBAL(volatile u8, table + index) & 2u;
}
