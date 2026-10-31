#include "cmre.h"

/* Entry: CDECL with no stack arguments (ret 0). One SEH record sits above the
   frame: { prev, handler 0x0095b9bb, state } (state -1 on entry, 0 around each
   match). The frame scratch (0x174 bytes, byte offsets from the frame base) is
   one block: word 0x00 loop index (kept in a local), words 0x04..0x93 hold 36
   record pointers and 0x04 is the first argument of 0x00945333; 0x94 is the
   object buffer for 0x0077bbe0, 0x00785790 and 0x007861e0.
   Record fields: 0x57 holds a pointer compared with the two table pointers and
   rewritten at the end of each iteration; records are 0x245 bytes apart. */

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_007875e0(int self, int param_1);
extern void CM_CDECL cm_fn_00945333(u32 *param_1, u32 *param_2, u32 param_3, void *param_4);

void CM_CDECL cm_fn_00642630(void)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 scratch[93];
    u32 prev;
    u32 g_a;
    u32 g_b;
    u32 g_c;
    u32 g_d;
    u32 g_e;
    u32 g_f;
    u32 g_f2;
    u32 ptr_a;
    u32 ptr_b;
    u32 chosen;
    u32 rec;
    u32 rec_k;
    u32 rec2;
    u32 rec3;
    u32 field;
    u32 i;
    u32 k;
    u32 count_eax;
    u32 v0;
    u32 v1;
    u8 cnt;
    s32 idx;

    prev = CM_FS(u32, 0);
    seh.state = -1;
    seh.handler = 0x0095b9bbu;
    seh.prev = prev;
    g_a = CM_GLOBAL(u32, 0x009cf578);
    CM_FS(u32, 0) = (u32)&seh;

    cnt = 0;
    g_d = CM_GLOBAL(u32, 0x00ae23d0);
    ptr_a = g_d + 107u * g_a;
    g_b = CM_GLOBAL(u32, 0x009cf57c);
    g_c = CM_GLOBAL(u32, 0x00ae235c);
    ptr_b = g_d + 107u * g_b;
    g_e = CM_GLOBAL(u32, 0x00ae23b4);

    /* Collect up to 36 records whose field 0x57 equals either table pointer. */
    if ((s32)g_c > 0) {
        rec = g_e;
        i = 0;
        do {
            field = *(volatile u32 *)(rec + 0x57u);
            if (field == ptr_a || field == ptr_b) {
                idx = (s32)(s8)cnt;
                cnt = (u8)(cnt + 1u);
                scratch[1 + idx] = rec;
                if (cnt == 0x24u) {
                    break;
                }
            }
            i = i + 1u;
            rec = rec + 0x245u;
        } while ((s32)(s16)i < (s32)g_c);
    }

    count_eax = (u32)(s32)(s8)cnt;
    cm_fn_00945333(&scratch[1], (u32 *)count_eax, 4u, (void *)0x0053fd40u);

    /* Walk all 36 slots (the loop runs regardless of the collected count). */
    k = 0;
    do {
        chosen = ((s32)(s16)k < 0x12) ? ptr_b : ptr_a;
        rec = scratch[1 + k];
        if (*(volatile u32 *)(rec + 0x57u) != chosen) {
            g_f = CM_GLOBAL(u32, 0x00b7431c);
            if (cm_fn_007875e0((int)g_f, (int)rec) != 0) {
                cm_fn_0077bbe0((void *)&scratch[37], 0x13u, 0u);
                rec2 = scratch[1 + k];
                seh.state = 0;
                v0 = *(volatile u32 *)rec2;
                cm_fn_00785790((void *)&scratch[37], 0u, v0);
                v1 = *(volatile u32 *)chosen;
                cm_fn_00785790((void *)&scratch[37], 1u, v1);
                g_f2 = CM_GLOBAL(u32, 0x00b7431c);
                rec3 = scratch[1 + k];
                cm_fn_007861e0((void *)g_f2, (u32)&scratch[37], rec3);
                seh.state = -1;
            }
        }
        rec_k = scratch[1 + k];
        k = k + 1u;
        *(volatile u32 *)(rec_k + 0x57u) = chosen;
    } while ((s32)(s16)k < 0x24);

    CM_FS(u32, 0) = seh.prev;
}
