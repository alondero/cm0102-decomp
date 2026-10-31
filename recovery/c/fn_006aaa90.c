#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, void *arg1, void *arg2);
struct cm_seh { void *prev; u32 handler; u32 state; };
void CM_STDCALL cm_fn_006aaa90(volatile u32 *object, s8 arg2, s8 arg3, s8 arg4)
{
    u8 buffer[224];
    volatile struct cm_seh frame;
    volatile u32 value;
    volatile u32 *member;
    void *old_fs = CM_FS(void *, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095cffbu;
    frame.prev = old_fs;
    CM_FS(void *, 0) = (void *)&frame;
    if (object != (volatile u32 *)0) {
        member = (volatile u32 *)((volatile u8 *)object + 0x53);
        if (*member != 0) {
            cm_fn_0077bbe0(buffer, 0x1789u, 0u);
            value = *object;
            frame.state = 0;
            cm_fn_00785790(buffer, 0, value);
            cm_fn_00785790(buffer, 1, (u32)(s32)arg2);
            cm_fn_00785790(buffer, 2, (u32)(s32)arg3);
            value = *(volatile u32 *)*member;
            cm_fn_00785790(buffer, 3, value);
            cm_fn_00785790(buffer, 4, (u32)(s32)arg4);
            cm_fn_007861e0(CM_GLOBAL(void * volatile, 0x00b7431c), buffer, (void *)object);
        }
    }
    CM_FS(void *, 0) = frame.prev;
}
