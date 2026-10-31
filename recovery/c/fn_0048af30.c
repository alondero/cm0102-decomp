/* Preferred-image VA 0x0048AF30, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: one stack argument (the object), ret 4, ECX not read, so stdcall.
   Returns a byte in AL; only AL is declared (the caller reads no wider value).
   - Null object or null dword at +0x61: return 0.
   - Let code = 0x21 if byte [record+0x0f] == 0x14; else 0x1f if [record+0x11]
     or [record+0x12] == 0x14; else 0x1d if [record+0x13] or [record+0x14] == 0x14;
     else 0x1c. Let v18 = signed byte at object +0x18.
   - Return 4 if (double)code * 1.15 < (double)v18, or if v18 > 0x22 (signed).
   - Else 1 if v18 < 0x12, 2 if v18 < 0x18.
   - Else 8 if byte +0x22 is zero and signed word [record+0x0b] <= 0x1770,
     otherwise 0x10.
   Field meanings remain unknown; offsets are used as the names. */
#include "cmre.h"

u8 CM_STDCALL cm_fn_0048af30(u8 *self)
{
    u8 *record;
    s8 v18;
    s8 code;
    double scaled;

    if (self == 0) {
        return 0;
    }
    record = *(u8 **)(self + 0x61);
    if (record == 0) {
        return 0;
    }
    v18 = (s8)self[0x18];
    if (record[0x0f] == 0x14) {
        code = 0x21;
    } else if (record[0x11] == 0x14 || record[0x12] == 0x14) {
        code = 0x1f;
    } else if (record[0x13] == 0x14 || record[0x14] == 0x14) {
        code = 0x1d;
    } else {
        code = 0x1c;
    }
    scaled = (double)(s32)code * 1.15;
    if (scaled < (double)(s32)v18) {
        return 4;
    }
    if (v18 > 0x22) {
        return 4;
    }
    if (v18 < 0x12) {
        return 1;
    }
    if (v18 < 0x18) {
        return 2;
    }
    if (self[0x22] == 0 && *(s16 *)(record + 0x0b) <= 0x1770) {
        return 8;
    }
    return 0x10;
}
