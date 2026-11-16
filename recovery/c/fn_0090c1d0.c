/* Preferred-image VA 0x0090c1d0, base 0x00400000, CM0102 3.9.68.
 * Original SHA-256:
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 *
 * The byte stores at 0x0090c248/250 and 0x0090c259 change the caller's
 * first and fourth argument homes, not the cached string/limit values.
 * Volatile DWORD formals retain those homes and their aliased reloads.
 *
 * 0x0090c1d7/1e6 use the high byte of saved ECX for the fold flag.
 * Its pop into caller-clobbered ECX (e.g. 0x0090c2ae) is not a declared
 * return value; the flag is represented by a local, not register allocation.
 */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_0066f4d0(void);

s32 CM_CDECL cm_fn_0090c1d0(volatile u32 left_home,
                           volatile u32 right_home,
                           volatile s32 mode, volatile u32 limit_home)
{
    u32 idx = 0;
    u8 fold = 1;
    u8 lc, rc, lkey, rkey, next;
    u32 left, right, limit;

    if (cm_fn_0066f4d0() == 10)
        fold = 0;

    left = left_home;                 /* 0x0090c1eb */
    if (left == 0)
        return right_home != 0 ? -1 : 0;

    right = right_home;               /* 0x0090c201 */
    if (right == 0)
        return 1;

    if (CM_GLOBAL(volatile u8, left) != 0) {
        limit = limit_home;           /* 0x0090c21a: cached before home writes */
        for (;;) {
            rc = CM_GLOBAL(volatile u8, right + idx);
            if (rc == 0)
                break;

            if (fold != 0) {
                lc = CM_GLOBAL(volatile u8, left + idx);
                rkey = CM_GLOBAL(volatile u8, 0x00aa2154u + (u32)rc * 3u);
                lkey = CM_GLOBAL(volatile u8, 0x00aa2154u + (u32)lc * 3u);
            } else {
                lc = CM_GLOBAL(volatile u8, left + idx);
                lkey = lc;
                rkey = rc;
            }

            *(volatile u8 *)&left_home = lkey; /* 0x0090c248/250 */
            {
                /* CMP reads mode before the right-key home is overwritten. */
                s32 saved_mode = mode;        /* 0x0090c254 */
                *(volatile u8 *)&limit_home = rkey; /* 0x0090c259 */
                if (saved_mode == 0) {
                    u32 key = left_home;     /* 0x0090c25f */
                    lkey = CM_GLOBAL(volatile u8,
                                     0x00aa2156u + (key & 0xffu) * 3u);
                    key = limit_home;        /* 0x0090c26f */
                    rkey = CM_GLOBAL(volatile u8,
                                     0x00aa2156u + (key & 0xffu) * 3u);
                }
            }

            if (lkey != rkey)
                return lkey < rkey ? -1 : 1;

            if (limit != 0 && idx == limit - 1u)
                return 0;

            next = CM_GLOBAL(volatile u8, left + idx + 1u);
            ++idx;
            if (next != 0)
                continue;
            break;
        }
    }

    lc = CM_GLOBAL(volatile u8, left + idx);
    if (lc != 0)
        return 1;

    right = right_home;               /* 0x0090c29e: reload, not cached EDI */
    rc = CM_GLOBAL(volatile u8, right + idx);
    if (rc != 0)
        return -1;
    return 0;
}
