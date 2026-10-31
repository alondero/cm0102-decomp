#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);

u32 CM_THISCALL cm_fn_004cca30(u32 self, volatile u32 key)
{
    u32 list = *(volatile u32 *)(self + 0x34u);
    u32 current, wanted;
    if (list == 0)
        return 0;
    current = *(volatile u32 *)(list + 2u);
    *(volatile u32 *)(list + 10u) = current;
    list = *(volatile u32 *)(self + 0x34u);
    current = cm_fn_0068d1b0(list);
    if (current == 0)
        return 0;
    wanted = key;
    do {
        if (*(volatile u32 *)current == wanted && *(volatile u8 *)(current + 0x50u) == 2)
            return current;
        list = *(volatile u32 *)(self + 0x34u);
        current = cm_fn_0068d1b0(list);
    } while (current != 0);
    return 0;
}
