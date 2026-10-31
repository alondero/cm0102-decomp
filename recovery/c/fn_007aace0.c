#include "cmre.h"

/* Entry: STDCALL with one stack argument (ret 4); the argument is the object
   pointer. ECX is written before it is read, so there is no receiver.
   Object fields used: 0x00 and 0x04 and 0x08 (variadic source words), 0x24
   and 0x39 (pointers into a 0x32-entry table at +0xd7), 0x28 and 0x3d (bytes
   that index dispatch tables at 0x007aae60, 0x007aae90 and 0x007aae44, 0x007aae74). */

extern u32 CM_THISCALL cm_fn_00896c60(int self, int param_2, int *param_3, int param_4, int param_5);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

void CM_STDCALL cm_fn_007aace0(u32 param_1)
{
    u32 obj = param_1;
    u32 ptr;
    u32 scan_al;
    u32 v0;
    u32 v4;
    u32 v8;
    s32 s;
    s32 s2;
    u32 cl;
    u32 cl2;
    u8 flag1;
    u8 flag2;
    s32 idx;

    /* First pointer at object+0x39: two calls, then clear the table slot. */
    ptr = *(volatile u32 *)(obj + 0x39);
    if (ptr != 0) {
        cm_fn_00896c60((int)CM_GLOBAL(u32, 0x00de1d08), (int)ptr, (int *)obj, 0, 1);
        ptr = *(volatile u32 *)(obj + 0x39);
        cm_fn_00896c60((int)CM_GLOBAL(u32, 0x00de1d08), (int)ptr, (int *)obj, 1, 1);
        ptr = *(volatile u32 *)(obj + 0x39);
        scan_al = 0;
        do {
            idx = (s32)(s8)scan_al;
            if (*(volatile u32 *)(ptr + (u32)idx * 4u + 0xd7u) == obj) {
                *(volatile u32 *)(ptr + (u32)idx * 4u + 0xd7u) = 0;
                break;
            }
            scan_al = (u8)(scan_al + 1u);
        } while (scan_al < 0x32u);
    }

    /* Second pointer at object+0x24: same sequence. */
    ptr = *(volatile u32 *)(obj + 0x24);
    if (ptr != 0) {
        cm_fn_00896c60((int)CM_GLOBAL(u32, 0x00de1d08), (int)ptr, (int *)obj, 0, 1);
        ptr = *(volatile u32 *)(obj + 0x24);
        cm_fn_00896c60((int)CM_GLOBAL(u32, 0x00de1d08), (int)ptr, (int *)obj, 1, 1);
        ptr = *(volatile u32 *)(obj + 0x24);
        scan_al = 0;
        do {
            idx = (s32)(s8)scan_al;
            if (*(volatile u32 *)(ptr + (u32)idx * 4u + 0xd7u) == obj) {
                *(volatile u32 *)(ptr + (u32)idx * 4u + 0xd7u) = 0;
                break;
            }
            scan_al = (u8)(scan_al + 1u);
        } while (scan_al < 0x32u);
    }

    /* First dispatch on the signed byte at object+0x3d, through the byte map at 0x007aae60. */
    s = (s32)*(volatile s8 *)(obj + 0x3d);
    if ((u32)s > 0x10u) {
        *(volatile u8 *)(obj + 0x3d) = 0x10;
        flag1 = 0;
    } else {
        cl = *(volatile u8 *)(0x007aae60u + (u32)s);
        (void)*(volatile u32 *)(0x007aae44u + cl * 4u);
        switch (cl) {
        case 0:
            flag1 = 0;
            break;
        case 1:
            return;
        case 2:
            *(volatile u8 *)(obj + 0x3d) = 5;
            flag1 = 1;
            break;
        case 3:
            *(volatile u8 *)(obj + 0x3d) = 6;
            flag1 = 1;
            break;
        case 4:
            *(volatile u8 *)(obj + 0x3d) = 8;
            flag1 = 1;
            break;
        case 5:
            v0 = *(volatile u32 *)(obj + 0x00);
            v8 = *(volatile u32 *)(obj + 0x08);
            v4 = *(volatile u32 *)(obj + 0x04);
            cm_fn_00944c9f(0x00de1f64u, 0x00a70150u, v4, v8, v0);
            flag1 = 0;
            break;
        default: /* 6 */
            *(volatile u8 *)(obj + 0x3d) = 0x10;
            flag1 = 0;
            break;
        }
    }

    /* Second dispatch on the signed byte at object+0x28, through the byte map at 0x007aae90. */
    s2 = (s32)*(volatile s8 *)(obj + 0x28);
    if ((u32)s2 > 0x10u) {
        *(volatile u8 *)(obj + 0x28) = 0x10;
        flag2 = 0;
    } else {
        cl2 = *(volatile u8 *)(0x007aae90u + (u32)s2);
        (void)*(volatile u32 *)(0x007aae74u + cl2 * 4u);
        switch (cl2) {
        case 0:
            flag2 = 0;
            break;
        case 1:
            return;
        case 2:
            *(volatile u8 *)(obj + 0x3d) = 5;
            flag2 = 1;
            break;
        case 3:
            *(volatile u8 *)(obj + 0x3d) = 6;
            flag2 = 1;
            break;
        case 4:
            *(volatile u8 *)(obj + 0x3d) = 8;
            flag2 = 1;
            break;
        case 5:
            v0 = *(volatile u32 *)(obj + 0x00);
            v8 = *(volatile u32 *)(obj + 0x08);
            v4 = *(volatile u32 *)(obj + 0x04);
            cm_fn_00944c9f(0x00de1f64u, 0x00a70150u, v4, v8, v0);
            flag2 = 0;
            break;
        default: /* 6 */
            *(volatile u8 *)(obj + 0x28) = 0x10;
            flag2 = 0;
            break;
        }
    }

    if (flag1 == 0) {
        *(volatile u32 *)(obj + 0x39) = 0;
    }
    if (flag2 == 0) {
        *(volatile u32 *)(obj + 0x24) = 0;
    }
}
