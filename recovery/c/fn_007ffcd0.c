#include "cmre.h"

/* fn_007ffcd0: thiscall (ECX = object, read before written) with one stack argument (ret 4).
   Preferred-image VA 0x007ffcd0, image base 0x00400000.
   Object fields: 0x380a signed 16-bit counts indexed by the signed 16-bit index at 0x382a
   (2 bytes per entry); records are 0x30 bytes, 16 per index step, starting at 0x7d4, with
   the compared u32 at +0x7d6 and checks at +0x7ee and +0x7f2. */
extern void CM_CDECL cm_fn_0090ba90(void *arg1, void *arg2, u32 count);

void CM_THISCALL cm_fn_007ffcd0(u8 *self, u32 param_2)
{
    s16 idx;
    s16 count;
    s16 j;
    u32 pofs;
    u8 *p;

    j = 0;
    idx = *(volatile s16 *)(self + 0x382a);
    count = *(volatile s16 *)(self + 0x380a + (u32)(s32)idx * 2u);
    if (count > 0) {
        do {
            pofs = (((u32)(s32)idx << 4) + (u32)(s32)j) * 0x30u;
            if (*(volatile u32 *)(self + pofs + 0x7d6u) == param_2) {
                break;
            }
            j = (s16)(j + 1);
        } while (j < count);
    }

    if (j >= count) {
        return;
    }

    pofs = (((u32)(s32)idx << 4) + (u32)(s32)j) * 0x30u;
    p = self + pofs;
    if (*(volatile u32 *)(p + 0x7ee) != 0u) {
        return;
    }
    if (*(volatile u32 *)(p + 0x7f2) != 0u) {
        return;
    }

    if (j != 0) {
        cm_fn_0090ba90(p + 0x7d4, self + (u32)(s32)idx * 0x300u + 0x7d4u, 0x30u);
    }

    idx = *(volatile s16 *)(self + 0x382a);
    *(volatile u32 *)(self + (u32)(s32)idx * 0x300u + 0x7eau) = 1u;
}
