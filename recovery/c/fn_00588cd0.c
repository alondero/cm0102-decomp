#include "cmre.h"

/* Four stack arguments, popped by the caller (ret 0, no ECX on entry).
   Stack words after the prologue: arg1 = esi, arg2 = edi, arg3 = ebx, arg4 at
   the original [esp+0x108].
   The original SEH record sits directly above the 0xe0-byte local area; the
   locals are passed to callees as the thiscall object (lea ecx). */

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern int CM_THISCALL cm_fn_004cf5a0(int *param_1, int *param_2);
extern u8 CM_STDCALL cm_fn_004ca470(u32 param_1, u32 param_2);
extern int cm_fn_0090cfc0(int param_1);

void CM_CDECL cm_fn_00588cd0(u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 locals[0xe0];
    u32 value;
    u32 result;
    u32 nine;
    s32 iv;
    s32 iv7;
    s8 c;

    seh.state = -1;
    seh.handler = 0x00958bfbu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    if (arg1 == 0) goto unlink;
    if (*(volatile u32 *)(arg1 + 0x61) == 0) goto unlink;
    if (arg2 == 0) goto unlink;
    if (*(volatile u32 *)(arg2 + 0x69) == 0) goto unlink;
    if (arg3 == 0) goto unlink;

    cm_fn_0077bbe0(locals, 0x138du, 0u);
    value = *(volatile u32 *)arg1;
    seh.state = 0;
    cm_fn_00785790(locals, 0u, value);
    value = *(volatile u32 *)(*(volatile u32 *)(arg1 + 4) + 0x33);
    cm_fn_00785790(locals, 1u, value);
    value = *(volatile u32 *)(*(volatile u32 *)(arg1 + 8) + 0x33);
    cm_fn_00785790(locals, 2u, value);
    value = *(volatile u32 *)(*(volatile u32 *)(arg1 + 0xc) + 0x33);
    cm_fn_00785790(locals, 3u, value);
    value = *(volatile u32 *)arg2;
    cm_fn_00785790(locals, 4u, value);
    value = *(volatile u32 *)(*(volatile u32 *)(arg2 + 4) + 0x33);
    cm_fn_00785790(locals, 5u, value);
    value = *(volatile u32 *)(*(volatile u32 *)(arg2 + 8) + 0x33);
    cm_fn_00785790(locals, 6u, value);
    value = *(volatile u32 *)(*(volatile u32 *)(arg2 + 0xc) + 0x33);
    cm_fn_00785790(locals, 7u, value);

    if (arg4 != 0) {
        s8 b;
        s32 sel;

        b = (s8)*(volatile u8 *)(arg1 + 0x18);
        sel = (b <= 0x18) ? 3 : 2;
        iv = cm_fn_0090cfc0(sel);
        cm_fn_00785790(locals, 8u, (u32)iv);
        nine = 999u;
    } else {
        iv = cm_fn_004cf5a0((int *)CM_GLOBAL(u32, 0xae1050), (int *)arg1);
        if (iv != 0) {
            (void)CM_GLOBAL(volatile u32, 0xae1050);
            c = (s8)cm_fn_004ca470(arg1, (u32)iv);
            if (c < 4) {
                iv7 = cm_fn_0090cfc0(7);
                if ((s32)(s8)*(volatile u8 *)(arg1 + 0x5b) < iv7) {
                    result = 0u;
                    goto have;
                }
                iv7 = cm_fn_0090cfc0(7);
                if ((s32)(s8)*(volatile u8 *)(arg1 + 0x57) < iv7) {
                    result = 1u;
                    goto have;
                }
                if ((s8)*(volatile u8 *)(arg1 + 0x18) < 0x15) {
                    iv7 = cm_fn_0090cfc0(8);
                    if ((s32)(s8)*(volatile u8 *)(arg1 + 0x5a) < iv7) {
                        result = 2u;
                        goto have;
                    }
                }
                if ((*(volatile u32 *)((u32)iv + 0x45u) & 0x30321u) != 0) {
                    result = 3u;
                    goto have;
                }
                result = (u32)cm_fn_0090cfc0(4) + 5u;
                goto have;
            }
            if (c == 7) {
                result = 4u;
                goto have;
            }
        }
        /* Falls here when iv is 0 (the original then reads address 0x45) or
           when the status byte is 4 or more and not 7. */
        result = ((*(volatile u32 *)((u32)iv + 0x45u) & 0x30321u) != 0) ? 3u : 5u;
    have:
        cm_fn_00785790(locals, 8u, result);
        nine = 0u;
    }

    cm_fn_00785790(locals, 9u, nine);
    cm_fn_007861e0((void *)CM_GLOBAL(u32, 0xb7431c), (u32)locals, arg3);

unlink:
    CM_FS(u32, 0) = seh.prev;
}
