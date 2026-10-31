#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00786090(void *, u32, u32, u32, u32, u32, u32);

typedef struct {
    void *prev;
    void *handler;
    s32 state;
} cm_seh_record;

void CM_STDCALL cm_fn_006a44a0(u32 *param_1, s16 param_2, u32 param_3)
{
    u8 local[224];
    volatile cm_seh_record seh;

    seh.state = -1;
    seh.handler = (void *)0x0095ce2b;
    seh.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&seh;

    cm_fn_0077bbe0(local, 0x177e, 0);
    seh.state = 0;
    cm_fn_00785790(local, 0, *(volatile u32 *)param_1);
    cm_fn_00785790(local, 1, (u32)(s32)param_2);
    cm_fn_00785790(local, 2, param_3);
    cm_fn_00786090(CM_GLOBAL(void * volatile, 0xb7431c), (u32)local, (u32)param_1,
                   0, 0, 0, 0);

    CM_FS(void *, 0) = seh.prev;
}
