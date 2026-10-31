#include "cmre.h"

u32 CM_THISCALL cm_fn_006cf710(u32 self, u32 selector, u32 amount)
{
    u32 index = (u32)(s32)(s8)selector;
    u32 address = self + index * 8920u + 0x4796u + 0x35u;
    u32 i;
    for (i = 0; i < 20u; ++i) {
        s8 flag = *(volatile s8 *)(address - 0x1cu);
        if (flag >= 0) {
            u16 value = *(volatile u16 *)address;
            *(volatile u16 *)address = (u16)(value + (u16)amount);
        }
        address += 0x1beu;
    }
    return address;
}
