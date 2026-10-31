/* Preferred-image VA 0x004197c0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);
void CM_THISCALL cm_fn_004197c0(u32 self, u32 item)
{
    u32 table, address, list, cursor, entry;
    u8 flag;
    s32 index;
    if (!item)
        return;
    table = CM_GLOBAL(volatile u32, self + 0xcu);
    if (!table)
        return;
    address = table + CM_GLOBAL(volatile u32, item);
    flag = CM_GLOBAL(volatile u8, address);
    if (!(flag & 2u))
        return;
    CM_GLOBAL(volatile u8, address) = flag ^ 2u;
    list = CM_GLOBAL(volatile u32, self + 4u);
    if (!list)
        return;
    cursor = CM_GLOBAL(volatile u32, list + 2u);
    CM_GLOBAL(volatile u32, list + 0xau) = cursor;
    list = CM_GLOBAL(volatile u32, self + 4u);
    index = 0;
    if ((s16)CM_GLOBAL(volatile u16, list) <= 0)
        return;
    do {
        entry = cm_fn_0068d1b0(list);
        if (!entry)
            return;
        cursor = CM_GLOBAL(volatile u32, entry);
        if (cursor == CM_GLOBAL(volatile u32, item)) {
            list = CM_GLOBAL(volatile u32, self + 4u);
            cm_fn_0068d1f0((u8 *)list, (const u8 *)entry);
            return;
        }
        list = CM_GLOBAL(volatile u32, self + 4u);
        ++index;
    } while (index < (s16)CM_GLOBAL(volatile u16, list));
}
