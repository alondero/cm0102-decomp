/* Preferred-image VA 0x005E5D60, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl with three stack arguments: a 16-bit key ([esp+4], sign-extended
   after the 0xFFFF test), a u16 array pointer ([esp+0xc]) and a dword index
   ([esp+0x14]). Returns 0 when key is 0xFFFF, the pointer is NULL, or the
   element words[n] is 0. Otherwise the record at 0x00AE1DAC + 5124 * (signed key)
   is used. Word values 0x7C are mapped to 0x20 before use. The result is
   ((3 * dword[rec+0x284]) / 4) - dword[rec + 20*cur + 0x0C] - dword[rec + 20*prev
   + 0x10], where cur and prev are the mapped words[n] and words[n-1]. A negative
   result is returned as 0 (JNS test). Loads happen in listing order: words[n],
   words[n-1], then the three record dwords. Field meanings remain unknown. */
#include "cmre.h"

s32 CM_CDECL cm_fn_005e5d60(u16 key, u16 *words, s32 n)
{
    u16 cur_raw;
    u16 prev_raw;
    u32 cur;
    u32 prev;
    u8 *rec;
    s32 lhs;
    s32 term_a;
    s32 term_b;
    u32 res;

    if (key == 0xffff) {
        return 0;
    }
    rec = (u8 *)(0x00ae1dacu + 5124u * (u32)(s32)(s16)key);
    if (words == 0) {
        return 0;
    }
    cur_raw = words[n];
    if (cur_raw == 0) {
        return 0;
    }
    prev_raw = words[n - 1];
    cur = (cur_raw == 0x7c) ? 0x20u : (u32)cur_raw;
    prev = (prev_raw == 0x7c) ? 0x20u : (u32)prev_raw;
    lhs = *(s32 *)(rec + 0x284);
    lhs = (s32)(3u * (u32)lhs) / 4;
    term_a = *(s32 *)(rec + 20u * cur + 0x0cu);
    term_b = *(s32 *)(rec + 20u * prev + 0x10u);
    res = (u32)lhs - (u32)term_a;
    res = res - (u32)term_b;
    if (res & 0x80000000u) {
        return 0;
    }
    return (s32)res;
}
