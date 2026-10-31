#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);

typedef struct {
    void *prev;
    void *handler;
    s32 state;
} cm_seh_record;

void CM_STDCALL cm_fn_0059ffb0(u32 *param_1, u32 param_2, s8 param_3)
{
    u8 local[224];
    volatile cm_seh_record seh;

    seh.state = -1;
    seh.handler = (void *)0x0095916b;
    seh.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&seh;

    cm_fn_0077bbe0(local, 0x177d, 0);
    seh.state = 0;
    cm_fn_00785790(local, 0, *param_1);
    cm_fn_00785790(local, 1, param_2);
    cm_fn_00785790(local, 2, (u32)(s32)param_3);
    cm_fn_007861e0(CM_GLOBAL(void *, 0xb7431c), (u32)local, (u32)param_1);

    CM_FS(void *, 0) = seh.prev;
}
