#include "cmre.h"

extern void CM_CDECL cm_fn_00945333(u32 *param_1, u32 *param_2, u32 param_3, void *param_4);

/* Cdecl, no stack arguments, `ret 0`, no return value. Four keys are built from
   globals as 107*G + E (E = [0x00ae23d0]): D8 from 0x009cf8e8 (edi), D4 from
   0x009cf8e4 (esi), D12 from 0x009cf8ec (ebp), D16 from 0x009cf8f0 (ebx), and
   V from 0x009cf8f4 (kept in a frame slot). A byte counter c (capped at 50 by
   the loop head) collects the record pointers T + 0x245*j (j < N = [0x00ae235c],
   T = [0x00ae23b4]) whose pointer at +0x57 is non-null and whose dword there
   equals [0x009cf8e0]. The collected pointer array (50 slots) is passed with
   count c, 4 and 0x53fe50 to 0x945333. Then for each collected record i the
   dword at +0x60 is set to D4 for i < 10, D8 for i < 20, D12 for i < 30, D16
   for i < 40 and V otherwise. */
void CM_CDECL cm_fn_008fe580(void)
{
    u32 ptrs[50];
    u32 g8;
    u32 g4;
    u32 g12;
    u32 g16;
    u32 g20;
    u32 e;
    u32 d4;
    u32 d8;
    u32 d12;
    u32 d16;
    u32 v;
    u32 n;
    u32 t;
    u32 y;
    u32 k;
    s32 j;
    s32 c;
    s32 i;
    u32 r;
    u32 value;

    g8 = *(volatile u32 *)0x009cf8e8;
    d8 = 107u * g8;
    g4 = *(volatile u32 *)0x009cf8e4;
    d4 = 107u * g4;
    g12 = *(volatile u32 *)0x009cf8ec;
    d12 = 107u * g12;
    g16 = *(volatile u32 *)0x009cf8f0;
    d16 = 107u * g16;
    e = *(volatile u32 *)0x00ae23d0;
    d8 += e;
    d4 += e;
    d12 += e;
    d16 += e;
    g20 = *(volatile u32 *)0x009cf8f4;
    v = 107u * g20 + e;
    n = *(volatile u32 *)0x00ae235c;
    t = *(volatile u32 *)0x00ae23b4;
    c = 0;
    j = 0;

    if ((s32)n > 0) {
        for (;;) {
            if (c >= 0x32)
                break;
            y = *(volatile u32 *)(t + 0x57);
            if (y != 0) {
                k = *(volatile u32 *)y;
                if (k == *(volatile u32 *)0x009cf8e0) {
                    ptrs[c] = t;
                    c++;
                }
            }
            j++;
            t += 0x245u;
            if (!((s32)(s16)j < *(volatile s32 *)0x00ae235c))
                break;
        }
    }

    cm_fn_00945333(ptrs, (u32 *)(u32)(s32)(s8)c, 4u, (void *)0x0053fe50);

    if ((s16)(s8)c > 0) {
        for (i = 0; i < (s16)(s8)c; i++) {
            r = ptrs[i];
            if (i < 10)
                value = d4;
            else if (i < 20)
                value = d8;
            else if (i < 30)
                value = d12;
            else if (i < 40)
                value = d16;
            else
                value = v;
            *(volatile u32 *)(r + 0x60) = value;
        }
    }
}
