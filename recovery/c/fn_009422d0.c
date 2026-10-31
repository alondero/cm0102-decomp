/* Preferred-image VA 0x009422d0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_00941630(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);

u32 CM_CDECL cm_fn_009422d0(u32 self, u32 key)
{
    u32 root = D(self), index = 0, array, item;
    s32 count = (s16)W(root + 8u);
    if (count <= 0) return 0;
    array = D(root + 0x10u);
    do {
        item = D(array + index * 4u);
        if (D(item + 4u) == key) {
            u32 a, b, c, d;
            item = D(array + index * 4u);
            if (!item) return 0;
            d = D(item + 0x10u);
            c = D(item + 0xcu);
            b = D(self + 0x54u);
            a = D(self + 0x5cu);
            return cm_fn_00941630(a, b, c, d, 0, 0);
        }
        ++index;
    } while ((s32)index < count);
    return 0;
}
