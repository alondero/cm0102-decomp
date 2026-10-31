#include "cmre.h"
extern u32 CM_CDECL cm_fn_0053b3d0(u32 nation);
extern u32 CM_CDECL cm_fn_0053b4f0(u32 nation);

u32 CM_STDCALL cm_fn_0076da80(u32 self, u32 nation, u32 kind)
{
    u32 selected;
    if (!*(volatile u32 *)(self + 0x24))
        return 0;
    if ((u8)kind == 0) {
        selected = cm_fn_0053b3d0(nation);
        if (*(volatile u32 *)(self + 0x24) == selected &&
            (*(volatile u8 *)(self + 0x5e) & 4))
            return 1;
    } else if ((u8)kind == 1) {
        selected = cm_fn_0053b3d0(nation);
        if (*(volatile u32 *)(self + 0x24) == selected &&
            (*(volatile u8 *)(self + 0x5e) & 8))
            return 1;
    } else if ((u8)kind == 2) {
        selected = cm_fn_0053b4f0(nation);
        if (*(volatile u32 *)(self + 0x24) == selected &&
            (*(volatile u8 *)(self + 0x5e) & 4))
            return 1;
    }
    return 0;
}
