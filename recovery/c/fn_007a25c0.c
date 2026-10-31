/* Preferred-image VA 0x007A25C0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: stdcall, one pointer argument, ret 4, byte result in AL.
   A null argument or a null dword at +0x61 returns 0. Otherwise the signed
   byte at argument+0x18 is the value; the record at +0x61 selects a threshold:
   0x21 if its byte +0x0f is 0x14, else 0x1f if byte +0x11 or +0x12 is 0x14,
   else 0x1d if byte +0x13 or +0x14 is 0x14, else 0x1c. The x87 test
   (threshold * double[0x009689C0]) < value together with value > 0x20 returns 4;
   otherwise value < 0x12 returns 1, value >= 0x18 returns 8, else 2.
   The original reuses its argument slot as fild scratch (stack only).
   Field meanings remain unknown. */
#include "cmre.h"

u8 CM_STDCALL cm_fn_007a25c0(u8 *arg)
{
    u8 *rec;
    s8 value;
    s8 threshold;

    if (arg == 0) {
        return 0;
    }
    rec = *(u8 **)(arg + 0x61);
    if (rec == 0) {
        return 0;
    }
    value = (s8)arg[0x18];
    if (rec[0x0f] == 0x14) {
        threshold = 0x21;
    } else if (rec[0x11] == 0x14 || rec[0x12] == 0x14) {
        threshold = 0x1f;
    } else if (rec[0x13] == 0x14 || rec[0x14] == 0x14) {
        threshold = 0x1d;
    } else {
        threshold = 0x1c;
    }
    if ((double)(s32)threshold * CM_GLOBAL(double, 0x009689c0) < (double)(s32)value
        && value > 0x20) {
        return 4;
    }
    if (value < 0x12) {
        return 1;
    }
    return value >= 0x18 ? 8 : 2;
}
