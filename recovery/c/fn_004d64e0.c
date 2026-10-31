#include "cmre.h"

/* Original frame (listing 0x004d64e0..0x004d65b3): an SEH record is pushed
   (prev, handler 0x00956eab, state -1), then esp moves down by 0xe0 for a
   local buffer. Local buffer base L is that esp. Pushes and pops below are
   tracked against L: a call receiver `lea ecx, [esp + N]` is L at each site. */

struct cm_seh_frame {
    void *prev;
    u32 handler;
    u32 state;
};

extern u32 CM_STDCALL cm_fn_007875e0(u32 param_1);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);

u32 CM_STDCALL cm_fn_004d64e0(u32 param_1, u32 param_2)
{
    volatile struct cm_seh_frame frame;
    u8 local[0xe0];
    /* EAX at return: `test eax, eax` leaves 0 on the skip path (je 0x4d659e);
       otherwise the low word of cm_fn_007861e0's result. The caller's use of
       EAX is not established, so the return is declared at 32 bits. */
    u32 eax_result = 0u;

    /* 0x004d64e6..0x004d64f4: push -1; push handler; push prev; mov ecx,[0xb7431c]. */
    /* mov eax, fs:[0] precedes the pushes; the record stores follow in
       push order: state, handler, prev. */
    void *prev = CM_FS(void *, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00956eabu;
    frame.prev = prev;
    (void)CM_GLOBAL(volatile u32, 0x00b7431cu);
    /* 0x004d64f4: mov fs:[0], esp. */
    CM_FS(void *, 0) = (void *)&frame;

    if (cm_fn_007875e0(param_2) != 0) {
        /* 0x004d6517..0x004d6523: push esi; push 0; push 0xbc4; ecx = L; call. */
        cm_fn_0077bbe0((void *)local, 0x00000bc4u, 0u);
        /* 0x004d652f..0x004d6533: ecx = L; state store is [esp+0xf0] = frame.state. */
        frame.state = 0u;
        /* 0x004d653e..0x004d6543: value = [param_1]; index 0. */
        cm_fn_00785790((void *)local, 0u, *(u32 *)param_1);
        /* 0x004d6548..0x004d6555: value = [[param_1+4] + 0x33]; index 1. */
        cm_fn_00785790((void *)local, 1u,
                       *(u32 *)(*(u32 *)(param_1 + 4u) + 0x33u));
        /* 0x004d655a..0x004d6567: value = [[param_1+8] + 0x33]; index 2. */
        cm_fn_00785790((void *)local, 2u,
                       *(u32 *)(*(u32 *)(param_1 + 8u) + 0x33u));
        /* 0x004d656c..0x004d6579: value = [[param_1+0xc] + 0x33]; index 3. */
        cm_fn_00785790((void *)local, 3u,
                       *(u32 *)(*(u32 *)(param_1 + 0xcu) + 0x33u));
        /* 0x004d657e..0x004d6587: value = [param_2]; index 4. */
        cm_fn_00785790((void *)local, 4u, *(u32 *)param_2);
        /* 0x004d658c..0x004d6598: ecx = [0xb7431c]; push param_2; push L. */
        eax_result = (u32)cm_fn_007861e0((void *)CM_GLOBAL(u32, 0x00b7431cu),
                                         (u32)local, param_2);
    }

    /* 0x004d659d..0x004d65a6: pop esi/edi; mov ecx,[esp+0xe4] = frame.prev;
       mov fs:[0], ecx. EAX is not changed by these instructions. */
    CM_FS(void *, 0) = frame.prev;
    return eax_result;
}
