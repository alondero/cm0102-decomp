/* Preferred-image VA 0x0084aeb0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_THISCALL cm_fn_0084aeb0(u32 self, u32 source, u32 target)
{
    u32 base = CM_GLOBAL(volatile u32, self + 0xdcu);
    u32 index = CM_GLOBAL(volatile u32, source);
    u32 entry = base + index * 9u;
    u8 flag = CM_GLOBAL(volatile u8, entry + 4u);
    u32 result = (index & 0xffffff00u) | flag;
    if (flag == 1) {
        u32 offset = 0;
        u32 i;
        for (i = 0; i < 40u; ++i) {
            u32 item = CM_GLOBAL(volatile u32, entry) + offset;
            u32 id = CM_GLOBAL(volatile u32, item + 0x0fu);
            u32 desired = CM_GLOBAL(volatile u32, target);
            if (id == desired)
                CM_GLOBAL(volatile u32, item + 0x17u) = 0xffffffffu;
            result = item;
            offset += 0x25u;
        }
    }
    return result;
}
