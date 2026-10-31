#include "cmre.h"

/* fn_006ce310: thiscall (ECX = object, read before written) with seven stack arguments (ret 0x1c).
   Preferred-image VA 0x006ce310, image base 0x00400000.
   Arguments (stack order): param_2 char flag (selects which of param_3/param_4 is the first
   output), param_3..param_7 byte output pointers (cleared on entry), param_8 char flag.
   Object fields: 0xf5bd and 0xf5f3 bytes copied to the outputs; 0x4792 pointer to a record with
   a word at +0x30 (passed to 0x004b0850), signed bytes at +0x4b and +0x4c, and a word flag at
   +0x38; 0x8ed4 signed word threshold.
   The return is the full 32-bit EAX of the listing: the byte-wide moves keep the upper bits of
   the previous EAX, and the final subtraction uses full registers. The variable e below tracks
   EAX through every write. */

extern u32 cm_fn_004b0850(u32 param_1);

u32 CM_THISCALL cm_fn_006ce310(u8 *self, s8 param_2, u8 *param_3, u8 *param_4, u8 *param_5,
                               u8 *param_6, u8 *param_7, s8 param_8)
{
    s8 local_1;
    s8 cVar3;
    u8 *pcVar4;
    u8 *pcVar5;
    u8 *rec;
    u32 w;
    u32 r4b;
    s8 c4b;
    u16 flags;
    u8 old5;
    u32 e;

    e = (u32)param_3;

    *(volatile u8 *)param_3 = 0u;
    *(volatile u8 *)param_4 = 0u;
    cVar3 = 0;
    *(volatile u8 *)param_5 = 0u;
    *(volatile u8 *)param_6 = 0u;
    local_1 = 0;
    *(volatile u8 *)param_7 = 0u;

    if (param_2 == 0) {
        pcVar4 = param_3;
        pcVar5 = param_4;
        if (param_8 != 0) {
            local_1 = 1;
        }
    } else {
        pcVar4 = param_4;
        pcVar5 = param_3;
        if (param_8 != 0) {
            cVar3 = 1;
        }
    }
    e = (e & 0xffffff00u) | (u8)param_8;

    *(volatile u8 *)pcVar4 = *(volatile u8 *)(self + 0xf5bd);
    e = (e & 0xffffff00u) | *(volatile u8 *)(self + 0xf5f3);
    *(volatile u8 *)pcVar5 = (u8)e;

    rec = (u8 *)*(volatile u32 *)(self + 0x4792);
    w = (u32)(s32)*(volatile s16 *)(rec + 0x30);
    r4b = cm_fn_004b0850(w);
    e = r4b;
    if ((u8)r4b == 3) {
        e = (u32)param_7;
        *(volatile u8 *)param_7 = 1u;
    }

    rec = (u8 *)*(volatile u32 *)(self + 0x4792);
    c4b = *(volatile s8 *)(rec + 0x4b);
    e = (e & 0xffffff00u) | (u8)c4b;
    if (c4b != -1) {
        *(volatile u8 *)pcVar4 = (u8)(*(volatile u8 *)pcVar4 + (u8)c4b);
        rec = (u8 *)*(volatile u32 *)(self + 0x4792);
        old5 = *(volatile u8 *)pcVar5;
        e = (e & 0xffffff00u) | *(volatile u8 *)(rec + 0x4c);
        *(volatile u8 *)pcVar5 = (u8)(old5 + (u8)e);
        e = (u32)(s32)cVar3;
        *(volatile u8 *)param_6 = 1u;

        if ((s32)(s8)*(volatile u8 *)pcVar4 + (s32)local_1 ==
            (s32)(s8)*(volatile u8 *)pcVar5 + (s32)cVar3) {
            rec = (u8 *)*(volatile u32 *)(self + 0x4792);
            flags = *(volatile u16 *)(rec + 0x38);
            if ((flags & 0x0004u) == 0u) {
                if ((flags & 0x4000u) == 0u ||
                    (s32)*(volatile s16 *)(self + 0x8ed4) > 0x3de) {
                    if (param_2 == 0) {
                        e = (u32)*(volatile u8 *)(self + 0xf5f3) + (u32)(s32)local_1;
                        if ((s32)e < (s32)*(volatile s8 *)(rec + 0x4b)) {
                            *(volatile u8 *)param_5 = 1u;
                        }
                    } else {
                        if ((s32)*(volatile s8 *)(rec + 0x4b) <
                            (s32)((u32)*(volatile u8 *)(self + 0xf5f3) + (u32)(s32)cVar3)) {
                            e = (u32)param_5;
                            *(volatile u8 *)param_5 = 1u;
                        }
                    }
                }
            }
        }
    }

    e = (e & 0xffffff00u) | *(volatile u8 *)param_5;
    if ((u8)e != 0u) {
        e = (e & 0xffffff00u) | 1u;
        return e;
    }
    e = (e & 0xffffff00u) | *(volatile u8 *)param_3;
    e = e - (((u32)param_3 & 0xffffff00u) | *(volatile u8 *)param_4);
    return e;
}
