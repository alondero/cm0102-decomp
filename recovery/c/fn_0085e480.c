#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern u32 CM_THISCALL cm_fn_0054c770(u16 *param_1, u32 *param_2, u8 param_3);
extern void CM_CDECL cm_fn_00549c90(int *param_1);
extern u8 CM_THISCALL cm_fn_007c6830(int param_1, int *param_2, int param_3, char param_4, int param_5);

/* Thiscall with three stack arguments (ret 0xc). The receiver (ECX) is an object. arg1
   points to a dword (copied into the object at +0, and used as an index), arg2 points to a
   dword (copied to +4, and a pointer for the callee), arg3 is a dword passed along.
   Fields are used by offset as in the listing. The two 16-byte-aligned scratch buffers
   used for the callees' output are one array: the first is at +0, the second at +8
   (overlapping by design, as the listing's esp offsets show). */
void CM_THISCALL cm_fn_0085e480(u8 *param_1, u32 *arg1, u32 *arg2, u32 arg3)
{
    u8 scratch[0x10];
    u32 v0;
    u32 q;
    u32 r;
    u32 ptr;
    u8 dl;
    u8 c;
    u8 al;
    u32 eax_val;
    u32 cval;
    u8 old;

    v0 = *arg1;
    *(u32 *)(param_1 + 0) = v0;
    *(u32 *)(param_1 + 4) = *arg2;
    *(u32 *)(param_1 + 8) = 0;
    *(u32 *)(param_1 + 0xc) = 0xc8U;

    cm_fn_00417c10((u32 *)(param_1 + 0x25), (u32 *)(scratch + 0), (u16 *)0x00ae2c90);
    r = cm_fn_0054c770((u16 *)0x00ae2c90, (u32 *)(scratch + 0), (u8)arg3);
    cm_fn_00417c10((u32 *)(param_1 + 0x2d), (u32 *)(scratch + 8), (u16 *)r);

    al = *(u8 *)(param_1 + 0x35);
    *(u32 *)(param_1 + 0x36) = 0xc8U;
    al = (u8)(al & 0xc5U);
    *(u8 *)(param_1 + 0x3a) = 0;
    al = (u8)(al | 5U);
    *(u8 *)(param_1 + 0x44) = 0;
    *(u8 *)(param_1 + 0x35) = al;

    v0 = *(u32 *)(param_1 + 0);
    *(u8 *)(param_1 + 0x4e) = 0;
    *(u32 *)(param_1 + 0x45) = 0;
    ptr = CM_GLOBAL(volatile u32, 0x00ae23bc) + (v0 * 55U) * 2U;
    cm_fn_00549c90((int *)ptr);

    *(u32 *)(param_1 + 0x40) = 0;
    *(u8 *)(param_1 + 0x4e) = 4;
    *(u8 *)(param_1 + 0x3f) = 0;
    *(u8 *)(param_1 + 0x3e) = 0;
    if (arg2 != 0) {
        q = *(u32 *)((u8 *)arg2 + 0x53);
    } else {
        q = 0;
    }
    if (arg2 != 0 && q != 0) {
        dl = *(u8 *)(q + 0x77);
        cval = CM_GLOBAL(volatile u32, 0x00b74350);
        al = cm_fn_007c6830((int)cval, (int *)arg1, (int)arg2, (char)dl, 0);
        if ((s8)al > 10) {
            c = 10;
        } else {
            c = al;
        }
        old = *(u8 *)(param_1 + 0x4f);
        *(u8 *)(param_1 + 0x4f) = (u8)((old & 0xf0U) | (c & 0x0fU));
    }

    eax_val = 0xffffffffU;
    *(u8 *)(param_1 + 0x49) = 0;
    *(u32 *)(param_1 + 0x4a) = eax_val;
    *(u16 *)(param_1 + 0x3b) = 0;
    *(u8 *)(param_1 + 0x1c) = 0;
    *(u8 *)(param_1 + 0x1d) = 0;
    *(u8 *)(param_1 + 0x1e) = 0;
    *(u8 *)(param_1 + 0x1f) = 0;
    *(u8 *)(param_1 + 0x20) = 0;
    *(u32 *)(param_1 + 0x10) = eax_val;
    *(u32 *)(param_1 + 0x14) = eax_val;
    *(u32 *)(param_1 + 0x18) = eax_val;
    *(u32 *)(param_1 + 0x21) = 0;
}
