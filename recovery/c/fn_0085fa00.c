#include "cmre.h"

extern s32 CM_THISCALL cm_fn_0054cdb0(u32 self, u32 first, u32 second);

s32 CM_THISCALL cm_fn_0085fa00(u32 self)
{
    u32 second = CM_GLOBAL(volatile u32, 0x00ae2c94u);
    u32 first = CM_GLOBAL(volatile u32, 0x00ae2c90u);
    return cm_fn_0054cdb0(self + 0x2du, first, second);
}
