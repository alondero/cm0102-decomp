#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007862d0(void *, u32, u32);

typedef struct {
    void *prev;
    void *handler;
    s32 state;
} cm_seh_record;

void CM_THISCALL cm_fn_005f7330(void *self, u32 *param_1, s8 param_2)
{
    u8 local[224];
    volatile cm_seh_record seh;
    volatile u8 *self_bytes = (volatile u8 *)self;

    seh.state = -1;
    seh.handler = (void *)0x0095adbb;
    seh.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&seh;

    cm_fn_0077bbe0(local, 0x12, 0);
    seh.state = 0;
    cm_fn_00785790(local, 0, *(volatile u32 *)param_1);
    cm_fn_00785790(local, 1, (u32)(s32)param_2);
    {
        u32 object = *(volatile u32 *)(self_bytes + 4);
        cm_fn_00785790(local, 2, *(volatile u32 *)(u8 *)object);
    }
    cm_fn_007862d0(CM_GLOBAL(void *, 0xb7431c), (u32)local,
                   *(volatile u32 *)(self_bytes + 4));

    CM_FS(void *, 0) = seh.prev;
}
