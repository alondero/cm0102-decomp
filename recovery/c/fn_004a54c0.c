#include "cmre.h"
extern u32 CM_CDECL cm_fn_004b0700(u32 v);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);

u32 CM_THISCALL cm_fn_004a54c0(u32 self, u32 type)
{
    u32 object = *(volatile u32 *)(self + 0x502);
    u32 arg = *(volatile u32 *)(object + 4);
    u32 lookup = CM_GLOBAL(volatile u32, 0x00ae2c74);
    if ((u32)cm_fn_005c6710((void *)lookup, arg))
        return 0;
    if ((s8)type == 8) {
        object = *(volatile u32 *)(self + 0x502);
        if (*(volatile u8 *)(object + 0x42) == 4)
            return 0;
    } else if ((s8)type == 2) {
        object = *(volatile u32 *)(self + 0x502);
        arg = *(volatile u32 *)(object + 4);
        arg = *(volatile u32 *)arg;
        if (!cm_fn_004b0700(arg))
            return 0;
    }
    return 1;
}
