/* Preferred-image VA 0x008cc930, image base 0x00400000, CM0102 3.9.68.
   Thiscall (ECX = self) with two stack arguments (`ret 8`): arg1 is a pointer
   to a key value, arg2 a pointer to a compare value. Field offsets from self:
   0x00 is a pointer to an array of entry pointers, 0x04 is the entry count
   (signed), 0x86e is a pointer to a byte table indexed by key * 8 + 1.
   Entry offsets: 0x00 a pointer whose dword at +2 is copied to +0xa, 0x08 a
   compare dword, 0x0c the key, 0x2c and 0x2d are bytes. Callees 0x0068d1b0
   (fastcall, one argument in ECX) and 0x008e7a70 (thiscall, receiver = entry,
   four pushed arguments 0, 0, 0, 1; callee pops) stay original boundaries. */
#include "cmre.h"

extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);
extern int CM_THISCALL cm_fn_008e7a70(int param_1, int param_2, int param_3,
                                      int param_4, int param_5);

u32 CM_THISCALL cm_fn_008cc930(u32 self, u32 key_ptr, u32 cmp_ptr)
{
    volatile u8 *base8 = (volatile u8 *)self;
    u32 result;
    s32 best;
    u32 index;
    u32 tbl;
    u32 key;
    u32 count;
    u32 arr;
    u32 entry;
    u32 ptr;
    u32 idx;
    u32 cand;
    u32 val;
    s32 score;

    result = 0;
    index = 0;
    best = -1;

    tbl = *(volatile u32 *)(base8 + 0x86e);
    key = *(volatile u32 *)key_ptr;
    if (*(volatile u8 *)(tbl + key * 8u + 1u) == 0)
        return 0;

    count = *(volatile u32 *)(base8 + 4);
    if ((s32)count <= 0)
        return 0;

    arr = *(volatile u32 *)base8;
    do {
        entry = *(volatile u32 *)(arr + index * 4u);
        if (entry != 0 && *(volatile u32 *)(entry + 0xc) == key)
            goto found;
        index++;
    } while ((s32)index < (s32)count);
    return 0;

found:
    ptr = *(volatile u32 *)entry;
    if (ptr == 0)
        goto no_ptr;

    val = *(volatile u32 *)(ptr + 2);
    *(volatile u32 *)(ptr + 0xa) = val;

    ptr = cm_fn_0068d1b0(*(volatile u32 *)entry);
    if (ptr == 0)
        return result;

    do {
        idx = *(volatile u32 *)ptr;
        arr = *(volatile u32 *)base8;
        cand = *(volatile u32 *)(arr + idx * 4u);

        val = *(volatile u32 *)(cand + 8);
        if (val == *(volatile u32 *)cmp_ptr
            || *(volatile u8 *)(cand + 0x2d) == 1) {
            if (*(volatile u8 *)(cand + 0x2c) == 2) {
                score = cm_fn_008e7a70((int)cand, 0, 0, 0, 1);
                if (score > best) {
                    result = cand;
                    best = score;
                }
            }
        }

        ptr = cm_fn_0068d1b0(*(volatile u32 *)entry);
    } while (ptr != 0);
    return result;

no_ptr:
    val = *(volatile u32 *)(entry + 8);
    if (val == *(volatile u32 *)cmp_ptr
        || *(volatile u8 *)(entry + 0x2d) == 1) {
        if (*(volatile u8 *)(entry + 0x2c) == 2)
            return entry;
    }
    return 0;
}
