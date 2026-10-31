/* Preferred-image VA 0x00468b90, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX = this) with one stack word; RET 4 pops it. Only the
   low word is read (signed). If that word equals -1 the routine returns 0 and
   reads no memory. Otherwise the signed word at this+0x382a selects a row, which
   gives the table pointer stored at this + row*0x300 + 0x7fc (wrapping 32-bit
   arithmetic). The result is the u32 at table + arg*0x18c + 0xba9a6, returned in
   EAX. Callers read no EDX, so the return width is 32 bits. Field and table
   meanings remain unknown. */
#include "cmre.h"

u32 CM_THISCALL cm_fn_00468b90(u8 *self, short arg1)
{
    u32 row;
    u32 index;
    u32 table;

    if (arg1 == -1) {
        return 0;
    }
    row = (u32)(s32)*(s16 *)(self + 0x382a);
    index = (u32)(s32)arg1;
    table = *(u32 *)(self + (((row * 3u) << 8) + 0x7fc));
    return *(u32 *)((u8 *)table + (index * 0x18cu) + 0xba9a6u);
}
