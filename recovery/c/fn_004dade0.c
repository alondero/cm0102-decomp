#include "cmre.h"

/* Original frame (listing 0x004dade0..0x004daeb3): an SEH record is pushed
   (prev, handler 0x0095700b, state -1), then esp moves down by 0xe0 for a
   local buffer. Local buffer base L is that esp. Pushes and pops below are
   tracked against L: a call receiver `lea ecx, [esp + N]` is L at each site.
   Same shape as fn_004d64e0; differences are the handler (0x0095700b) and the
   first thiscall argument (0xbbc). */

struct cm_seh_frame {
    void *prev;
    u32 handler;
    u32 state;
};

extern u32 CM_STDCALL cm_fn_007875e0(u32 param_1);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);

u32 CM_STDCALL cm_fn_004dade0(u32 param_1, u32 param_2)
{
    volatile struct cm_seh_frame frame;
    u8 local[0xe0];
    /* EAX at return: `test eax, eax` leaves 0 on the skip path (je 0x4dae9e);
       otherwise the low word of cm_fn_007861e0's result. The caller's use of
       EAX is not established, so the return is declared at 32 bits. */
    u32 eax_result = 0u;

    /* 0x004dade6..0x004dadf4: push -1; push handler; push prev; mov ecx,[0xb7431c]. */
    /* mov eax, fs:[0] precedes the pushes; the record stores follow in
       push order: state, handler, prev. */
    void *prev = CM_FS(void *, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095700bu;
    frame.prev = prev;
    (void)CM_GLOBAL(volatile u32, 0x00b7431cu);
    /* 0x004dadf4: mov fs:[0], esp. */
    CM_FS(void *, 0) = (void *)&frame;

    if (cm_fn_007875e0(param_2) != 0) {
        /* 0x004dae17..0x004dae23: push esi; push 0; push 0xbbc; ecx = L; call. */
        cm_fn_0077bbe0((void *)local, 0x00000bbcu, 0u);
        /* 0x004dae2f..0x004dae33: ecx = L; state store is [esp+0xf0] = frame.state. */
        frame.state = 0u;
        /* 0x004dae3e..0x004dae43: value = [param_1]; index 0. */
        cm_fn_00785790((void *)local, 0u, *(u32 *)param_1);
        /* 0x004dae48..0x004dae55: value = [[param_1+4] + 0x33]; index 1. */
        cm_fn_00785790((void *)local, 1u,
                       *(u32 *)(*(u32 *)(param_1 + 4u) + 0x33u));
        /* 0x004dae5a..0x004dae67: value = [[param_1+8] + 0x33]; index 2. */
        cm_fn_00785790((void *)local, 2u,
                       *(u32 *)(*(u32 *)(param_1 + 8u) + 0x33u));
        /* 0x004dae6c..0x004dae79: value = [[param_1+0xc] + 0x33]; index 3. */
        cm_fn_00785790((void *)local, 3u,
                       *(u32 *)(*(u32 *)(param_1 + 0xcu) + 0x33u));
        /* 0x004dae7e..0x004dae87: value = [param_2]; index 4. */
        cm_fn_00785790((void *)local, 4u, *(u32 *)param_2);
        /* 0x004dae8c..0x004dae98: ecx = [0xb7431c]; push param_2; push L. */
        eax_result = (u32)cm_fn_007861e0((void *)CM_GLOBAL(u32, 0x00b7431cu),
                                         (u32)local, param_2);
    }

    /* 0x004dae9d..0x004daea6: pop esi/edi; mov ecx,[esp+0xe4] = frame.prev;
       mov fs:[0], ecx. EAX is not changed by these instructions. */
    CM_FS(void *, 0) = frame.prev;
    return eax_result;
}
