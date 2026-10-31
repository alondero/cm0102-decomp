#include "cmre.h"

typedef u32 (CM_THISCALL *method_fn)(u32 self, u32 first, u32 second, u32 third);

u32 CM_THISCALL cm_fn_0048e190(u32 self, u32 first, u32 second, u32 third)
{
    u8 selector = *(volatile u8 *)(first + 0x42u);
    u32 receiver = self;
    u32 table;
    method_fn method;
    if (selector != 0xffu) {
        u32 entries = *(volatile u32 *)(self + 12u);
        receiver = *(volatile u32 *)(entries + (u32)(s32)(s8)selector * 4u);
    }
    table = *(volatile u32 *)receiver;
    method = (method_fn)*(volatile u32 *)(table + 0x54u);
    return method(receiver, first, second, third);
}
