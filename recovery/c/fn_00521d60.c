/* Preferred-image VA 0x00521D60, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object; read before written), two stack arguments
   (record pointer at [esp+4] on entry, arg2 at [esp+8]), ret 8. No EAX value is set on
   any path, so the return is void.
   Table walk: index = signed word at record+0x34. mask = word at
   dword(object+0xa3) + index*0x68 + 0x0f, OR-ed into record+0x38; then the
   record+0x38 word is re-read. If bit 0x800 is clear, return with no store. Else
   entry = dword(dword(object+0xad) + index*4) + arg2*0x35. Two signed bytes are read
   from dword(object+0xa7) + 6*s + 4, where s is the signed word at entry and entry+2
   (the first is at +0, the second at +2). Compare as signed bytes:
   first < second: store (flags ^ 0x800) | 0x1000; first > second: store flags | 0x800;
   equal: store (flags ^ 0x800) | 1. Accesses are volatile so the listing's read and
   store order is kept. Field meanings remain unknown. */
#include "cmre.h"

void CM_THISCALL cm_fn_00521d60(void *object, void *record, u32 arg2)
{
    volatile u8 *obj = (volatile u8 *)object;
    volatile u8 *rec = (volatile u8 *)record;
    u8 *table;
    u8 *entry;
    u8 *a7;
    s32 index;
    u16 mask;
    u16 flags;
    s16 s0;
    s16 s1;
    s8 cl;
    s8 al;

    index = *(volatile s16 *)(rec + 0x34);
    table = *(u8 * volatile *)(obj + 0xa3);
    mask = *(volatile u16 *)(table + index * 0x68 + 0x0f);
    *(volatile u16 *)(rec + 0x38) = (u16)(*(volatile u16 *)(rec + 0x38) | mask);
    flags = *(volatile u16 *)(rec + 0x38);
    if ((flags & 0x800) == 0) {
        return;
    }
    table = *(u8 * volatile *)(obj + 0xad);
    entry = *(u8 * volatile *)(table + index * 4);
    entry = entry + arg2 * 0x35;
    a7 = *(u8 * volatile *)(obj + 0xa7);
    s0 = *(volatile s16 *)entry;
    s1 = *(volatile s16 *)(entry + 2);
    cl = *(volatile s8 *)(a7 + (s32)s0 * 6 + 4);
    al = *(volatile s8 *)(a7 + (s32)s1 * 6 + 4);
    if (cl < al) {
        flags = (u16)((flags ^ 0x800) | 0x1000);
    } else if (cl > al) {
        flags = (u16)(flags | 0x800);
    } else {
        flags = (u16)((flags ^ 0x800) | 0x0001);
    }
    *(volatile u16 *)(rec + 0x38) = flags;
}
