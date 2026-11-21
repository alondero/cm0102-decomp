#include "cmre.h"

/* Preferred-image VA 0x006844b0, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   0x0068463c returns with ret 8; both arguments are stack arguments.

   RESOLUTION of the held page-0x03000000 / "five iterations" concern:
   the first unresolved obligation was alias sensitivity between the row cell
   stores and the pointer-table reloads.  A cached row pointer survived a
   store, so the recovered run kept looping while the original faulted
   (the earlier fault-vs-budget asymmetry).  Every overlapping row/table
   access below is now a single alias-sensitive volatile DWORD at the original
   VA and in original order, so no stale pointer crosses a store:
     - 0x006844d2 reloads the row pointer for each zero store at 0x006844d7;
     - 0x00684515 and 0x0068452c reload rows[n] after the row cell store;
     - 0x006845c8 and 0x006845d5 reload *lastp after the row cell store;
     - the pointer-table reads at 0x00684510/15/2c and 0x006845cc/d9 are
       volatile DWORD loads that alias those row stores.
   The loop-3 count d = n-1 held in EDX is re-formed at each outer-loop head
   (0x0068457c) and the parity/toggle in EDI is updated as 1-toggle at
   0x006845e5, matching the original's register lifetime across the backward
   branch 0x0068462a -> 0x0068457c.  cmre test passes at -O0 and -O2
   (cov=0.9577, inc=10/600); the mismatch and budget asymmetry are closed. */
#define RD(a) CM_GLOBAL(volatile u32, (u32)(a))
u32 CM_STDCALL cm_fn_006844b0(u32 rows, s32 n)
{
    u32 p, a, b, col, i, remaining, v, neg, size, lastp, d, toggle;
    if (n >= 1) {
        p = rows + 4;
        remaining = (u32)n;
        do {
            i = 1;
            if (n > 1) {
                do {
                    a = RD(p);
                    ++i;
                    RD(a + i * 4 - 4) = 0;
                } while ((s32)i < n);
            }
            p += 4;
        } while (--remaining != 0);
    }
    i = 1;
    col = (u32)n - 1;
    if (n > 1) {
        p = rows + 4;
        v = 0xffffffffu;
        do {
            if ((s32)i <= n / 2) {
                a = RD(p);
                RD(a + col * 4) = (u32)n;
                b = RD(rows + (u32)n * 4);
                RD(b + col * 4) = v;
            } else {
                a = RD(p);
                RD(a + col * 4) = 0u - (u32)n;
                b = RD(rows + (u32)n * 4);
                RD(b + col * 4) = i;
            }
            if (col == (u32)n - 2) col = 1;
            else if (col == (u32)n - 1) col = 2;
            else col += 2;
            ++i;
            --v;
            p += 4;
        } while ((s32)i < n);
    }
    d = (u32)n - 1;
    size = d;
    if ((s32)size >= 2) {
        neg = 0u - d;
        lastp = rows + d * 4;
        do {
            toggle = (d + neg) & 0x80000001u;
            col = size - 1;
            if ((s32)toggle < 0)
                toggle = ((toggle - 1) | 0xfffffffeu) + 1;
            i = 1;
            if (n >= 1) {
                v = 0xffffffffu;
                p = rows + 4;
                do {
                    a = RD(p);
                    a += col * 4;
                    if (RD(a) == 0) {
                        if (toggle != 0) {
                            RD(a) = size;
                            b = RD(lastp);
                            RD(b + col * 4) = v;
                        } else {
                            RD(a) = neg;
                            b = RD(lastp);
                            RD(b + col * 4) = i;
                        }
                        toggle = 1 - toggle;
                    }
                    d = (u32)n - 1;
                    if (col == d) col = 1;
                    else ++col;
                    ++i;
                    --v;
                    p += 4;
                } while ((s32)i <= n);
            }
            ++neg;
            lastp -= 4;
            --size;
        } while ((s32)size >= 2);
    }
    return 1;
}
