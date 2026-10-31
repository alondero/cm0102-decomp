/* Preferred-image VA 0x0066A280, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object) with three stack arguments, ret 0xc;
   returns the object in EAX. Stores dword +0x05 = arg1, dword +0x01 = arg2,
   byte +0x00 = 0 and dword +0x0a = arg3.
   The table at 0x00B63D60 has 0x23 entries of 0x48 bytes whose first dword is
   a record pointer. An entry qualifies when that pointer is nonzero, the
   record's byte +0x11c is nonzero and either arg1 is 0 or that byte has bit 1
   (mask 2) set. First pass: the byte total of qualifying entries is stored at
   byte +0x09. Second pass: for each of the 0x23 bytes of the array at arg2, the
   entry selected by that signed byte is tested the same way; each qualifying
   one increments a byte tally, and when the signed tally equals
   ((s8)total + 1) / 2 (signed, truncating) byte +0x00 is set to the record's
   byte +0x38 when arg3 is nonzero, otherwise its byte +0x04.
   The original reuses arg2's stack slot as scratch for the total (stack only).
   Object accesses are volatile so -O2 keeps the listing's store order.
   Field meanings remain unknown. */
#include "cmre.h"

#define CM_FN_0066A280_ENTRY(i) CM_GLOBAL(u8 *, 0x00b63d60 + (u32)(i) * 0x48u)

u8 *CM_THISCALL cm_fn_0066a280(u8 *object, u32 arg1, s8 *arg2, u32 arg3)
{
    volatile u8 *self = (volatile u8 *)object;
    u8 total = 0;
    u8 tally = 0;
    u8 *record;
    u8 flags;
    u32 i;
    s32 index;

    *(volatile u32 *)(self + 0x05) = arg1;
    *(s8 *volatile *)(self + 0x01) = arg2;
    self[0x00] = 0;
    *(volatile u32 *)(self + 0x0a) = arg3;
    for (i = 0; i < 0x23; i++) {
        record = CM_FN_0066A280_ENTRY(i);
        if (record != 0) {
            flags = record[0x11c];
            if (flags != 0 && (arg1 == 0 || (flags & 2) != 0)) {
                total++;
            }
        }
    }
    self[0x09] = total;
    for (i = 0; i < 0x23; i++) {
        index = (s32)arg2[i];
        if (CM_FN_0066A280_ENTRY(index) == 0) {
            continue;
        }
        record = CM_FN_0066A280_ENTRY(index);
        flags = record[0x11c];
        if (flags == 0) {
            continue;
        }
        if (arg1 != 0 && (flags & 2) == 0) {
            continue;
        }
        tally++;
        if ((s32)(s8)tally == ((s32)(s8)total + 1) / 2) {
            if (arg3 != 0) {
                self[0x00] = record[0x38];
            } else {
                self[0x00] = record[0x04];
            }
        }
    }
    return object;
}
