/* WIP: register boundary at 0x007954d6. The callee 0x0054c770 is declared void in the brief,
   but its listing ends with `mov eax, [esp+0x14]` (its param_2), and the caller pushes EAX
   after it (0x007954d6, 0x007954e3 paths). The call-site cast to a u32-returning type is
   rejected by the build (-Werror=cast-function-type). Blocking question: may 0x0054c770 be
   declared with a u32 return, or is a different call-site form approved? */
#include "cmre.h"

/* Thiscall (ECX is read first and kept as the receiver), ret 0x14: five stack
   arguments after the receiver. Stack arguments by ESP entry offset:
   arg1 at +4 (dword, read through as a pointer), arg2 at +8 (dword pointer),
   arg3 at +0x0c (dword value used as a copy source), arg4 at +0x10 (low byte,
   sign-extended), arg5 at +0x14 (low byte).

   Receiver: a table of 8-byte elements; the dword at receiver + 8*k is read
   after the calls, where k = G - [arg1] - 1 for the global G at 0x00ae2364.

   Scratch block of 0x144 bytes (listing L = ESP after `sub esp, 0x144`):
   0x000 first p2 of 0x00417c10, 0x008 its p2 on path 2, 0x010 the 0x0054c770 p2
   on path 1, 0x018 the 0x0054c770 p2 on path 2, 0x020 its p2 on path 1, 0x028 its
   p2 on the default path, 0x030 a dword copied from [arg2] followed by a 255-byte
   copy of the arg3 buffer from 0x034, 0x132 terminator byte, 0x133 and 0x13b
   receivers for 0x0054a110 and 0x00417c10, 0x143 low byte of arg5.
   The SEH frame sits above the scratch block: { prev, handler, state } at
   L+0x144, +0x148, +0x14c. Its state is written as a dword or as a low byte,
   exactly as the listing does. */

extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern short * CM_THISCALL cm_fn_00549f70(short *param_1, short param_2, char param_3, u16 param_4, u32 param_5);
extern void CM_THISCALL cm_fn_0054c770(short *param_1, u32 *param_2, u8 param_3);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(volatile u8 *self, const u8 *src, s16 count);
extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);

/* cm_fn_0054c770 is declared void. Its listing ends with `mov eax, [esp+0x14]`
   (its param_2), so EAX after the call is that pointer. The caller pushes EAX,
   so the call site reads EAX through this pointer type. */
typedef u32 (CM_THISCALL *cm_fn_0054c770_eax_t)(void *self, u32 *param_2, u8 param_3);

void CM_THISCALL cm_fn_007953d0(u32 self, u32 arg1, u32 arg2, u32 arg3, s8 arg4, u8 arg5)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0x144];
    u32 sc;
    u32 g;
    u32 v;
    u32 eax_770;
    u32 p2_417;
    u32 p3_417;
    s32 k;
    s32 t;

    seh.state = -1;
    seh.handler = 0x0095ea04u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    sc = (u32)scratch;

    cm_fn_0054a110((u16 *)(sc + 0x133u));
    seh.state = 0;
    cm_fn_0054a110((u16 *)(sc + 0x13bu));

    g = (u32)CM_GLOBAL(s32, 0x00ae2364u);
    v = *(volatile u32 *)arg1;
    seh.state = 1;
    k = (s32)(g - v - 1u);

    if (k >= 0 && k < 0x10) {
        v = *(volatile u32 *)arg2;
        *(volatile u32 *)(sc + 0x30u) = v;
        cm_fn_009466d0(sc + 0x34u, arg3, 0xffu);

        scratch[0x132] = 0u;
        cm_fn_00417c10((u32 *)(sc + 0x133u), (u32 *)sc, (u16 *)0x00ae2c90u);

        *(volatile u8 *)(sc + 0x143u) = arg5;

        t = (s32)arg4;
        if (t == 1) {
            /* Path 1: 0x0054c770 with p2 = L+0x10 and p3 = 1; its EAX is
               p2, pushed as the second argument of 0x00417c10. */
            eax_770 = ((cm_fn_0054c770_eax_t)(void *)cm_fn_0054c770)((void *)0x00ae2c90u, (u32 *)(sc + 0x10u), 1u);
            p3_417 = eax_770;
            *(volatile u8 *)&seh.state = 2u;
            p2_417 = sc + 0x20u;
        } else if (t == 2) {
            /* Path 2: 0x0054c770 with p2 = L+0x18 and p3 = 4. */
            eax_770 = ((cm_fn_0054c770_eax_t)(void *)cm_fn_0054c770)((void *)0x00ae2c90u, (u32 *)(sc + 0x18u), 4u);
            p3_417 = eax_770;
            *(volatile u8 *)&seh.state = 3u;
            p2_417 = sc + 0x08u;
        } else {
            /* Default: 0x00549f70 with receiver L; its returned pointer is the
               second argument of 0x00417c10. */
            p3_417 = (u32)cm_fn_00549f70((short *)sc, 0x17, 7, 0x7b3u, 0xffffffffu);
            *(volatile u8 *)&seh.state = 4u;
            p2_417 = sc + 0x28u;
        }

        cm_fn_00417c10((u32 *)(sc + 0x13bu), (u32 *)p2_417, (u16 *)p3_417);

        v = *(volatile u32 *)(self + (u32)k * 8u);
        *(volatile u8 *)&seh.state = 1u;
        cm_fn_0068d0d0((volatile u8 *)v, (const u8 *)(sc + 0x30u), 0x114);
    }

    CM_FS(u32, 0) = seh.prev;
}
