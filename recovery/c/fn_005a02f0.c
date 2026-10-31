#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, void *arg1, u32 arg2);

struct seh_registration { void *prev; u32 handler; u32 state; };

void CM_STDCALL cm_fn_005a02f0(s32 param_1)
{
    u8 buffer[224];
    volatile struct seh_registration frame;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095918bu;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;
    cm_fn_0077bbe0(buffer, 0x1783u, 0u);
    frame.state = 0u;
    cm_fn_00785790(buffer, 0u, (u32)param_1);
    cm_fn_007861e0(CM_GLOBAL(void *, 0x00b7431c), buffer,
                   (u32)CM_GLOBAL(s32, 0x00ae23b4) + (u32)param_1 * 0x245u);
    CM_FS(void *, 0) = frame.prev;
}
