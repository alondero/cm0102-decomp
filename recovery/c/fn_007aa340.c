#include "cmre.h"

/* Stdcall (ret 8): obj is the first stack argument (listing [esp+0x16c] after
   the prologue, entry ESP + 4); arg_char is the second stack argument, read by
   the listing only as its low byte (movsx ecx, byte ptr [esp+0x170]).

   Object fields by byte offset from obj: 0x00 dword, 0x04 and 0x08 and 0x0c
   object pointers (each followed by a dword read at +0x33), 0x18 signed byte,
   0x3d byte, 0x61 dword.

   Scratch block of 0x158 bytes (listing L = ESP right after `sub esp, 0x158`):
   offset 0x000 is the receiver for 0x0077bbe0 and 0x00785790 and the first
   argument of 0x007861e0; offset 0x0e0 is the buffer that 0x008d1da0 fills and
   the loop reads back. These are one allocation with explicit offsets.

   The SEH frame sits above the scratch block: { prev, handler, state } at
   entry ESP - 0x0c, - 0x08 and - 0x04. */

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern int CM_THISCALL cm_fn_004cf5a0(int *param_1, int *param_2);
/* 0x008d1da0 reads ECX on entry: the receiver is the [0x00de1f54] value the
   listing loads and tests at 0x7aa454. */
extern char CM_THISCALL cm_fn_008d1da0(u32 self, int param_1, u32 param_2, int *param_3, char param_4);

void CM_STDCALL cm_fn_007aa340(u32 obj, s8 arg_char)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 scratch[0x158 / 4];
    u32 sc;
    u32 value;
    u32 p4;
    u32 p8;
    u32 pc;
    u32 ret_obj;
    u32 x;
    u32 p;
    u32 cursor;
    u32 receiver;
    s32 remaining;
    u8 kind;
    s8 cnt;
    int run;

    seh.state = -1;
    seh.handler = 0x0095f24bu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    sc = (u32)scratch;

    run = 0;
    if (obj != 0u) {
        if (*(volatile u32 *)(obj + 0x61u) == 0u) {
            run = 1;
        } else {
            kind = *(volatile u8 *)(obj + 0x3du);
            if (kind != 8u && kind != 5u) {
                run = 1;
            }
        }
    }

    if (run) {
        cm_fn_0077bbe0((void *)sc, 0xbbbu, 0u);

        value = *(volatile u32 *)obj;
        seh.state = 0;
        cm_fn_00785790((void *)sc, 0u, value);

        p4 = *(volatile u32 *)(obj + 0x04u);
        value = *(volatile u32 *)(p4 + 0x33u);
        cm_fn_00785790((void *)sc, 1u, value);

        p8 = *(volatile u32 *)(obj + 0x08u);
        value = *(volatile u32 *)(p8 + 0x33u);
        cm_fn_00785790((void *)sc, 2u, value);

        pc = *(volatile u32 *)(obj + 0x0cu);
        value = *(volatile u32 *)(pc + 0x33u);
        cm_fn_00785790((void *)sc, 3u, value);

        cm_fn_00785790((void *)sc, 4u, (u32)(s32)arg_char);

        ret_obj = (u32)cm_fn_004cf5a0((int *)CM_GLOBAL(u32, 0x00ae1050u), (int *)obj);
        CM_GLOBAL(u32, 0x00ae1080u) = ret_obj;

        if (ret_obj == 0u || (*(volatile u8 *)(ret_obj + 0x35u) & 0x3fu) == 5u) {
            value = 0xffffffffu;
        } else {
            x = *(volatile u32 *)(ret_obj + 0x04u);
            p = CM_GLOBAL(u32, 0x00ae23b4u) + x * 0x245u;
            if (p == 0u) {
                value = 0xffffffffu;
            } else {
                value = *(volatile u32 *)p;
            }
        }
        cm_fn_00785790((void *)sc, 5u, value);

        cm_fn_00785790((void *)sc, 6u, (u32)(s32)*(volatile s8 *)(obj + 0x18u));

        receiver = CM_GLOBAL(volatile u32, 0x00de1f54u);
        if (receiver == 0u) {
            cnt = 0;
        } else {
            cnt = cm_fn_008d1da0(receiver, (int)obj, 0x0au, (int *)(sc + 0xe0u), 0x1e);
        }

        if (cnt > 0) {
            cursor = sc + 0xe0u;
            remaining = (s32)cnt;
            do {
                value = *(volatile u32 *)cursor;
                cm_fn_007861e0((void *)CM_GLOBAL(u32, 0x00b7431cu), sc, value);
                cursor = cursor + 4u;
                remaining = remaining - 1;
            } while (remaining != 0);
        }
    }

    CM_FS(u32, 0) = seh.prev;
}
