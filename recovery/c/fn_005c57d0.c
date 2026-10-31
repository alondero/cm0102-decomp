/* Preferred-image VA 0x005c57d0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
void CM_STDCALL cm_fn_005c57d0(u32 self, u32 value)
{
    s8 count = (s8)CM_GLOBAL(volatile u8, self + 8u);
    s32 index = 0;
    if (count <= 0)
        return;
    do {
        u32 slot = self + ((u32)index + 1u) * 25u;
        if (!CM_GLOBAL(volatile u32, slot)) {
            CM_GLOBAL(volatile u32, slot) = value;
            CM_GLOBAL(volatile u8, self + (u32)index * 25u + 0x21u) = 1;
            return;
        }
        ++index;
    } while (index < count);
}
