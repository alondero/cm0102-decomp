/* fn_008c7470: VA 0x008c7470, image base 0x00400000, 236 bytes. Convention: thiscall
   (receiver in ECX, saved to ebx), one stack argument (rec, saved to esi), `ret 4`.
   Returns 1 or 0 in EAX. Single direct caller 0x008c31a4.

   Record layout used by this routine, from the listing (offsets from rec):
     +0x04  pointer P (null-checked, copied into P+0x0a)
     +0x08  count-like field f8, scaled by 0x245 against global 0x00ae23b4
     +0x0c  count-like field f12, scaled by 0x6e against global 0x00ae23bc
   Every memory access is volatile and kept in listing order, including the
   re-reads of rec+4, rec+0x0c and both globals on the loop path. */
#include "cmre.h"

extern int CM_THISCALL cm_fn_004cf5a0(int *param_1, int *param_2);
extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);
extern u32 CM_THISCALL cm_fn_008c7560(int param_1, int *param_2, int param_3, int param_4);

#define G_AE23B4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define G_AE23BC CM_GLOBAL(volatile u32, 0x00ae23bc)
#define G_AE1050 CM_GLOBAL(volatile u32, 0x00ae1050)
#define G_AE1080 CM_GLOBAL(volatile u32, 0x00ae1080)

CM_THISCALL u32 cm_fn_008c7470(u32 self, u32 rec)
{
    u32 a;
    u32 b;
    u32 f8;
    u32 f12;
    u32 x;
    u32 y;
    u32 p;
    u32 p2;
    u32 t;
    u32 node;
    u32 r;
    u32 s;
    u32 value;
    u32 s2;
    u32 x3;
    u32 r4;

    a = G_AE23B4;                                  /* mov edx, [0xae23b4] */
    f8 = *(volatile u32 *)(rec + 0x08u);          /* mov eax, [esi+8] */
    y = a + f8 * 0x245u;                           /* lea: A + 581*f8 */
    f12 = *(volatile u32 *)(rec + 0x0cu);         /* mov eax, [esi+0xc] */
    b = G_AE23BC;                                  /* mov eax, [0xae23bc] */
    x = b + f12 * 0x6eu;                           /* lea: B + 110*f12 */
    if (cm_fn_008c7560((int)self, (int *)x, (int)y, 0) != 0) {
        return 1u;
    }

    p = *(volatile u32 *)(rec + 0x04u);            /* mov eax, [esi+4] */
    if (p == 0u) {
        return 0u;
    }
    t = *(volatile u32 *)(p + 0x02u);              /* mov edx, [eax+2] */
    *(volatile u32 *)(p + 0x0au) = t;              /* mov [eax+0xa], edx */

    p2 = *(volatile u32 *)(rec + 0x04u);           /* mov ecx, [esi+4] */
    node = cm_fn_0068d1b0((int)p2);
    if (node == 0u) {
        return 0u;
    }

    for (;;) {
        f12 = *(volatile u32 *)(rec + 0x0cu);     /* mov eax, [esi+0xc] */
        b = G_AE23BC;                              /* mov eax, [0xae23bc] */
        x = b + f12 * 0x6eu;                       /* lea: B + 110*f12 */
        r = (u32)cm_fn_004cf5a0((int *)G_AE1050, (int *)x);
        G_AE1080 = r;                              /* mov [0xae1080], eax (dword) */
        if (r == 0u) {
            value = 0u;
        } else {
            s = *(volatile u32 *)(r + 0x04u);     /* mov eax, [eax+4] */
            a = G_AE23B4;                          /* mov ecx, [0xae23b4] */
            value = a + s * 0x245u;                /* lea: A + 581*s */
        }

        s2 = *(volatile u32 *)node;                /* mov eax, [edi] */
        b = G_AE23BC;                              /* mov ecx, [0xae23bc] */
        x3 = b + s2 * 0x6eu;                       /* lea: B + 110*s2 */
        r4 = cm_fn_008c7560((int)self, (int *)x3, (int)value, 0);
        if (r4 != 0u) {
            return 1u;
        }

        p2 = *(volatile u32 *)(rec + 0x04u);       /* mov ecx, [esi+4] */
        node = cm_fn_0068d1b0((int)p2);
        if (node == 0u) {
            break;
        }
    }

    return 0u;
}
