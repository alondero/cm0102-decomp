#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_0055f990(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_00415640(u32 self, u32 a, u32 b, u32 c, u32 d)
{
    u32 eax1 = (c & 0xffff0000u) | W(0x00aebdea);
    u32 edx1 = (b & 0xffff0000u) | W(0x00aebdec);
    u32 eax2 = (c & 0xffff0000u) | W(0x00ae31aa);
    u32 index = (u32)(s32)(s16)W(self + 0x382a);
    u32 receiver = D(self + 0x7fcu + index * 0x300u);
    return cm_fn_0055f990(receiver, 2, 0, 0, 0, 0, 0, 0, 0x10,
                          eax2, edx1, 1, 1, eax1, b, 0, c, d, a);
}
