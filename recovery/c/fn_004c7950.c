/* Preferred-image VA 0x004c7950, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_004c79a0(u32 self, u32 flag);
extern u32 CM_THISCALL cm_fn_0085e960(u32 self, u32 record, u32 argument);

u32 CM_THISCALL cm_fn_004c7950(u32 self, u32 record, u32 argument)
{
    u32 slot = cm_fn_004c79a0(self, 1);
    u32 index, table, entries;
    if ((s32)slot < 0) return slot;
    index = D(record);
    table = D(0x00ae2d0cu);
    D(table + index * 79u) = slot;
    index = D(record);
    entries = D(self);
    D(entries + slot * 80u) = index;
    entries = D(self);
    return cm_fn_0085e960(entries + slot * 80u, record, argument);
}
