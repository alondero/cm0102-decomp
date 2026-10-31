#include "cmre.h"

/* Callees are thiscall: ECX is the receiver, the listing pushes the stack arguments and
   the callees pop them (ret 8, ret 8, ret 4). */
extern u8 CM_THISCALL cm_fn_00456740(u8 *self, u32 a, u32 b);
extern u32 CM_THISCALL cm_fn_004a7f20(u8 *self, u32 a, u32 b);
extern void CM_THISCALL cm_fn_004ce4e0(u8 *self, u32 arg);

/* Thiscall with two stack arguments (ret 8): arg1 points at a pair of dwords (R0, R1) and
   arg2 supplies a byte. The receiver's fields are used by offset: +0x0c is a base B of
   14-byte records, +0x14 a signed count N, +0x18 a signed index I stored back each step.
   The record at B + 14*I is tested by the callees at 0x00456740 and 0x004a7f20; a nonzero
   result from the first ends the routine. Otherwise the record is retired: its dword at +0
   is set to -1 and the old value X selects the block at P + 110*X, where P is the global
   0x00ae23bc. If the byte at +0xd is 3, the block is passed to 0x004ce4e0. */
void CM_THISCALL cm_fn_004cd9c0(u8 *param_1, u32 *param_2, u32 param_3)
{
    s32 index;
    s32 count;
    u32 base;
    u32 element;
    u32 r0;
    u32 r1;
    u32 found;
    u32 old;
    u32 block;

    index = *(s32 *)(param_1 + 0x18);
    count = *(s32 *)(param_1 + 0x14);
    if (index >= count) {
        return;
    }
    do {
        index = *(s32 *)(param_1 + 0x18);
        base = *(u32 *)(param_1 + 0xc);
        element = base + (u32)index * 14U;
        r1 = param_2[1];
        r0 = param_2[0];
        if (cm_fn_00456740((u8 *)(element + 4), r0, r1) != 0) {
            return;
        }
        r1 = param_2[1];
        r0 = param_2[0];
        found = cm_fn_004a7f20((u8 *)(element + 4), r0, r1);
        if (found != 0) {
            if ((s8)*(u8 *)(element + 0xc) > (s8)(u8)param_3) {
                return;
            }
        }
        old = *(u32 *)element;
        if (old != 0xffffffffU) {
            block = CM_GLOBAL(u32, 0x00ae23bc) + old * 110U;
            *(u32 *)element = 0xffffffffU;
            if (*(u8 *)(element + 0xd) == 3) {
                cm_fn_004ce4e0(param_1, block);
            }
        }
        index = *(s32 *)(param_1 + 0x18);
        count = *(s32 *)(param_1 + 0x14);
        index++;
        *(s32 *)(param_1 + 0x18) = index;
    } while (index < count);
}
