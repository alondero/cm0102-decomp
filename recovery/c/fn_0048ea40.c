/* Plain C recovery of the routine at preferred-image VA 0x0048ea40 (image base 0x00400000).
   Thiscall receiver in ECX (self); two stack arguments (param_2, param_3; ret 8).
   Returns EAX as the original leaves it.
   Frame: MSVC SEH registration at the top of the 0xec-byte frame, with a 224-byte
   local buffer directly below it (buffer = frame base, registration at +0xe0).
   Callees: 0x007875e0 (stdcall, one argument), 0x0077bbe0 (thiscall, ECX buffer),
   0x00785790 (thiscall, ECX buffer), 0x007861e0 (thiscall; inherits ECX from the
   global at 0x00b7431c through to its own callee, so ECX is set explicitly). */
#include "cmre.h"

extern u32 CM_STDCALL cm_fn_007875e0(int param_1);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);

/* One object: the 224-byte local buffer followed by the SEH registration in
   listing order (prev at +0xe0, handler at +0xe4, state at +0xe8). */
struct cm_frame_0048ea40 {
    u8 scratch[0xe0];
    void *prev;
    u32 handler;
    u32 state;
};

u32 CM_THISCALL cm_fn_0048ea40(int param_1, u32 *param_2, u32 param_3)
{
    volatile struct cm_frame_0048ea40 frame;
    u32 self = (u32)param_1;
    u32 buf;
    u32 eax;
    u32 p;
    u32 v;

    /* push -1; push handler; push fs:[0]; mov fs:[0], esp */
    frame.state = 0xffffffffu;
    frame.handler = 0x009560abu;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame.prev;

    buf = (u32)frame.scratch;

    /* mov ecx, [0xb7431c] precedes this call; the stdcall callee never reads ECX. */
    (void)CM_GLOBAL(volatile u32, 0x00b7431c);
    eax = cm_fn_007875e0((int)param_2);
    if (eax != 0) {
        cm_fn_0077bbe0((void *)buf, 0x1f, 0);

        eax = *(volatile u32 *)param_2;
        frame.state = 0;

        cm_fn_00785790((void *)buf, 0, eax);

        p = *(volatile u32 *)(self + 4);
        v = *(volatile u32 *)p;
        cm_fn_00785790((void *)buf, 1, v);

        eax = param_3;
        cm_fn_00785790((void *)buf, 2, eax);

        v = (u32)(s32)*(volatile s16 *)(self + 0x40);
        cm_fn_00785790((void *)buf, 3, v);

        p = CM_GLOBAL(volatile u32, 0x00b7431c);
        eax = (u32)cm_fn_007861e0((void *)p, buf, (u32)param_2);
    }

    CM_FS(void *, 0) = frame.prev;
    return eax;
}
