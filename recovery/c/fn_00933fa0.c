#include "cmre.h"

void CM_CDECL cm_fn_00933fa0(void *arg1, void *arg2,
                             s32 arg3, s32 arg4, s32 arg5, volatile s32 arg6)
{
    volatile u32 *A = *(volatile u32 * volatile *)((volatile u8 *)arg1 + 0x3c8);
    volatile u32 *T = *(volatile u32 * volatile *)((volatile u8 *)arg2 + 0x18);
    volatile u32 *home2 = (volatile u32 *)(void *)&arg2;
    volatile u32 *home4 = (volatile u32 *)(void *)&arg4;
    s32 index6 = arg6;
    u32 masked;
    s32 dA, dB, t3, t4m3, m5, combined, threshold, bound, tmp;

    masked = (T[index6] + 0x20u) & 0xffffffc0u;
    dA = (s32)A[arg4];
    t3 = (s32)T[arg3];
    dA = (s32)((u32)dA - A[arg3]);
    dB = (s32)A[index6];
    *home2 = (u32)dA;
    dB = (s32)((u32)dB - A[arg5]);
    if ((s32)(masked - (u32)t3) < 0xc0) goto store_masked;
    if (dA <= -8) goto store_masked;
    if (dB <= -8) goto store_masked;
    tmp = (s32)((u32)dB + (u32)dA);
    t4m3 = (s32)((u32)T[arg4] - (u32)t3);
    m5 = (s32)(masked - T[arg5]);
    combined = (s32)((u32)m5 + (u32)t4m3);
    *home4 = (u32)combined;
    threshold = (s32)((u32)tmp + ((m5 >= 0x20) ? 0u : 0x19u) - 0x20u);
    combined = (s32)*home4;
    if (combined < threshold) goto add_40;
    bound = (s32)((u32)*home2 + (u32)dB + 0x39u);
    if (combined <= bound) goto middle;
    T[arg3] = T[arg3] + 0x40u;
    T[arg6] = masked;
    return;
middle:
    tmp = (m5 >= 0x20) ? 0x19 : 0;
    dB = (s32)((u32)dB - (u32)tmp);
    if (m5 > dB) goto store_masked;
    tmp = (s32)((u32)*home2 + 0x19u);
    if (t4m3 <= tmp) goto store_masked;
    T[arg3] = T[arg3] + 0x40u;
add_40:
    masked = masked + 0x40u;
store_masked:
    T[arg6] = masked;
}
