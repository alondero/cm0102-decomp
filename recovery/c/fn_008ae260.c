#include "cmre.h"

extern u32 CM_STDCALL cm_fn_00944c2e(u32 version, u32 output);
extern u32 CM_THISCALL cm_fn_008af290(u32 self);

u32 CM_THISCALL cm_fn_008ae260(volatile u8 *self)
{
    if (*(volatile u32 *)(self + 0x4b6) == 0) {
        if (cm_fn_00944c2e(0x101, (u32)self + 0x328) != 0)
            return 0;
        *(volatile u32 *)(self + 0x4b6) = 1;
    }
    return cm_fn_008af290((u32)self) != 0;
}
