#include "cmre.h"

/* Thiscall search over a 40-row table of 37-byte rows (ret 0x10, four stack
   arguments). Object field: 0xdc dword = table base B. Row index comes from the
   dword at arg_1. Table base for that index is the dword at B + 9*index. Row k
   lives at base2 + 37*k. A row matches when its dword at +0x0f equals the dword at
   arg_2, or (only when arg_4 is nonzero) its dword at +0x13 equals it. A matched
   row with a nonzero arg_3 is returned unless its byte at +0 is 0x0b; with a zero
   arg_3 it is returned only when that byte is 0x0b. Otherwise return 0. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

u32 CM_THISCALL cm_fn_00849ce0(u32 self, u32 arg_1, u32 arg_2, u32 arg_3, u32 arg_4)
{
    u32 base;
    u32 base2;
    u32 value;
    u32 row;
    u8 k;

    base = OBJ_U32(0xdc);
    base2 = *(volatile u32 *)(base + (*(volatile u32 *)arg_1) * 9u);
    value = *(volatile u32 *)arg_2;

    for (k = 0u; k < 0x28u; k = (u8)(k + 1u)) {
        row = base2 + 37u * (u32)k;
        if (*(volatile u32 *)(row + 0x0fu) != value) {
            if (arg_4 == 0u) {
                continue;
            }
            if (*(volatile u32 *)(row + 0x13u) != value) {
                continue;
            }
        }
        if (arg_3 != 0u) {
            if (*(volatile u8 *)row != 0x0bu) {
                return row;
            }
        } else {
            if (*(volatile u8 *)row == 0x0bu) {
                return row;
            }
        }
    }
    return 0u;
}
