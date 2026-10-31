#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);

typedef struct {
    void *prev;
    void *handler;
    s32 state;
} cm_seh_record;

void CM_STDCALL cm_fn_0059f900(u32 *param_1, s16 param_2, s8 param_3)
{
    u8 local[224];
    volatile cm_seh_record seh;
    u32 value;
    u32 *optional;

    seh.state = -1;
    seh.handler = (void *)0x009590cb;
    seh.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&seh;

    cm_fn_0077bbe0(local, 0x177a, 0);
    seh.state = 0;
    cm_fn_00785790(local, 0, *(volatile u32 *)param_1);
    optional = *(u32 *volatile *)((u8 *)param_1 + 0x69);
    value = optional ? *(volatile u32 *)optional : 0xffffffff;
    cm_fn_00785790(local, 1, value);
    cm_fn_00785790(local, 2, (u32)(s32)param_2);
    cm_fn_00785790(local, 3, (u32)(s32)param_3);
    cm_fn_007861e0(CM_GLOBAL(void * volatile, 0xb7431c), (u32)local,
                   (u32)param_1);

    CM_FS(void *, 0) = seh.prev;
}
