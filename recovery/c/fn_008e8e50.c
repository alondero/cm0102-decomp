#include "cmre.h"
extern u32 CM_STDCALL cm_fn_008e8c40(u32 arg1, u32 arg2);

u32 CM_THISCALL cm_fn_008e8e50(u32 self, u32 value, u32 *out)
{
    s8 count = *(volatile s8 *)(self + 8);
    s8 i = 0;
    if (count > 0) {
        do {
            u32 base = *(volatile u32 *)(self + 4);
            u32 entry = base + (u32)(s32)i * 12;
            if (cm_fn_008e8c40(value, entry)) {
                if (out) {
                    base = *(volatile u32 *)(self + 4);
                    *(volatile u32 *)out = base + (u32)(s32)i * 12;
                }
                return 1;
            }
            count = *(volatile s8 *)(self + 8);
            i = (s8)(i + 1);
        } while (i < count);
    }
    if (count != 0)
        return 0;
    if (out)
        *(volatile u32 *)out = 0;
    return 1;
}
