#include "cmre.h"

extern void cm_fn_0093fa90(int *param_1, int *param_2);
extern void CM_CDECL cm_fn_00941760(u8 *object, u32 *output);
extern void cm_fn_00943f30(int *param_1, int *param_2);
extern void cm_fn_00944300(int param_1);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

/* Thiscall with no stack arguments (ret 0). ECX points at a pointer to the object O.
   The routine frees each element of the array at O+0x1b (count at O+0x1f, element
   field +0x14 freed first), then releases the blocks at O+0x1b, O+0x14, O+0x10, O+0x0c,
   O+0x08 and O+0x04, and finally clears the object's first 35 bytes. The callees at
   0x0093fa90, 0x00943f30 and 0x00941760 receive a pointer to the saved-ECX stack slot as
   their output; that slot is a local here. */
void CM_THISCALL cm_fn_005b1940(u32 *param_1)
{
    u32 saved_ecx;
    u32 o;
    u32 counter;
    s32 k;
    s32 index;
    u32 element;
    u32 y;
    u32 a;
    u32 p;
    u32 q;
    u32 r;
    u32 t;
    u32 u;
    u32 i;

    saved_ecx = (u32)param_1;
    o = *param_1;
    counter = 0;
    k = *(s32 *)(o + 0x1f);
    if (k > 0) {
        do {
            index = (s32)(s16)counter;
            element = *(u32 *)(*(u32 *)(o + 0x1b) + (u32)index * 4U);
            y = *(u32 *)(element + 0x14);
            if (y != 0) {
                cm_fn_00944c94((void *)y);
            }
            cm_fn_00944c94((void *)element);
            k = *(s32 *)(o + 0x1f);
            counter++;
        } while ((s32)(s16)counter < k);
    }

    a = *(u32 *)(o + 0x1b);
    if (a != 0) {
        cm_fn_009452ca(a);
    }

    p = *(u32 *)(o + 0x14);
    if (p != 0) {
        cm_fn_0093fa90((int *)p, (int *)&saved_ecx);
    }

    q = *(u32 *)(o + 0x10);
    if (q != 0) {
        cm_fn_00943f30((int *)q, (int *)&saved_ecx);
    }

    r = *(u32 *)(o + 0xc);
    if (r != 0) {
        cm_fn_00941760((u8 *)r, &saved_ecx);
    }

    t = *(u32 *)(o + 8);
    if (t != 0) {
        cm_fn_00944300((int)t);
    }

    u = *(u32 *)(o + 4);
    if (u != 0) {
        cm_fn_00944c94((void *)u);
    }

    for (i = 0; i < 8; i++) {
        *(u32 *)(o + 4 * i) = 0;
    }
    *(u16 *)(o + 32) = 0;
    *(u8 *)(o + 34) = 0;
}
