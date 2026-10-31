#include "cmre.h"

extern void CM_CDECL cm_fn_00945333(u32 *param_1, u32 *param_2, u32 param_3, void *param_4);
extern u32 CM_CDECL cm_fn_00946464(u32 param_1, u32 param_2, u32 param_3, int param_4, void *param_5);

/* Thiscall (ECX = self), one stack argument (arg_04 = [entry+4]), `ret 4`, no
   return value. self+0 is a pointer B to 42735-byte records (index i at B +
   42735*i), self+4 is a signed count N. Each record has a pointer S at +0xa6e3
   (the array base for 30-byte entries) and a count C at +0xa6eb.
   For each i < N: a null record is skipped; S == 0 ends the routine (no
   further calls). Otherwise 0x946464 is called with (&local, S, C, 0x1e,
   0x601a10) where local = [arg_04], and its result R (non-zero) drives a
   compaction of the 30-byte entries: q = (R - S) * 0x88888889 >> 36, and when
   q < C-1 the entries from q+1 onward are shifted down one slot (7 dwords plus
   a word per entry, forward, as rep movsd/movsw). Then the last byte of the
   final entry is cleared, its first dword set to -1, and C is decremented.
   After the loop, 0x945333(S, C, 0x1e, 0x601a10) is called on the last record
   processed (or on arg_04 when N <= 0). Records are reached through the same
   pointer B; the final call uses the record of the last index. */
void CM_THISCALL cm_fn_005fff30(void *self, u32 arg_04)
{
    volatile u8 *base = (volatile u8 *)self;
    u32 local;
    u32 rec;
    u32 last;
    s8 i;   /* byte counter at [esp+0x13]: wraps 127 -> -128 like the original */
    u32 n;
    u32 s;
    u32 c;
    u32 r;
    u32 d_idx;
    u32 off;
    u32 dst;
    u32 src;
    u32 cc;
    u32 s2;
    u32 q;
    u32 ecx_d;
    u32 edx_v;
    u32 k;

    if (arg_04 == 0)
        return;

    n = *(volatile u32 *)(base + 4);
    last = arg_04;
    i = 0;
    if ((s32)n > 0) {
        do {
            rec = *(volatile u32 *)base + 42735u * (u32)(s32)i;
            last = rec;
            if (rec != 0) {
                s = *(volatile u32 *)(rec + 0xa6e3);
                if (s == 0)
                    return;
                local = *(volatile u32 *)arg_04;
                c = *(volatile u32 *)(rec + 0xa6eb);
                s = *(volatile u32 *)(rec + 0xa6e3);
                r = cm_fn_00946464((u32)&local, s, c, 0x1e, (void *)0x00601a10);
                if (r != 0) {
                    s = *(volatile u32 *)(rec + 0xa6e3);
                    ecx_d = r - s;
                    q = (u32)(((unsigned long long)0x88888889ull * (unsigned long long)ecx_d) >> 36);
                    c = *(volatile u32 *)(rec + 0xa6eb);
                    if ((s32)q < (s32)(c - 1u)) {
                        d_idx = q;
                        off = ((q << 4) - q) << 1;
                        do {
                            s = *(volatile u32 *)(rec + 0xa6e3);
                            d_idx++;
                            dst = s + off;
                            src = dst + 30u;
                            for (k = 0; k < 7; k++)
                                *(volatile u32 *)(dst + 4u * k) = *(volatile u32 *)(src + 4u * k);
                            *(volatile u16 *)(dst + 28u) = *(volatile u16 *)(src + 28u);
                            off += 30u;
                            c = *(volatile u32 *)(rec + 0xa6eb) - 1u;
                        } while ((s32)d_idx < (s32)c);
                    }

                    cc = *(volatile u32 *)(rec + 0xa6eb);
                    edx_v = (cc << 4) - cc;
                    s2 = *(volatile u32 *)(rec + 0xa6e3);
                    *(volatile u8 *)(s2 + edx_v * 2u - 0x1au) = 0;

                    cc = *(volatile u32 *)(rec + 0xa6eb);
                    edx_v = *(volatile u32 *)(rec + 0xa6e3);
                    ecx_d = (cc << 4) - cc;
                    *(volatile u32 *)(edx_v + ecx_d * 2u - 0x1eu) = 0xffffffffu;
                    cc = *(volatile u32 *)(rec + 0xa6eb);
                    *(volatile u32 *)(rec + 0xa6eb) = cc - 1u;
                }
            }
            i = (s8)(i + 1);
        } while ((s32)i < *(volatile s32 *)(base + 4));
    }

    c = *(volatile u32 *)(last + 0xa6eb);
    s = *(volatile u32 *)(last + 0xa6e3);
    cm_fn_00945333((u32 *)s, (u32 *)c, 0x1e, (void *)0x00601a10);
}
