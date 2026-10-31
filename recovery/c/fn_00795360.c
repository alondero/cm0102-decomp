/* Preferred-image VA 0x00795360, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_004a7f20(u32 self, u32 first, u32 second);
extern void CM_STDCALL cm_fn_00795e20(s32 index, volatile s32 *item);
u32 CM_THISCALL cm_fn_00795360(u32 self)
{
    u32 slot = self, result = 0, group;
    for (group = 0; group < 16u; ++group, slot += 8u) {
        u32 list = CM_GLOBAL(volatile u32, slot);
        u32 cursor = CM_GLOBAL(volatile u32, list + 2u);
        s32 index = 0;
        result = list;
        CM_GLOBAL(volatile u32, list + 0xau) = cursor;
        list = CM_GLOBAL(volatile u32, slot);
        if ((s16)CM_GLOBAL(volatile u16, list) > 0) {
            do {
                u32 item, first, second;
                list = CM_GLOBAL(volatile u32, slot);
                item = cm_fn_0068d1b0(list);
                first = CM_GLOBAL(volatile u32, 0x00ae2c90u);
                second = CM_GLOBAL(volatile u32, 0x00ae2c94u);
                if (cm_fn_004a7f20(item + 0x10bu, first, second))
                    cm_fn_00795e20((s32)group, (volatile s32 *)item);
                list = CM_GLOBAL(volatile u32, slot);
                ++index;
                result = (u32)(s32)(s16)CM_GLOBAL(volatile u16, list);
            } while (index < (s32)result);
        }
    }
    return result;
}
