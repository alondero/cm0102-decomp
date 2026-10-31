#include "cmre.h"

/* Three stack arguments (cdecl, ret 0): the string pointer at [esp+4] on entry,
   then two flags (tested for zero only). ECX is read by `or ecx, 0xffffffff`
   only as scratch for repne scasb; the result does not depend on it.
   The routine scans the string for its length twice (the second scan repeats
   the first), then searches outward from the midpoint: the backward index
   starts at the midpoint and must be above zero, the forward index starts at
   the midpoint and must be below the length. A space at the matched index is
   replaced with LF (byte 0x0a) and the routine returns 1. Otherwise it
   returns 0 and writes nothing. */
u32 CM_CDECL cm_fn_0090cb20(u8 *s, u32 flag2, u32 flag3)
{
    u32 len;
    u32 t;
    s32 lenS;
    s32 fwd;
    s32 back;

    if (s == 0) {
        return 0;
    }

    /* First repne scasb scan: length in ECX after `not`/`dec` is the byte count. */
    len = 0;
    while (s[len] != 0) {
        len++;
    }
    if (len == 0) {
        return 0;
    }

    /* Second scan repeats the first. */
    len = 0;
    while (s[len] != 0) {
        len++;
    }

    /* cdq; sub eax, edx; sar eax, 1 */
    lenS = (s32)len;
    t = len - (u32)(lenS >> 31);
    fwd = (s32)t >> 1;
    back = fwd;

    while (back > 0 || (s32)fwd < (s32)len) {
        if (flag2 != 0 && back > 0 && s[back] == 0x20) {
            s[back] = 0x0a;
            return 1;
        }
        if (flag3 != 0 && fwd < (s32)len && s[fwd] == 0x20) {
            s[fwd] = 0x0a;
            return 1;
        }
        back = back - 1;
        fwd = fwd + 1;
    }

    return 0;
}
