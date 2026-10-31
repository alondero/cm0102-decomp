#include "cmre.h"

/* Thiscall with three stack arguments (ret 0x0c). ECX is the object pointer
   (self); it is copied to ebx on entry and read at [ebx] and [ebx+4] later.
   arg2 is the dword at [esp+4] on entry; arg3 and arg4 are the low bytes of
   the next two stack slots, read with movsx (signed).

   SEH frame, original offsets from the entry esp E: prev at E-0x0c (fs:[0]
   points here), handler at E-0x08 (pushed 0x0095cf4b), state at E-0x04
   (pushed -1, then set to 0 after the 0x0077bbe0 call and before 0x00785790).
   Local buffer local_ec is [esp+0] after `sub esp, 0xe0` (L = E-0xec), so
   it occupies L+0 .. L+0xdf, directly below the frame. */

extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 arg1);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1,
                                                     u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg,
                                                     u32 index_arg,
                                                     u32 value_arg);
extern u8 CM_THISCALL cm_fn_005986b0(u32 self);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1,
                                                     u32 arg2);

void CM_THISCALL cm_fn_006a9350(u32 self, u32 arg2, s8 arg3, s8 arg4)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 local_ec[0xe0];

    seh.state = -1;
    seh.handler = 0x0095cf4bu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    if (arg2 != 0) {
        u32 obj = cm_fn_005a0590(CM_GLOBAL(volatile u32, 0x00ae2a38u), arg2);

        if (obj != 0) {
            u32 v0;
            u32 v1;
            u32 self_dword0;
            s16 field_word;
            u8 al;
            s8 cvar;
            u8 flag;
            s32 f4;
            u32 f0;

            (void)cm_fn_0077bbe0(local_ec, 0x1786u, 0u);

            /* Reads the dword at arg2 before the state store. */
            v0 = *(volatile u32 *)arg2;
            seh.state = 0;
            (void)cm_fn_00785790(local_ec, 0u, v0);
            (void)cm_fn_00785790(local_ec, 1u, (u32)(s32)arg3);
            (void)cm_fn_00785790(local_ec, 2u, (u32)(s32)arg4);

            /* Reads arg2 dword, then self dword, then the signed 16-bit field
               at self_dword0 + 73 * v1 + 6 (the two lea instructions compute
               73 * v1 in edx). */
            v1 = *(volatile u32 *)arg2;
            self_dword0 = *(volatile u32 *)self;
            field_word = *(volatile s16 *)(self_dword0 + v1 * 73u + 6u);
            (void)cm_fn_00785790(local_ec, 3u, (u32)(s32)field_word);

            /* Receiver is obj (mov ecx, edi). Result is the low byte (al). */
            al = cm_fn_005986b0(obj);
            cvar = (s8)al;
            /* Signed: al < -2 or al >= 0 gives 0; otherwise 1. */
            if (cvar < -2 || cvar >= 0) {
                flag = 0u;
            } else {
                flag = 1u;
            }

            /* Dword at obj+4 (signed) first; when zero, the dword at obj
               (unsigned, against 1000000) decides. */
            f4 = *(volatile s32 *)(obj + 4u);
            if (f4 > 0) {
                /* keep flag */
            } else if (f4 < 0) {
                flag = 1u;
            } else {
                f0 = *(volatile u32 *)obj;
                if (f0 < 1000000u) {
                    flag = 1u;
                }
            }

            (void)cm_fn_00785790(local_ec, 4u, (u32)(s32)(s8)flag);
            (void)cm_fn_00785790(local_ec, 5u, 0u);
            (void)cm_fn_007861e0((void *)CM_GLOBAL(volatile u32, 0x00b7431cu),
                                 (u32)local_ec, arg2);
        }
    }

    CM_FS(u32, 0) = seh.prev;
}
