#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(
    void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(
    void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_00786090(
    void *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4,
    u32 arg5, u32 arg6);

void CM_STDCALL cm_fn_006a5d10(volatile u8 *object)
{
    struct seh_frame { u32 prev, handler, state; } volatile frame;
    volatile u8 local[232];
    u32 first;
    u32 second;
    u16 limit;
    u16 high;
    u32 node;
    u32 child;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095ceebu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)(unsigned long)&frame;

    first = CM_GLOBAL(volatile u32, 0x00dd6ec8u);
    second = CM_GLOBAL(volatile u32, 0x00dd6eccu);
    limit = CM_GLOBAL(volatile u16, 0x00ae2c92u);
    *(volatile u32 *)(local) = first;
    *(volatile u32 *)(local + 4) = second;
    high = *(volatile u16 *)(local + 2);
    if (high <= limit && (high < limit ||
            (s16)first <=
            (s16)CM_GLOBAL(volatile u16, 0x00ae2c90u))) {
        (void)cm_fn_0077bbe0((void *)(local + 8), 0x1782, 0);
        frame.state = 0;
        node = *(volatile u32 *)object;
        (void)cm_fn_00785790((void *)(local + 8), 0, node);
        node = *(volatile u32 *)(object + 0xcf);
        child = *(volatile u32 *)node;
        (void)cm_fn_00785790((void *)(local + 8), 1, child);
        node = *(volatile u32 *)(object + 0xcf);
        child = *(volatile u32 *)(node + 4);
        child = *(volatile u32 *)(child + 0x33);
        (void)cm_fn_00785790((void *)(local + 8), 2, child);
        node = *(volatile u32 *)(object + 0xcf);
        child = *(volatile u32 *)(node + 8);
        child = *(volatile u32 *)(child + 0x33);
        (void)cm_fn_00785790((void *)(local + 8), 3, child);
        node = *(volatile u32 *)(object + 0xcf);
        child = *(volatile u32 *)(node + 0xc);
        child = *(volatile u32 *)(child + 0x33);
        (void)cm_fn_00785790((void *)(local + 8), 4, child);
        (void)cm_fn_00786090((void *)CM_GLOBAL(volatile u32,
            0x00b7431cu), (u32)(unsigned long)(local + 8), (u32)(unsigned long)object,
            0, 0, 0, 0);
    }
    CM_FS(u32, 0) = frame.prev;
}
