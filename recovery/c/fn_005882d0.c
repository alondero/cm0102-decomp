#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_007875e0(void *self, u32 arg);
extern u32 CM_CDECL cm_fn_005444a0(int param_1, int param_2);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

/* Cdecl with two stack arguments (arg_04 = record A, arg_08 = record B), `ret 0`,
   no return value. The SEH frame is one volatile record: state -1 at entry and
   0 before the sequence of 0x785790 calls (it is never set back). A 0xe0-byte
   local buffer `buf` is the thiscall object for 0x77bbe0 and every 0x785790
   call (passed as ECX, the same address each time).
   Guards: A, A+0x61 and B must be non-zero. When A+0x39 is non-zero, the global
   at 0x00b7431c is the object for thiscall 0x7875e0 (argument A+0x39), and a
   zero result exits. Then buf is set up by 0x77bbe0 (args 0xbc8, 0); 0x785790
   is called with (index, value) = (0, [A]), (1, [A+4]+0x33), (2, [A+8]+0x33),
   (3, [A+0xc]+0x33), (4, [B]), and (5, flag) where flag is 1 if 0x5444a0(A, 0)
   is non-zero. Finally 0x7861e0 is thiscall on the global at 0x00b7431c with
   (buf, [A+0x39]). */
void CM_CDECL cm_fn_005882d0(void *arg_04, void *arg_08)
{
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u8 buf[0xe0];
    u32 a;
    u32 b;
    u32 p0;
    u32 p1;
    u32 p2;
    u32 p3;
    u32 p4;
    u32 v;
    u32 flag;
    u32 g;

    a = (u32)arg_04;
    b = (u32)arg_08;

    seh.state = -1;
    seh.handler = (void *)0x00958b9b;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    if (a == 0)
        goto done;
    if (*(volatile u32 *)(a + 0x61) == 0)
        goto done;
    if (b == 0)
        goto done;
    if (*(volatile u32 *)(a + 0x39) != 0) {
        g = *(volatile u32 *)0x00b7431c;
        if (cm_fn_007875e0((void *)g, *(volatile u32 *)(a + 0x39)) == 0)
            goto done;
    }

    cm_fn_0077bbe0((void *)buf, 0xbc8u, 0);

    p0 = *(volatile u32 *)a;
    seh.state = 0;
    cm_fn_00785790((void *)buf, 0, p0);

    p1 = *(volatile u32 *)(a + 4);
    v = *(volatile u32 *)(p1 + 0x33);
    cm_fn_00785790((void *)buf, 1, v);

    p2 = *(volatile u32 *)(a + 8);
    v = *(volatile u32 *)(p2 + 0x33);
    cm_fn_00785790((void *)buf, 2, v);

    p3 = *(volatile u32 *)(a + 0xc);
    v = *(volatile u32 *)(p3 + 0x33);
    cm_fn_00785790((void *)buf, 3, v);

    p4 = *(volatile u32 *)b;
    cm_fn_00785790((void *)buf, 4, p4);

    flag = (cm_fn_005444a0((int)a, 0) != 0) ? 1u : 0u;
    cm_fn_00785790((void *)buf, 5, flag);

    v = *(volatile u32 *)(a + 0x39);
    g = *(volatile u32 *)0x00b7431c;
    cm_fn_007861e0((void *)g, (u32)buf, v);

done:
    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
}
