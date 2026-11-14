#include "cmre.h"

extern short CM_THISCALL cm_fn_0055f990(int param_1, u32 param_2, u32 param_3,
    u32 param_4, u32 param_5, u32 param_6, u32 param_7, u32 param_8, u32 param_9,
    u32 param_10, u32 param_11, u32 param_12, u32 param_13, u32 param_14, u32 param_15,
    u32 param_16, u32 param_17, u32 param_18, u32 param_19);

void CM_THISCALL cm_fn_00803f30(u32 self, u32 arg1, u32 arg2, u32 arg3)
{
    s32 index = (s32)*(const s16 *)(self + 0x382au);
    u32 offset = ((u32)index * 3u) << 8;
    u32 receiver = *(const u32 *)(self + 0x7fcu + offset);

    cm_fn_0055f990((int)receiver, 1u, 0x64u, 0xau, 0x316u, 0x46u, 0u, 0u, 0x30u,
        arg1, 0u, 0xcu, 7u, arg2, arg3, 0u, 0u, 0u, 0xffffffffu);
}
