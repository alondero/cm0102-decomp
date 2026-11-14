#include "cmre.h"

CM_THISCALL u32 cm_fn_0055fba0(void *self, u32 p1, u32 p2, u32 p3, u32 p4,
                               u32 p5, u32 p6, u32 p7, u32 p8, u32 p9,
                               u32 p10, u32 p11);

CM_THISCALL u32 cm_fn_00456850(void *self, u32 value)
{
    u16 w = CM_GLOBAL(u16, 0xae31aa);
    s32 sel = *(s16 *)((u8 *)self + 0x382a);
    u32 off = ((u32)sel * 3u) << 8;
    u32 ctx = *(u32 *)((u8 *)self + off + 0x7fc);

    return cm_fn_0055fba0((void *)ctx, 0, 0, 0, 0, 0, 0, 0, 0, 0x130,
                          (u32)w, value);
}
