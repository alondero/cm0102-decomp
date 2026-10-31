#include "cmre.h"

u32 CM_THISCALL cm_fn_005cb9b0(u32 self, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f)
{
    *(volatile u8 *)self = (u8)a;
    *(volatile u8 *)(self + 1) = (u8)b;
    *(volatile u8 *)(self + 2) = (u8)c;
    *(volatile u8 *)(self + 0xb) = (u8)d;
    *(volatile u32 *)(self + 0xc) = e;
    *(volatile u32 *)(self + 0x10) = f;
    return e;
}
