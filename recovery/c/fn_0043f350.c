#include "cmre.h"

void CM_CDECL cm_fn_0043f350(volatile u32 *data, u32 count)
{
    u32 address;

    if (count == 0) {
        return;
    }
    address = (u32)data;
    do {
        u32 low = *(volatile u32 *)address;
        u32 high = *(volatile u32 *)(address + 4u);
        unsigned long long input = ((unsigned long long)high << 32) | low;
        unsigned long long left;
        unsigned long long first_right;
        unsigned long long second_right;
        u32 out_low;
        u32 out_high;

        left = input << 16;
        left |= low & 0x0000ff00u;
        left = (left << 16) | (low & 0x00ff0000u);
        left = (left << 16) | (low & 0xff000000u);
        left <<= 8;
        out_low = (u32)left;
        out_high = (u32)(left >> 32);

        first_right = (unsigned long long)(((long long)input) >> 16);
        second_right = ((unsigned long long)((u32)first_right & 0xff000000u)
                        | ((unsigned long long)(high & 0xffu) << 32)) >> 8;
        out_low |= (u32)second_right;
        out_high |= (u32)(second_right >> 32);
        out_low |= ((u32)((s32)high >> 24) & 0xffu);
        out_low |= ((u32)((s32)high >> 8) & 0xff00u);

        *(volatile u32 *)address = out_low;
        *(volatile u32 *)(address + 4u) = out_high;
        address += 8u;
        --count;
    } while (count != 0);
}
