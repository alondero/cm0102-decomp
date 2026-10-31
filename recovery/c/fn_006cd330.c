#include "cmre.h"

extern u8 CM_THISCALL cm_fn_006f5c30(u32 self, u32 param_2, u32 param_3, u32 param_4, u32 param_5);

/* Thiscall (ECX = self) with seven stack arguments (`ret 0x1c`); the listing
   reads arg_04 (signed byte index A), arg_08 (base B), arg_0c, arg_10, arg_14,
   arg_18 and arg_1c (byte compared with 2 at [esp+0x34] under the 0x18-byte
   frame). Each other stack slot is read as its full dword, as the listing does.
   Element records are 44 bytes apart from B + 44*A; the record pointer is the
   dword at each slot. The count is the signed byte at B + A + 0x58. A record
   is skipped when its signed byte at +0x2b is above 1 or its byte at +0x19 is
   negative. When arg_1c == 2 a local byte flag is incremented; otherwise the
   record is passed to 0x6f5c30 (thiscall on the record, four pushed dwords in
   the order arg_10, arg_14, arg_0c, arg_18). A non-zero result clears this
   bytes +0x1a6 and +0x1a5 and returns 1; otherwise the flag byte is returned.
   The callee's result is read as its low byte only. */
u8 CM_THISCALL cm_fn_006cd330(void *self, s32 arg_04, u32 arg_08, u32 arg_0c,
                              u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c)
{
    volatile u8 *base = (volatile u8 *)self;
    s32 a;
    u32 slot;
    u32 elem;
    s8 count;
    s32 i;
    u8 flag;
    u8 r;

    a = (s32)(s8)arg_04;
    flag = 0;
    count = (s8)*(volatile u8 *)(arg_08 + (u32)a + 0x58);
    if (count <= 0)
        return flag;

    slot = arg_08 + (u32)a * 44u;
    i = 0;
    do {
        elem = *(volatile u32 *)slot;
        if ((s8)*(volatile u8 *)(elem + 0x2b) > 1) {
            /* skip */
        } else if ((s8)*(volatile u8 *)(elem + 0x19) < 0) {
            /* skip */
        } else if ((u8)arg_1c == 2) {
            flag++;
        } else {
            r = cm_fn_006f5c30(elem, arg_10, arg_14, arg_0c, arg_18);
            if (r != 0) {
                *(volatile u8 *)((u32)base + 0x1a6) = 0;
                *(volatile u8 *)((u32)base + 0x1a5) = 0;
                return 1;
            }
        }
        count = (s8)*(volatile u8 *)(arg_08 + (u32)a + 0x58);
        i++;
        slot += 4;
    } while (i < (s32)count);

    return flag;
}
