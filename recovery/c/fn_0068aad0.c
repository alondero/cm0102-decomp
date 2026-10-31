/* Preferred-image VA 0x0068AAD0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object, copied to ESI), two stack arguments
   (first at entry [esp+4] is an integer compared with dwords at record+0x1c
   and +0x20; second at entry [esp+8] is the record pointer), ret 8.
   Byte pairs are compared as signed chars: record+0x43/+0x44, then (if the object
   word +0xab has bit 0x02) +0x47/+0x48, then (if bit 0x01 or 0x20) +0x49/+0x4a.
   A differing pair returns object+0xc2 when the 1c/20 match rules select it,
   otherwise 0. A pair that is equal falls through to the next test. When no
   pair decides, the result is object+0xc3. Return width is one byte (AL).
   Single direct caller 0x0043c4aa; field meanings remain unknown. */
#include "cmre.h"

/* Mirrors 0x68aaee..0x68aaf9 and 0x68ab48..0x68ab4f. Equality with the dword at
   record+0x1c, or with record+0x20, chooses the result depending on which of the
   signed pair is larger or smaller. Returns 1 when the result is selected. */
static int cm_fn_0068aad0_select(u8 *record, s32 arg_a, s8 lo, s8 hi)
{
    s32 field_1c = *(s32 *)(record + 0x1c);

    if (arg_a == field_1c && lo > hi) {
        return 1;
    }
    if (arg_a == *(s32 *)(record + 0x20) && lo < hi) {
        return 1;
    }
    return 0;
}

u8 CM_THISCALL cm_fn_0068aad0(void *self, s32 arg_a, void *record_ptr)
{
    u8 *base = (u8 *)self;
    u8 *record = (u8 *)record_ptr;
    s8 dl;
    s8 bl;
    u16 flags;

    dl = (s8)record[0x43];
    bl = (s8)record[0x44];
    if (dl != bl) {
        if (cm_fn_0068aad0_select(record, arg_a, dl, bl)) {
            return base[0xc2];
        }
        return 0;
    }

    flags = *(u16 *)(base + 0xab);
    if ((flags & 2) != 0) {
        dl = (s8)record[0x47];
        bl = (s8)record[0x48];
        if (dl != bl) {
            if (cm_fn_0068aad0_select(record, arg_a, dl, bl)) {
                return base[0xc2];
            }
            return 0;
        }
    }

    if ((flags & 0x21) != 0) {
        dl = (s8)record[0x49];
        bl = (s8)record[0x4a];
        if (dl != bl) {
            if (cm_fn_0068aad0_select(record, arg_a, dl, bl)) {
                return base[0xc3];
            }
            return 0;
        }
    }

    return base[0xc3];
}
