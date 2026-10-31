#include "cmre.h"

/* Cdecl wildcard match. Stack arguments: arg1 (listing [esp+0x18] after the
   pushes) is s2, the pattern that may contain '*' and '.'; arg2 (ebp) is s1,
   the name. Index k walks s2 and index j walks s1. Each byte is read in listing
   order. The original's arg2 slot is reused as a byte temporary for the
   character-fold result; that slot is not observable here. */

extern u32 CM_CDECL cm_fn_0090c2f0(u32 value);
extern u32 *CM_CDECL cm_fn_00946540(const u8 *text, u32 ch);

/* Two-byte-step string compare used by the original (sbb-based result):
   returns 0 when both strings end together, otherwise -1 or 1. */
static s32 cm_wild_compare(const u8 *a, const u8 *b)
{
    for (;;) {
        u8 x = a[0];
        u8 y = b[0];
        if (x != y) {
            return (x < y) ? -1 : 1;
        }
        if (x == 0) {
            return 0;
        }
        x = a[1];
        y = b[1];
        if (x != y) {
            return (x < y) ? -1 : 1;
        }
        a = a + 2;
        b = b + 2;
        if (x == 0) {
            return 0;
        }
    }
}

u32 CM_CDECL cm_fn_00933a00(const u8 *s2, const u8 *s1)
{
    u32 k = 0;
    u32 j = 0;
    u8 bl;
    u32 r1;
    u32 r2;

    if (cm_wild_compare(s1, (const u8 *)0x00ad6ad8u) == 0) {
        return 0;
    }

    if (cm_wild_compare(s1, s2) == 0) {
        return 1;
    }
    if (s2[0] == 0) {
        return 1;
    }

    for (;;) {
        bl = s2[k];
        if (bl == 0x2au) {
            if (s2[k + 1] != 0x2eu) {
                return (s2[k + 1] == 0) ? 1u : 0u;
            }
            for (;;) {
                if (s1[j] == 0x2eu) {
                    if (cm_fn_00946540(s1 + j + 1, 0x2eu) == 0) {
                        k = k + 1;
                        break;
                    }
                }
                if (s1[j] == 0) {
                    return 0;
                }
                j = j + 1;
            }
        } else {
            r1 = cm_fn_0090c2f0(s1[j]);
            r2 = cm_fn_0090c2f0(bl);
            if ((u8)r2 != (u8)r1) {
                return 0;
            }
        }

        k = k + 1;
        j = j + 1;
        if (s2[k] == 0) {
            return 1;
        }
    }
}
