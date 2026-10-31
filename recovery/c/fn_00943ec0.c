/* Preferred-image VA 0x00943ec0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_009445d0(u32 allocator, u32 address);

u32 CM_CDECL __attribute__((regparm(1))) cm_fn_00943ec0(u32 incoming_eax, u32 object)
{
    u32 list = D(object + 0x28u), result = incoming_eax;
    if (list) {
        u32 index = 0;
        if ((s16)W(list + 6u) > 0) {
            do {
                u32 array = D(list + 8u);
                u32 allocator = D(list);
                u32 entry = D(array + index * 4u);
                cm_fn_009445d0(allocator, entry);
                ++index;
            } while ((s32)index < (s32)(s16)W(list + 6u));
        }
        {
            u32 array = D(list + 8u);
            u32 allocator = D(list);
            cm_fn_009445d0(allocator, array);
        }
        {
            u32 extra = D(list + 0xcu);
            u32 allocator = D(list);
            cm_fn_009445d0(allocator, extra);
        }
        result = cm_fn_009445d0(D(list), list);
    }
    D(object + 0x28u) = 0;
    return result;
}
