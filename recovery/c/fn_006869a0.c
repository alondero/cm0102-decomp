/* Preferred-image VA 0x006869A0, image base 0x00400000; primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object) with five stack arguments, ret 0x14.
   Arguments 3 and 4 are signed indexes; a negative value returns argument 3
   unchanged in EAX. Otherwise a 59-byte row at table + arg3 * 59 holds a pointer
   at +5. That pointer's block is indexed by arg4 * 14 and receives byte and word
   updates. Arguments 5 and 6 are signed bytes compared against each other. The
   object's bytes +0xC2 and +0xC3 are signed deltas added to a word. Returns the
   address of the word at block + arg4 * 14 + 0xA. Field and value meanings remain
   unknown. */
#include "cmre.h"

u32 CM_THISCALL cm_fn_006869a0(u8 *object, u8 *table, s32 arg3, s32 arg4,
                               s8 arg5, s8 arg6)
{
    u8 *block;
    u32 offset;

    if (arg3 < 0 || arg4 < 0) {
        return (u32)arg3;
    }
    block = *(u8 **)(table + (u32)arg3 * 59u + 5u);
    offset = (u32)arg4 * 14u;
    *(u8 *)(block + offset + 4u) += 1;
    if (arg5 > arg6) {
        *(u8 *)(block + offset + 5u) += 1;
        *(s16 *)(block + offset + 0xcu) =
            (s16)(*(s16 *)(block + offset + 0xcu) + (s16)(s8)object[0xc2]);
    } else if (arg5 == arg6) {
        *(u8 *)(block + offset + 6u) += 1;
        *(s16 *)(block + offset + 0xcu) =
            (s16)(*(s16 *)(block + offset + 0xcu) + (s16)(s8)object[0xc3]);
    } else {
        *(u8 *)(block + offset + 7u) += 1;
    }
    *(s16 *)(block + offset + 8u) =
        (s16)(*(s16 *)(block + offset + 8u) + (s16)arg5);
    *(s16 *)(block + offset + 0xau) =
        (s16)(*(s16 *)(block + offset + 0xau) + (s16)arg6);
    return (u32)(block + offset + 0xau);
}
