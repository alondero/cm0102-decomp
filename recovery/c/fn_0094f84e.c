/* Preferred-image VA 0x0094F84E, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (two stack arguments: [ebp+8] = word pointer, [ebp+0xc] = value,
   plain ret 0). V = value is divided by 32 with IDIV, giving quotient q and signed
   remainder r. Stack slot [ebp+0xc] is reused as an accumulator starting at 0.
   Three words starting at the pointer are rewritten: each is replaced by
   (word >> (r & 31)) | accumulator, and the accumulator becomes
   (word & ((1 << (r & 31)) - 1)) << ((32 - r) & 31).
   Then, for index b = 2, 1, 0 (descending), word[b] = word[b - q] when b >= q
   (signed), otherwise word[b] = 0. ESI is only read by OR with -1, so its entry
   value does not affect the result. The return value is not compared; field
   meanings remain unknown. */
#include "cmre.h"

void CM_CDECL cm_fn_0094f84e(u32 *words, s32 value)
{
    s32 quot;
    s32 rem;
    u32 sel;
    u32 mask;
    u32 merge;
    u32 low;
    s32 i;
    s32 b;
    s32 c;

    quot = value / 32;
    rem = value % 32;
    sel = (u32)rem & 31u;
    mask = ~(0xffffffffu << sel);
    merge = 0;
    for (i = 0; i < 3; i++) {
        low = words[i] & mask;
        words[i] = (words[i] >> sel) | merge;
        merge = low << (((u32)(32 - rem)) & 31u);
    }
    b = 2;
    for (c = 8; c >= 0; c -= 4) {
        if (b < quot) {
            words[b] = 0;
        } else {
            words[b] = words[b - quot];
        }
        b--;
    }
}
