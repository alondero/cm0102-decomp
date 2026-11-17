#include "cmre.h"
/* Original instruction evidence: preferred-image VAs in CM0102 3.9.68,
 * SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Entry ECX saved at 447441; RET 8 at 447873.
 * Receiver setups: 44754c/447556, 4476c7/4477fc, 447840, 447500.
 * 44759e/4475a0 and 4476d4..4476d9 supply selector, club, flag
 * with the saved receiver in ECX. 447565..44757a pushes only club.
 */
extern u32 CM_THISCALL cm_fn_004528d0(u32, u32 *, u32);
extern u32 CM_THISCALL cm_fn_00452710(u32, u32 *, u32);
extern u32 CM_THISCALL cm_fn_00449590(u32, s8, u32, u8);
extern u32 CM_THISCALL cm_fn_00452cb0(u32, u32, u8);
extern u32 CM_THISCALL cm_fn_0048d200(u32, s32);
extern u32 CM_THISCALL cm_fn_006827d0(u32, u32);
extern u32 CM_CDECL cm_fn_00669950(u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile s16, (a))
#define H(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

u32 CM_THISCALL cm_fn_00447430(u32 self, u32 root, u32 enabled)
{
    u8 scratch[0x220];
    u32 current, id, offset, count, entry, club, rec, rec2, stat, value;
    u32 table, old, next, p, n, k;
    u8 score;
    s32 idx;
    if (!root || B(root + 0x44) != 255) {
        void *a = scratch + 0x120;
        void *b = scratch + 0x20;
        u32 line = root ? 0x4f1 : 0x4e9;
        if (root) {
            a = scratch + 0x20;
            b = scratch + 0x120;
        }
        cm_fn_00944cff(0x98b6a0, 0, 0, a, b);
        value = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, value, a, b, line);
        cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
        D(0xb67a34) = 0;
        return 0;
    }
    p = D(root + 4);
    id = D(p);
    current = root;
    for (;;) {
        cm_fn_006827d0(current, 0);
        count = 0;
        offset = 0;
        if (W(current + 0x3e) > 0) do {
            entry = D(current + 0xb1) + offset;
            if (B(entry + 0x11)) {
                club = D(entry);
                if (club) {
                    rec = cm_fn_004528d0(self, (u32 *)club, 1);
                    rec2 = cm_fn_00452710(self, (u32 *)club, 1);
                    value = D(club);
                    stat = D(self + 0x1c) + value * 0x6a;
                    value = cm_fn_00669950(club);
                    p = D(current + 0xb1);
                    score = B(p + offset + 4);
                    if (cm_fn_00449590(self, 1, club, 1)) {
                        old = D(rec + 1);
                        if ((s32)old < 0) goto low_store;
                        table = D(0xae23d0);
                        if (W(table + id * 0x6b + 0x69) >
                            W(table + old * 0x6b + 0x69)) goto low_store;
                        if (id == old && score < B(rec)) {
low_store:
                            B(rec) = score;
                            D(rec + 1) = id;
                            H(rec + 5) = 0;
                            table = D(0xae23d0);
                        }
                        if (enabled) {
                            old = D(rec2 + 1);
                            if ((s32)old < 0 || (s32)old > (s32)D(0xae2378))
                                goto low_second;
                            if (W(table + id * 0x6b + 0x69) >
                                W(table + old * 0x6b + 0x69)) goto low_second;
                            if (id == old && score < B(rec2)) {
low_second:
                                B(rec2) = score;
                                D(rec2 + 1) = id;
                                H(rec2 + 5) = (u16)value;
                                if (cm_fn_00600a50(club, 0, 1, 0, 0) ||
                                    cm_fn_0053b900((u32 *)club)) {
                                    if (W(stat + 0x68) >= 5) {
                                        n = D(self + 0x2c);
                                        if ((s32)n > 0) {
                                            k = D(club);
                                            p = D(self + 0x24);
                                            for (idx = 0; idx < (s32)n; ++idx, p += 6)
                                                if (D(p) == k && B(p + 4) == 1)
                                                    goto high;
                                        }
                                        cm_fn_00452cb0(self, D(club), 1);
                                    }
                                }
                            }
                        }
                    }
high:
                    if (cm_fn_00449590(self, 2, club, 1)) {
                        old = D(rec + 8);
                        if ((s32)old < 0) goto high_store;
                        table = D(0xae23d0);
                        if (W(table + id * 0x6b + 0x69) <
                            W(table + old * 0x6b + 0x69)) goto high_store;
                        if (id == old && score > B(rec + 7)) {
high_store:
                            B(rec + 7) = score;
                            D(rec + 8) = id;
                            H(rec + 12) = 0;
                            table = D(0xae23d0);
                        }
                        if (enabled) {
                            old = D(rec2 + 8);
                            if ((s32)old < 0 || (s32)old > (s32)D(0xae2378))
                                goto high_second;
                            if (W(table + id * 0x6b + 0x69) <
                                W(table + old * 0x6b + 0x69)) goto high_second;
                            if (id == old && score > B(rec2 + 7)) {
high_second:
                                B(rec2 + 7) = score;
                                D(rec2 + 8) = id;
                                H(rec2 + 12) = (u16)value;
                                if (cm_fn_00600a50(club, 0, 1, 0, 0) ||
                                    cm_fn_0053b900((u32 *)club)) {
                                    if (W(stat + 0x68) >= 5) {
                                        n = D(self + 0x2c);
                                        if ((s32)n > 0) {
                                            k = D(club);
                                            p = D(self + 0x24);
                                            for (idx = 0; idx < (s32)n; ++idx, p += 6)
                                                if (D(p) == k && B(p + 4) == 2)
                                                    goto advance;
                                        }
                                        cm_fn_00452cb0(self, D(club), 2);
                                    }
                                }
                            }
                        }
                    }
                }
            }
advance:
            ++count;
            offset += 0x3b;
        } while ((s32)count < W(current + 0x3e));
        idx = (s8)B(current + 0x44);
        if (idx >= (s32)D(root + 0x30)) return 1;
        next = cm_fn_0048d200(root, idx + 1);
        if (B(next + 0x43) != 2) return 1;
        {
            s16 num = W(next + 0x3e);
            if (num != W(current + 0x3e)) return 1;
        }
        current = next;
    }
}
