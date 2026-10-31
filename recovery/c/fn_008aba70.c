#include "cmre.h"
extern void CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2, u32 buffer1, u32 buffer2);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void cm_fn_005e8290(u32 message, u32 buffer, u32 mode);
extern u8 CM_THISCALL cm_fn_006d4330(u32 self, u8 value, u8 index);

u8 CM_THISCALL cm_fn_008aba70(u32 self, u8 index, u32 pointer)
{
    volatile u8 scratch[0x200];
    if ((s8)index < 0 || (s8)index > 0x13) {
        cm_fn_00944cff(0x00a8b94cu, 0u, 0u, (u32)&scratch[0x100], (u32)&scratch[0]);
        {
            u32 text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                           (u32)&scratch[0x100], (u32)&scratch[0], 0x1ee5u);
        }
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0u);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    } else {
        u32 receiver;
        if (pointer != 0u && *(volatile u32 *)(self + 0xde5u) == 0u)
            goto return_ff;
        receiver = *(volatile u32 *)(self + 4u);
        if (receiver == 0u)
            goto return_ff;
        u8 value;
        if (pointer != 0u)
            value = *(volatile u8 *)(self + 0xdeau);
        else
            value = *(volatile u8 *)(self + 0xde9u);
        return cm_fn_006d4330(receiver, value, index);
    }
return_ff:
    return 0xffu;
}



