#include "cmre.h"
extern u8 CM_THISCALL cm_fn_004cbb60(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cbd20(u32, u32, u32, u32);
u32 CM_THISCALL cm_fn_004cc0b0(u32 self, u32 first, u32 second)
{
    u8 result = cm_fn_004cbb60(self, first, second);
    if (result != 1 && result != 3) return 0;
    return cm_fn_004cbd20(self, first, second, 0);
}
