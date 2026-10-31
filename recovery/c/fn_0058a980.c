#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, void *arg1, void *arg2);
struct cm_seh { void *prev; u32 handler; u32 state; };
void CM_CDECL cm_fn_0058a980(volatile u32 *arg1, volatile u32 *arg2, u32 arg3)
{
    u8 buffer[224];
    volatile struct cm_seh frame;
    volatile u32 *items;
    volatile u32 *p;
    u32 value;
    frame.state = 0xffffffffu;
    frame.handler = 0x00958cfbu;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;
    cm_fn_0077bbe0(buffer, 0x0fc2u, 0u);
    frame.state = 0;
    items = arg2;
    value = *items;
    cm_fn_00785790(buffer, 0, value);
    p = *(volatile u32 **)((volatile u8 *)items + 4);
    value = *(volatile u32 *)((volatile u8 *)p + 0x33);
    cm_fn_00785790(buffer, 1, value);
    p = *(volatile u32 **)((volatile u8 *)items + 8);
    value = *(volatile u32 *)((volatile u8 *)p + 0x33);
    cm_fn_00785790(buffer, 2, value);
    p = *(volatile u32 **)((volatile u8 *)items + 0x0c);
    value = *(volatile u32 *)((volatile u8 *)p + 0x33);
    cm_fn_00785790(buffer, 3, value);
    cm_fn_00785790(buffer, 4, arg3);
    value = *arg1;
    cm_fn_00785790(buffer, 5, value);
    cm_fn_007861e0(CM_GLOBAL(void * volatile, 0x00b7431c), buffer, (void *)arg1);
    CM_FS(void *, 0) = frame.prev;
}
