/* Preferred-image VA 0x00569e30; image base 0x00400000; pinned target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 param_2);
extern void CM_THISCALL cm_fn_0056a090(u32 self, u32 param_2);

u8 CM_THISCALL cm_fn_00569e30(u32 self, u32 param_2, u32 param_3)
{
    s32 index;
    s32 limit;
    u32 table;
    u32 base;
    u32 result;
    u32 scaled;
    u8 first;

    if (param_2 == 0)
        return 0;
    index = (s32)*(volatile u32 *)param_2;
    limit = (s32)CM_GLOBAL(volatile u32, 0x00ae2364);
    if (index >= limit)
        return 0;
    table = *(volatile u32 *)(self + 0x04u);
    first = *(volatile u8 *)(table + (u32)index);
    if (first != 0 && CM_GLOBAL(volatile u8, 0x00ae233c) == 0) {
        cm_fn_0056a090(self, param_2);
        return 0;
    }
    if (param_3 != 0)
        return first;
    if (first != 0) {
        result = cm_fn_004cf5a0(CM_GLOBAL(volatile u32, 0x00ae1050), param_2);
        CM_GLOBAL(volatile u32, 0x00ae1080) = result;
        if (result == 0) {
            cm_fn_0056a090(self, param_2);
            return 0;
        }
        base = *(volatile u32 *)(result + 0x04u);
        table = CM_GLOBAL(volatile u32, 0x00ae23b4) + base;
        scaled = base + base * 8u;
        scaled = (scaled << 4) + base;
        table = table + scaled * 4u;
        if (table == 0) {
            cm_fn_0056a090(self, param_2);
            return 0;
        }
    }
    base = *(volatile u32 *)param_2;
    table = *(volatile u32 *)(self + 0x04u);
    return *(volatile u8 *)(table + base);
}
