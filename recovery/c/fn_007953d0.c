/* fn_007953d0 -- preferred-image VA 0x007953d0 (image base 0x00400000), 371 bytes.
 *
 * Resolved translation. The earlier WIP in this file was blocked on one register
 * boundary (0x007954d6); it is resolved here, see "0x0054c770 boundary" below.
 *
 * Listing facts used:
 *  - Entry: ECX is read first (thiscall receiver). `ret 0x14` pops five stack
 *    arguments; the argument homes are E0+4, E0+8, E0+0c, E0+10, E0+14 where E0
 *    is the entry ESP. arg4 is read as a sign-extended BYTE (0x00795493),
 *    arg5 as a BYTE (0x00795485). arg1/arg2/arg3 are read as DWORDs.
 *  - SEH frame (0x007953d0..0x007953de): push -1 (state), push 0x0095ea04
 *    (handler), push fs:[0] (prev), fs:[0] = esp. It is one volatile
 *    { prev, handler, state } record; the state is written as a DWORD at
 *    0x00795403 (0) and 0x00795427 (1) and as a low BYTE afterwards
 *    (0x007954ba = 4, 0x007954db = 3, 0x007954fb = 2, 0x0079551d = 1).
 *  - Scratch block: `sub esp, 0x144` allocates L..L+0x143; three saved
 *    registers (ebx/esi/edi) sit below it and the SEH frame sits above it at
 *    L+0x144 (prev), L+0x148 (handler), L+0x14c (state). All displacements in
 *    the listing are resolved against the live ESP, so the buffer offsets below
 *    are the original physical offsets from L:
 *      L+0x000  param_2 of the first 0x00417c10 (L itself)
 *      L+0x008  param_2 of 0x00417c10 on the arg4==2 path
 *      L+0x010  param_2 of 0x0054c770 on the arg4==1 path
 *      L+0x018  param_2 of 0x0054c770 on the arg4==2 path
 *      L+0x020  param_2 of 0x00417c10 on the arg4==1 path
 *      L+0x028  param_2 of 0x00417c10 on the default path
 *      L+0x030  DWORD copied from [arg2]; also the source of the final copy
 *      L+0x034  destination of the 0xff-byte copy of arg3
 *      L+0x132  NUL terminator written at 0x00795472
 *      L+0x133  object built by 0x0054a110, receiver of the first 0x00417c10
 *      L+0x13b  object built by 0x0054a110, receiver of the second 0x00417c10
 *      L+0x143  low byte of arg5 (0x0079548c)
 *    The final copy (0x00795524) takes 0x114 bytes from L+0x30, i.e. exactly
 *    L+0x30..L+0x143.
 *  - Index: esi = [0x00ae2364] - [arg1] - 1 (0x0079541a..0x00795431), used
 *    signed: `js` then `cmp esi,0x10 / jge`. The destination object is
 *    [edi + esi*8] with edi = the receiver (0x00795510).
 *  - 0x0054c770 boundary (0x007954d6, 0x007954f6): the callee is declared void
 *    in the brief, but its own listing ends with `mov eax, [esp+0x14]`, i.e.
 *    its param_2 slot, and the caller pushes that EAX afterwards as param_3 of
 *    0x00417c10. The value is therefore observable, so the boundary is
 *    declared u32-returning here: the original code runs unchanged and leaves
 *    exactly that EAX in the register. (The earlier WIP cast the void callee
 *    to a u32-returning pointer, which the build rejects with
 *    -Werror=cast-function-type; declaring the real return width needs no cast.)
 *  - 0x0068d0d0 is called with ECX = the destination object, [esp] = the source
 *    and [esp+4] = the count (0x114), and it pops those eight bytes (the
 *    following `mov ecx,[esp+0x150]` at 0x00795529 reads the SEH prev slot
 *    only when the stack is back at E0-0x15c). Its brief signature reserves
 *    EDX, but the original call site never loads EDX before the call, so the
 *    EDX input is the residue of the preceding original call; this translation
 *    does not write EDX and leaves it to the same residue instead of inventing
 *    a value.
 */
#include "cmre.h"

/* Original callees, bound to their preferred-image addresses. */
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern short * CM_THISCALL cm_fn_00549f70(short *param_1, short param_2, char param_3, u16 param_4, u32 param_5);
/* Return width from the callee's own listing (`mov eax, [esp+0x14]`), see above. */
extern u32 CM_THISCALL cm_fn_0054c770(short *param_1, u32 *param_2, u8 param_3);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(volatile u8 *self, const u8 *src, s16 count);
extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);

