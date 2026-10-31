#include "cmre.h"

extern void CM_CDECL cm_fn_00945333(u32 *param_1, u32 *param_2, u32 param_3, void *param_4);

/* Cdecl, no arguments, `ret 0`, no return value. The frame keeps a 60-entry
   array of record pointers (rec, at entry-0x104+0x14) and a loop counter.
   Four keys are computed from globals A (0x9cf614), B (0x9cf7c0), C (0x9cf61c)
   and D (0x9cf618), each as 107*x + E with E = [0xae23d0]: keyA = 107A + E
   (edi), keyB = 107B + E (eax), keyC = 107C + E (esi), keyD = 107D + E (ebp).
   For every record pointer T (table base [0xae23b4], stride 0x245, N records
   from [0xae235c]) whose dword at +0x57 equals one of the four keys, T is
   appended to rec while the count stays below 60. The loop stops after N
   records or when the count is 60 (as a signed byte).
   0x945333 is then called with (rec, count, 4, 0x53fd40). Finally each
   recorded T gets the key stored at +0x60: keyC for indices 0..19, keyD for
   20..39 and keyA for 40 and up. */
void CM_CDECL cm_fn_007d0210(void)
{
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    u32 e;
    u32 key_a;
    u32 key_b;
    u32 key_c;
    u32 key_d;
    u32 n;
    u32 t;
    u32 v;
    u32 k;
    u32 rec[60];
    u8 bl;
    s32 count;
    s32 i;
    u32 rec_i;

    a = *(volatile u32 *)0x009cf614;
    b = *(volatile u32 *)0x009cf7c0;
    k = 0;
    bl = 0;
    key_a = ((a * 3u) * 9u * 4u) - a;
    c = *(volatile u32 *)0x009cf61c;
    key_c = ((c * 3u) * 9u * 4u) - c;
    d = *(volatile u32 *)0x009cf618;
    key_b = ((b * 3u) * 9u * 4u) - b;
    key_d = ((d * 3u) * 9u * 4u) - d;
    e = *(volatile u32 *)0x00ae23d0;
    key_b += e;
    key_c += e;
    key_d += e;
    key_a += e;

    n = *(volatile u32 *)0x00ae235c;
    t = *(volatile u32 *)0x00ae23b4;
    if ((s32)n > 0) {
        for (;;) {
            if ((s8)bl >= 0x3c)
                break;
            v = *(volatile u32 *)(t + 0x57);
            if (v == key_c || v == key_a || v == key_d || v == key_b) {
                rec[bl] = t;
                bl++;
            }
            k++;
            t += 0x245u;
            n = *(volatile u32 *)0x00ae235c;
            if (!((s32)(s16)k < (s32)n))
                break;
        }
    }

    count = (s32)(s8)bl;
    cm_fn_00945333(rec, (u32 *)(u32)count, 4u, (void *)0x0053fd40);

    if ((s16)(s8)bl > 0) {
        i = 0;
        do {
            rec_i = rec[i];
            if ((s16)i < 0x14)
                *(volatile u32 *)(rec_i + 0x60) = key_c;
            else if ((s16)i < 0x28)
                *(volatile u32 *)(rec_i + 0x60) = key_d;
            else
                *(volatile u32 *)(rec_i + 0x60) = key_a;
            i++;
        } while ((s16)i < (s16)(s8)bl);
    }
}
