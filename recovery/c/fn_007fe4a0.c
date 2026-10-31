#include "cmre.h"

/* fn_007fe4a0: thiscall (ECX = object, read before written) with six stack arguments (ret 0x18).
   Preferred-image VA 0x007fe4a0, image base 0x00400000.
   Object: a signed 16-bit index at 0x382a (saved, zeroed, scanned 0..15 and restored) and a
   signed 16-bit count table at 0x380a (2 bytes per index). Records are 0x30 bytes; per index
   the block is 768 bytes (16 rows of 48), with a dword key at +0x7d6 in each row's block base.
   Each step may call 0x007fe600 with (arg2..arg6), and copy rows with 0x0090ba90.
   The index is re-read from memory at each point where the listing re-reads it. */

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6);
extern void CM_CDECL cm_fn_0090ba90(void *arg1, void *arg2, u32 count);

u32 CM_THISCALL cm_fn_007fe4a0(u8 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6)
{
    u16 saved;
    s32 idx;
    s32 idx3;
    s32 idx5;
    s32 j;
    u32 key;

    saved = *(volatile u16 *)(self + 0x382au);
    *(volatile u16 *)(self + 0x382au) = 0u;

    for (;;) {
        idx = (s16)*(volatile s16 *)(self + 0x382au);
        if (*(volatile s16 *)(self + 0x380au + (u32)idx * 2u) != 0) {
            if (arg1 == 0u) {
                cm_fn_007fe600((u32)self, arg2, arg3, arg4, arg5, arg6);
            } else {
                key = *(volatile u32 *)(self + (u32)idx * 0x300u + 0x7d6u);
                if (arg1 == key) {
                    cm_fn_007fe600((u32)self, arg2, arg3, arg4, arg5, arg6);
                }
            }

            idx = (s16)*(volatile s16 *)(self + 0x382au);
            j = 1;
            if (*(volatile s16 *)(self + 0x380au + (u32)idx * 2u) > 1) {
                do {
                    cm_fn_0090ba90(self + (u32)idx * 0x300u + 0x7d4u,
                                   self + (((u32)idx << 4) + (u32)(s32)j) * 0x30u + 0x7d4u,
                                   0x30u);
                    if (arg1 == 0u) {
                        cm_fn_007fe600((u32)self, arg2, arg3, arg4, arg5, arg6);
                    } else {
                        idx3 = (s16)*(volatile s16 *)(self + 0x382au);
                        key = *(volatile u32 *)(self + (u32)idx3 * 0x300u + 0x7d6u);
                        if (arg1 == key) {
                            cm_fn_007fe600((u32)self, arg2, arg3, arg4, arg5, arg6);
                        }
                    }
                    idx = (s16)*(volatile s16 *)(self + 0x382au);
                    j = (s16)(j + 1);
                } while (j < *(volatile s16 *)(self + 0x380au + (u32)idx * 2u));
            }

            idx5 = (s16)*(volatile s16 *)(self + 0x382au);
            if (*(volatile s16 *)(self + 0x380au + (u32)idx5 * 2u) > 1) {
                cm_fn_0090ba90(self + (u32)idx5 * 0x300u + 0x7d4u,
                               self + (u32)idx5 * 0x300u + 0x804u,
                               0x30u);
            }
        }

        *(volatile s16 *)(self + 0x382au) = (s16)(*(volatile s16 *)(self + 0x382au) + 1);
        if (!(*(volatile s16 *)(self + 0x382au) < 0x10)) {
            break;
        }
    }

    *(volatile u16 *)(self + 0x382au) = saved;
    return 1u;
}
