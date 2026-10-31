#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007863f0(void *, u32, u32);
typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

void CM_STDCALL cm_fn_0059f300(u32 *param_1, u32 param_2, u32 param_3, s8 param_4)
{
    u8 local[224];
    volatile cm_seh_record seh;
    u8 mode;
    u32 object;

    seh.state = -1;
    seh.handler = (void *)0x0095907b;
    seh.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&seh;

    cm_fn_0077bbe0(local, 0x1779, 0);
    seh.state = 0;
    cm_fn_00785790(local, 0, *(volatile u32 *)param_1);
    cm_fn_00785790(local, 1, param_2);
    cm_fn_00785790(local, 2, param_3);
    mode = (u8)param_4;
    cm_fn_00785790(local, 3, (u32)(s32)param_4);

    if (*(volatile u8 *)((u8 *)param_1 + 0x64) != 3) {
        if (mode == 2 || mode == 7) {
            object = *(volatile u32 *)((u8 *)param_1 + 0x53);
            if (object != 0) {
                cm_fn_007863f0(CM_GLOBAL(void * volatile, 0xb7431c),
                               (u32)local, object);
            } else {
                cm_fn_007861e0(CM_GLOBAL(void * volatile, 0xb7431c),
                               (u32)local, (u32)param_1);
            }
        } else {
            cm_fn_007861e0(CM_GLOBAL(void * volatile, 0xb7431c),
                           (u32)local, (u32)param_1);
        }
    }
    CM_FS(void *, 0) = seh.prev;
}
