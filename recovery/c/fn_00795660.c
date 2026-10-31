/* Preferred-image VA 0x00795660, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32 self, u32 target);

u32 CM_THISCALL cm_fn_00795660(u32 self, u32 first, u32 second)
{
    u32 index = D(0x00ae2364u) - D(first) - 1u;
    u32 result = first, list, cursor, item, counter = 0;
    if ((s32)index < 0 || (s32)index >= 16) return result;
    list = D(self + index * 8u);
    result = list;
    cursor = D(list + 2u);
    D(list + 0xau) = cursor;
    list = D(self + index * 8u);
    if ((s16)W(list) <= 0) return result;
    for (;;) {
        item = cm_fn_0068d1b0(list);
        if (!item) return 0;
        if (D(item) == D(second)) {
            list = D(self + index * 8u);
            return cm_fn_0068d1f0(list, item);
        }
        list = D(self + index * 8u);
        ++counter;
        result = (u32)(s32)(s16)W(list);
        if ((s32)counter >= (s32)result) return result;
    }
}
