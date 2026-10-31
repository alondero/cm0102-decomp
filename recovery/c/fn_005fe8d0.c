#include "cmre.h"
extern u32 CM_STDCALL cm_fn_005fe700(u32 packet, u32 person);

u32 CM_THISCALL cm_fn_005fe8d0(u32 self)
{
    u32 i, offset = 0;
    for (i = 0; i < 16; ++i) {
        u32 index = CM_GLOBAL(volatile u32, 0x00ae2364) + i - 16;
        u32 person = CM_GLOBAL(volatile u32, 0x00ae23bc) + index * 110;
        if (*(volatile u8 *)(person + 0x5f)) {
            u32 packet = *(volatile u32 *)self + offset + 0x9ad4;
            if (!cm_fn_005fe700(packet, person)) {
                u32 p = *(volatile u32 *)self + offset + 0x9ad4;
                u32 j;
                for (j = 0; j < 19; ++j)
                    *(volatile u32 *)(p + j * 4) = 0;
                *(volatile u16 *)(p + 76) = 0;
                *(volatile u8 *)(p + 78) = 0;
            }
        }
        offset += 0xa6ef;
    }
    return 1;
}
