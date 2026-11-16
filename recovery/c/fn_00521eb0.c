/* fn_00521eb0, preferred-image VA 0x00521eb0, CM 01/02 3.9.68. */
#include "cmre.h"

extern short * CM_THISCALL cm_fn_00549f70(short *, short, char, u16, u32);

unsigned long long CM_CDECL cm_fn_00521eb0(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5,
                                           u32 arg6, u32 arg7, u32 arg8, u32 arg9)
{
    u8 scratch[8];
    u32 sum;
    u32 index;
    u32 elem;
    u32 edx;
    u16 word0;
    u8 byte2;
    u8 byte7;
    u32 value9;

    sum = arg8 + arg5;
    cm_fn_00549f70((short *)scratch, (short)arg3, (char)arg4, (u16)sum, arg6);
    word0 = *(volatile u16 *)(scratch + 0);
    byte2 = *(volatile u8 *)(scratch + 2);
    index = (u32)(s32)(s16)arg2;
    elem = arg1 + index * 104u;

    *(volatile u16 *)(elem + 0x03) = word0;
    byte7 = *(volatile u8 *)&arg7;
    *(volatile u8 *)(elem + 0x05) = (u8)(byte2 - (u8)arg8);
    value9 = *(volatile u32 *)&arg9;
    *(volatile u8 *)(elem + 0x06) = byte7;
    *(volatile u32 *)(elem + 0x13) = value9;

    edx = (index * 3u) & 0xffff0000u;
    edx = (edx | (u32)word0) & 0xffffff00u;
    edx |= (u32)byte7;
    return ((unsigned long long)edx << 32) | (unsigned long long)elem;
}
