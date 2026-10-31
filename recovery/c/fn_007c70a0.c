/* Preferred-image VA 0x007c70a0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);
void CM_THISCALL cm_fn_007c70a0(u32 self)
{
    u32 base = CM_GLOBAL(volatile u32, self);
    u32 index = 0, offset = 0;
    s32 count;
    if (!base)
        return;
    count = (s32)CM_GLOBAL(volatile u32, self + 0x18u);
    if (count > 0) {
        do {
            u32 field;
            base = CM_GLOBAL(volatile u32, self);
            field = CM_GLOBAL(volatile u32, base + offset);
            if (field)
                cm_fn_009452ca(field);
            base = CM_GLOBAL(volatile u32, self);
            field = CM_GLOBAL(volatile u32, base + offset + 0x0eu);
            if (field)
                cm_fn_009452ca(field);
            count = (s32)CM_GLOBAL(volatile u32, self + 0x18u);
            ++index;
            offset += 0x12u;
        } while ((s32)index < count);
    }
    base = CM_GLOBAL(volatile u32, self);
    cm_fn_009452ca(base);
}
