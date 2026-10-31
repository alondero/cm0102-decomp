/* Preferred-image VA 0x00515190 (image base 0x00400000). No stack arguments,
   plain `ret`, no register read before written: cdecl.

   Frame: the SEH record {prev, handler 0x009573bb, state} sits above a 0x174-byte
   scratch area (S = esp after `sub esp, 0x174`; frame prev at S+0x174, state at
   S+0x17c). Scratch offsets, from the listing with esp = S-0x10 after the four
   saved registers:
     0x00      loop counter (dword, [esp+0x10])
     0x04..0x83  record pointer array A[0..31] (dword each, [esp+0x14])
     0x94..    object buffer for the 0x0077bbe0 / 0x00785790 / 0x007861e0 calls
   Entries of A past the first `count` written entries are never initialised by
   the listing and are read anyway by the second loop; they stay uninitialised here. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007875e0(u32 self, int param_1);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self,u32 arg1,u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self,u32 arg1,u32 arg2);
extern void cm_fn_00945333(u32 *param_1,u32 *param_2,u32 param_3,void *param_4);

void CM_CDECL cm_fn_00515190(void)
{
    volatile struct { u32 prev; u32 handler; s32 state; } frame;
    volatile u8 scratch[0x174];
    u32 prev_value;
    u32 g1;
    u32 g2;
    u32 g3;
    u32 g4;
    u32 g5;
    u32 g6;
    u32 edi_value;
    u32 ebp_value;
    u32 ebx_value;
    u32 rec;
    u32 esi_value;
    u32 edx_value;
    u32 eax_value;
    u32 ebx_read;
    u32 cnt_index;
    u8 cnt;
    s32 count;
    s32 ecx_index;
    s32 ctr;

    prev_value = CM_FS(u32, 0);
    frame.state = -1;
    frame.handler = 0x009573bbu;
    frame.prev = prev_value;
    g1 = CM_GLOBAL(volatile u32, 0x009cf8a8u);
    CM_FS(u32, 0) = (u32)&frame;

    cnt = 0;
    g2 = CM_GLOBAL(volatile u32, 0x00ae23d0u);
    g3 = CM_GLOBAL(volatile u32, 0x009cf8acu);
    g4 = CM_GLOBAL(volatile u32, 0x00ae235cu);
    g5 = CM_GLOBAL(volatile u32, 0x00ae23b4u);
    edi_value = g2 + 107u * g1;
    ebp_value = g2 + 107u * g3;

    if ((s32)g4 > 0) {
        rec = g5;
        ecx_index = 0;
        for (;;) {
            esi_value = *(volatile u32 *)(rec + 0x57u);
            if (esi_value == edi_value || esi_value == ebp_value) {
                cnt_index = (u32)(s32)(s8)cnt;
                cnt = (u8)(cnt + 1u);
                *(volatile u32 *)&scratch[4u + cnt_index * 4u] = rec;
                if (cnt == 0x20) {
                    break;
                }
            }
            ecx_index = ecx_index + 1;
            rec = rec + 0x245u;
            if (!(ecx_index < (s32)g4)) {
                break;
            }
        }
    }

    count = (s32)(s8)cnt;
    (void)cm_fn_00945333((u32 *)&scratch[4], (u32 *)(u32)count, 4u, (void *)0x0053fd40u);

    *(volatile s32 *)&scratch[0] = 0;
    for (;;) {
        ctr = *(volatile s32 *)&scratch[0];
        ebx_value = (ctr < 0x10) ? edi_value : ebp_value;
        rec = *(volatile u32 *)&scratch[4u + (u32)ctr * 4u];

        if (*(volatile u32 *)(rec + 0x57u) != ebx_value) {
            if (cm_fn_007875e0(CM_GLOBAL(volatile u32, 0x00b7431cu), (int)rec) != 0) {
                (void)cm_fn_0077bbe0((void *)&scratch[0x94], 0x13u, 0u);
                edx_value = *(volatile u32 *)&scratch[4u + (u32)ctr * 4u];
                frame.state = 0;
                eax_value = *(volatile u32 *)edx_value;
                (void)cm_fn_00785790((void *)&scratch[0x94], 0u, eax_value);
                ebx_read = *(volatile u32 *)ebx_value;
                (void)cm_fn_00785790((void *)&scratch[0x94], 1u, ebx_read);
                edx_value = *(volatile u32 *)&scratch[4u + (u32)ctr * 4u];
                g6 = CM_GLOBAL(volatile u32, 0x00b7431cu);
                (void)cm_fn_007861e0((void *)g6, (u32)&scratch[0x94], edx_value);
                frame.state = -1;
            }
        }

        ctr = *(volatile s32 *)&scratch[0];
        rec = *(volatile u32 *)&scratch[4u + (u32)ctr * 4u];
        ctr = ctr + 1;
        *(volatile u32 *)(rec + 0x57u) = ebx_value;
        *(volatile s32 *)&scratch[0] = ctr;
        if (ctr < 0x20) {
            continue;
        }
        break;
    }

    CM_FS(u32, 0) = frame.prev;
}
