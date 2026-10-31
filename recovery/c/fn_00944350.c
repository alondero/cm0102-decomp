/* Preferred-image VA 0x00944350, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 address);
u32 CM_CDECL __attribute__((regparm(1))) cm_fn_00944350(u32 incoming_eax, u32 object)
{
    u32 count, array, index, value, result;
    if (!object) return incoming_eax;
    count = *(volatile u32 *)(object + 8u);
    array = *(volatile u32 *)(object + 0xcu);
    if ((s32)count > 0) {
        for (index = 0; index < count; ++index) {
            value = *(volatile u32 *)(array + index * 4u);
            if (value) cm_fn_009452ca(value);
        }
    }
    cm_fn_009452ca(array);
    result = (u32)cm_fn_009452ca(object);
    return result;
}