void CM_THISCALL cm_fn_007953d0(u32 self, u32 arg1, u32 arg2, u32 arg3, s8 arg4, u8 arg5)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 scratch[0x55];              /* 0x144 bytes of original scratch, 4-byte aligned */
    u8 *sc = (u8 *)scratch;
    u32 g;
    u32 v;
    u32 idx;
    u32 p2_417;
    u32 p3_417;

    /* 0x007953d0: push -1 / push 0x0095ea04 / push fs:[0] / fs:[0] = esp */
    seh.state = -1;
    seh.handler = 0x0095ea04u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    /* 0x007953f0: lea ecx, [esp+0x13f] -> L+0x133 ; call 0x0054a110 */
    cm_fn_0054a110((u16 *)(sc + 0x133u));
    /* 0x00795403: mov dword ptr [esp+0x158], 0 */
    seh.state = 0;
    /* 0x007953fc: lea ecx, [esp+0x147] -> L+0x13b ; call 0x0054a110 */
    cm_fn_0054a110((u16 *)(sc + 0x13bu));

    /* 0x00795413..0x00795427 */
    g = (u32)CM_GLOBAL(s32, 0x00ae2364u);
    v = *(u32 *)arg1;
    seh.state = 1;
    /* 0x0079542e..0x0079543a: esi = g - [arg1] - 1, signed range test */
    idx = g - v - 1u;

    if ((s32)idx >= 0 && (s32)idx < 0x10) {
        /* 0x00795440..0x0079545f */
        v = *(u32 *)arg2;
        *(volatile u32 *)(sc + 0x30u) = v;
        cm_fn_009466d0((u32)(sc + 0x34u), arg3, 0xffu);

        /* 0x00795472: mov byte ptr [esp+0x13e], 0 -> L+0x132 */
        *(volatile u8 *)(sc + 0x132u) = 0u;
        /* 0x0079547a: push 0xae2c90 ; push edx(=L) ; call 0x00417c10 */
        cm_fn_00417c10((u32 *)(sc + 0x133u), (u32 *)sc, (u16 *)0x00ae2c90u);

        /* 0x0079548c: mov byte ptr [esp+0x14f], al -> L+0x143 */
        *(volatile u8 *)(sc + 0x143u) = (u8)arg5;

        /* 0x00795493: movsx eax, byte ptr [esp+0x16c] */
        if (arg4 == 1) {
            /* 0x007954e6: lea ecx, [esp+0x1c] -> L+0x10 ; push 1 ; push ecx ;
               mov ecx, 0xae2c90 ; call 0x0054c770 ; push eax */
            p3_417 = cm_fn_0054c770((short *)0x00ae2c90u, (u32 *)(sc + 0x10u), 1u);
            /* 0x007954fb: state low byte = 2 */
            *(volatile u8 *)&seh.state = 2u;
            /* 0x007954f6: lea edx, [esp+0x2c] -> L+0x20 */
            p2_417 = (u32)(sc + 0x20u);
        } else if (arg4 == 2) {
            /* 0x007954c5: lea edx, [esp+0x24] -> L+0x18 ; push 4 ; push edx ;
               mov ecx, 0xae2c90 ; call 0x0054c770 ; push eax */
            p3_417 = cm_fn_0054c770((short *)0x00ae2c90u, (u32 *)(sc + 0x18u), 4u);
            /* 0x007954db: state low byte = 3 */
            *(volatile u8 *)&seh.state = 3u;
            /* 0x007954d7: lea eax, [esp+0x18] -> L+0x08 */
            p2_417 = (u32)(sc + 0x08u);
        } else {
            /* 0x007954a1: push -1 ; push 0x7b3 ; push 7 ; push 0x17 ;
               lea ecx, [esp+0x1c] -> L ; call 0x00549f70 ; push eax */
            p3_417 = (u32)cm_fn_00549f70((short *)sc, 0x17, 7, 0x7b3u, 0xffffffffu);
            /* 0x007954ba: state low byte = 4 */
            *(volatile u8 *)&seh.state = 4u;
            /* 0x007954b5: lea ecx, [esp+0x34] -> L+0x28 */
            p2_417 = (u32)(sc + 0x28u);
        }

        /* 0x00795504: lea ecx, [esp+0x14f] -> L+0x13b ; call 0x00417c10 */
        cm_fn_00417c10((u32 *)(sc + 0x13bu), (u32 *)p2_417, (u16 *)p3_417);

        /* 0x00795510: mov ecx, dword ptr [edi + esi*8] */
        v = *(volatile u32 *)(self + idx * 8u);
        /* 0x0079551d: mov byte ptr [esp+0x160], bl -> state low byte = 1 */
        *(volatile u8 *)&seh.state = 1u;
        /* 0x00795513: lea eax, [esp+0x3c] -> L+0x30 ; push 0x114 ; push eax ;
           call 0x0068d0d0 */
        cm_fn_0068d0d0((volatile u8 *)v, (const u8 *)(sc + 0x30u), 0x114);
    }

    /* 0x00795529..0x00795533: restore the SEH chain */
    CM_FS(u32, 0) = seh.prev;
}
