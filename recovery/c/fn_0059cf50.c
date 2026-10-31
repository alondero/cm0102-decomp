/* Preferred-image VA 0x0059cf50, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0053b900(u32 input);
extern void CM_FASTCALL cm_fn_00598370(u32 self);

u32 CM_THISCALL cm_fn_0059cf50(u32 self)
{
    u32 count = D(0x00ae235cu);
    u32 index = 0, offset = 0, destination = D(self);
    if ((s32)count <= 0) return count;
    do {
        u32 base = D(0x00ae23b4u);
        u32 result = cm_fn_0053b900(base + offset);
        if (result) return result;
        cm_fn_00598370(destination);
        count = D(0x00ae235cu);
        ++index;
        offset += 0x245u;
        destination += 0x167u;
    } while ((s32)index < (s32)count);
    return count;
}
