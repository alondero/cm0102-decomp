/* Preferred-image VA 0x00933170, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern s32 CM_CDECL cm_fn_00946aae(u32 file);

u32 CM_CDECL cm_fn_00933170(u32 object)
{
    u32 index, offset, value;
    if (!object) return 0xffffffffu;
    if (D(object + 0x19u) == 4) {
        index = D(object + 0xcu);
        offset = index * 67u * 4u;
        value = D(object + 8u);
        return D(object + offset + 0x25u) + value;
    }
    value = (u32)cm_fn_00946aae(D(object));
    index = D(object + 0xcu);
    offset = D(object + index * 67u * 4u + 0x25u);
    return value - offset;
}
