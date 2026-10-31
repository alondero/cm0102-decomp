#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0051a150(u32 self, u32 input, u32 argument);
extern u32 CM_THISCALL cm_fn_009058b0(u32 self, u32 object, u32 mode);

u32 CM_THISCALL cm_fn_00633290(u32 self, u32 input, u32 argument)
{
    s8 a = *(volatile s8 *)(input + 0x47);
    s8 b;
    u32 object, receiver;
    if ((u8)a != 0xff)
        b = *(volatile s8 *)(input + 0x48);
    else {
        a = *(volatile s8 *)(input + 0x43);
        b = *(volatile s8 *)(input + 0x44);
    }
    if (a > b) {
        object = *(volatile u32 *)(input + 0x1c);
        object = *(volatile u32 *)(object + 0x53);
        receiver = CM_GLOBAL(volatile u32, 0x00de1f58);
        cm_fn_009058b0(receiver, object, 2);
    } else if (a < b) {
        object = *(volatile u32 *)(input + 0x20);
        object = *(volatile u32 *)(object + 0x53);
        receiver = CM_GLOBAL(volatile u32, 0x00de1f58);
        cm_fn_009058b0(receiver, object, 2);
    } else {
        object = *(volatile u32 *)(input + 0x1c);
        object = *(volatile u32 *)(object + 0x53);
        receiver = CM_GLOBAL(volatile u32, 0x00de1f58);
        cm_fn_009058b0(receiver, object, 1);
        object = *(volatile u32 *)(input + 0x20);
        object = *(volatile u32 *)(object + 0x53);
        receiver = CM_GLOBAL(volatile u32, 0x00de1f58);
        cm_fn_009058b0(receiver, object, 1);
    }
    return cm_fn_0051a150(self, input, argument);
}
