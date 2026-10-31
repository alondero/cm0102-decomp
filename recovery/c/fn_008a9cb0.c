#include "cmre.h"

extern u32 CM_THISCALL cm_fn_005b4120(int param_1, u32 param_2, u32 *param_3);
extern u32 CM_THISCALL cm_fn_005b3de0(int param_1, u32 param_2, int param_3, u32 *param_4);

/* Thiscall with no stack arguments; returns 1 on the two completing paths and 0 otherwise.
   Fields are used by offset: +0xe40 must be zero, +0x2727 byte must not be 1, +0xc4dd byte
   must be nonzero. Then the block at +0xe44 is copied forward to +0x4a63 (0xf07 dwords,
   then a word and a byte; the two blocks do not overlap) and the flag dword at +0xc4de is
   set to 1. When the byte at +0xc4dd is 2, 0x005b4120 is called with the receiver at
   +0xe44; otherwise 0x005b3de0 is called with the dword at +0x2729. The first argument is
   1 when the byte at +0x2727 equals 2. Both calls leave the dword at +0x4a5f set to 1. */
u32 CM_THISCALL cm_fn_008a9cb0(u8 *param_1)
{
    u32 i;
    u8 dl;
    u32 flag;

    if (*(u32 *)(param_1 + 0xe40) != 0) {
        return 0;
    }
    if (*(u8 *)(param_1 + 0x2727) == 1) {
        return 0;
    }
    if (*(u8 *)(param_1 + 0xc4dd) == 0) {
        return 0;
    }
    for (i = 0; i < 0xf07U; i++) {
        *(u32 *)(param_1 + 0x4a63 + 4 * i) = *(u32 *)(param_1 + 0xe44 + 4 * i);
    }
    *(u16 *)(param_1 + 0x4a63 + 0x3c1c) = *(u16 *)(param_1 + 0xe44 + 0x3c1c);
    *(u8 *)(param_1 + 0x4a63 + 0x3c1e) = *(u8 *)(param_1 + 0xe44 + 0x3c1e);

    dl = *(u8 *)(param_1 + 0xc4dd);
    *(u32 *)(param_1 + 0xc4de) = 1;
    if (dl == 2) {
        flag = (*(u8 *)(param_1 + 0x2727) == 2) ? 1U : 0U;
        cm_fn_005b4120((int)(param_1 + 0xe44), flag, (u32 *)(param_1 + 0xc2cd));
        *(u32 *)(param_1 + 0x4a5f) = 1;
        return 1;
    }
    flag = (*(u8 *)(param_1 + 0x2727) == 2) ? 1U : 0U;
    cm_fn_005b3de0((int)(param_1 + 0xe44), flag, (int)*(u32 *)(param_1 + 0x2729),
                   (u32 *)(param_1 + 0xc2a1));
    *(u32 *)(param_1 + 0x4a5f) = 1;
    return 1;
}
