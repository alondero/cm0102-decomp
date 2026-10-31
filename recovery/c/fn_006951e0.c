#include "cmre.h"

extern void CM_THISCALL cm_fn_00695240(u32 self, u32 object, u32 record, u32 arg, u32 mode);

void CM_THISCALL cm_fn_006951e0(u32 self, volatile u8 *record, u32 arg)
{
    u32 object;
    if (*(volatile u32 *)record == 0xfffffffeu || record[0x40] == 0)
        return;
    object = *(volatile u32 *)(record + 0x1c);
    if (*(volatile u32 *)(object + 0xcf))
        cm_fn_00695240(self, object, (u32)record, arg, 1);
    object = *(volatile u32 *)(record + 0x20);
    if (*(volatile u32 *)(object + 0xcf))
        cm_fn_00695240(self, object, (u32)record, arg, 0);
}
