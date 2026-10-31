#include "cmre.h"

extern u32 CM_THISCALL cm_fn_008b53d0(u32 self, u32 arg1);
extern u32 CM_THISCALL cm_fn_005fcb60(u32 self, u32 path, u32 person, u8 type);

u32 CM_THISCALL cm_fn_008b0e70(u32 self, u32 arg)
{
    u32 index = (u32)(s32)*(volatile s8 *)(self + 0x7d9u);
    u32 result = cm_fn_008b53d0(self + index * 155u + 0xfau, arg);
    u32 receiver;
    if (result == 0)
        return 0;
    receiver = CM_GLOBAL(volatile u32, 0x00b63c98);
    cm_fn_005fcb60(receiver, arg, 0, 3);
    return 1;
}
