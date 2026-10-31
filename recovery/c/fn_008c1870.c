/* Preferred-image VA 0x008c1870, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
u32 CM_THISCALL cm_fn_008c1870(u32 self, u32 target)
{
    u32 list = self + 0x92cu;
    u32 first = CM_GLOBAL(volatile u32, self + 0x92eu);
    u32 entry;
    CM_GLOBAL(volatile u32, list + 0xau) = first;
    entry = cm_fn_0068d1b0(list);
    while (entry) {
        u32 desired = CM_GLOBAL(volatile u32, target);
        u32 current = CM_GLOBAL(volatile u32, entry);
        if (desired == current)
            return 1;
        entry = cm_fn_0068d1b0(list);
    }
    return 0;
}
