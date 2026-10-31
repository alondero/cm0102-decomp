/* Preferred-image VA 0x00687970, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object) with one stack argument, ret 4.
   A zero argument selects the buffer at dword +0xb1 and marks it as the
   object's own. Nothing happens if the signed word +0x3e is below 1 or the
   buffer is null. Otherwise:
   - if dword buffer+0x05 is nonzero, for i = 0 while i < (s16)word +0x3e
     (re-read every pass), the dword at buffer+0x05 + i*0x3b is passed to
     009452CA when nonzero;
   - if dword buffer+0x09 is nonzero and (s16)word +0x3e > 0, the same loop runs
     over buffer+0x09 + i*0x3b;
   - the buffer itself is passed to 009452CA;
   - for the object's own buffer, word +0x3e and dword +0xb1 are cleared.
   Direct callers do not observe EAX or EDX. Field meanings remain unknown;
   009452CA is an explicit boundary. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);

void CM_THISCALL cm_fn_00687970(u8 *self, u8 *buffer)
{
    u32 owned = 0;
    s16 entries;
    u8 *slot;
    s32 i;

    if (buffer == 0) {
        buffer = *(u8 **)(self + 0xb1);
        owned = 1;
    }
    entries = *(s16 *)(self + 0x3e);
    if (entries < 1 || buffer == 0) {
        return;
    }
    if (*(u32 *)(buffer + 0x05) != 0 && entries > 0) {
        slot = buffer + 0x05;
        i = 0;
        do {
            if (*(u32 *)slot != 0) {
                cm_fn_009452ca(*(u32 *)slot);
            }
            i++;
            slot += 0x3b;
        } while (i < (s32)*(s16 *)(self + 0x3e));
    }
    if (*(u32 *)(buffer + 0x09) != 0 && *(s16 *)(self + 0x3e) > 0) {
        slot = buffer + 0x09;
        i = 0;
        do {
            if (*(u32 *)slot != 0) {
                cm_fn_009452ca(*(u32 *)slot);
            }
            i++;
            slot += 0x3b;
        } while (i < (s32)*(s16 *)(self + 0x3e));
    }
    cm_fn_009452ca((u32)buffer);
    if (owned != 0) {
        *(u16 *)(self + 0x3e) = 0;
        *(u32 *)(self + 0xb1) = 0;
    }
}
