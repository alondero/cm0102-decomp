/* Preferred-image VA 0x0055cbc0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_0058f120(u32 self, u32 first, u32 second);

u32 CM_THISCALL cm_fn_0055cbc0(u32 self, u32 index)
{
    u32 base = D(0x00ae23bcu);
    u32 record = base + index * 110u;
    u32 value, limit, first, second, receiver;
    if (record) {
        value = D(record);
        limit = D(0x00ae2364u);
        if ((s32)value >= (s32)limit) return 0;
    }
    base = D(self + 0x14u);
    receiver = D(self + 0x18u);
    record = base + index * 29u;
    second = D(record + 0x18u);
    first = D(record + 0x14u);
    return cm_fn_0058f120(receiver, first, second);
}
