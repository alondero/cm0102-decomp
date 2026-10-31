#include "cmre.h"

typedef void (CM_THISCALL *cm_fn_0057d0a0_slot0)(void *self, u32 arg1);

extern void CM_THISCALL cm_fn_00687970(void *self, u32 arg1);
extern void CM_THISCALL cm_fn_0068d040(void *self);
extern void CM_THISCALL cm_fn_00682300(void *self);
extern void CM_FASTCALL cm_fn_0049f450(void *self);
extern void *CM_CDECL cm_fn_00944c94(void *ptr);
extern void *CM_CDECL cm_fn_009452ca(void *ptr);

void CM_THISCALL cm_fn_0057d0a0(volatile u8 *self)
{
    volatile struct { u32 previous, handler, state; } frame;
    u32 index;

    frame.state = 0xffffffffu;
    frame.handler = 0x00958888u;
    frame.previous = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    *(volatile u32 *)self = 0x0096a0d4u;
    frame.state = 0u;
    cm_fn_00687970((void *)self, 0u);

    {
        u32 value = *(volatile u32 *)(self + 0xba);
        if (value != 0) {
            cm_fn_009452ca((void *)value);
            *(volatile u32 *)(self + 0xba) = 0;
        }
    }

    {
        s32 count = *(volatile s32 *)(self + 0x30);
        if (count >= 0) {
            index = 0;
            do {
                u32 array = *(volatile u32 *)(self + 0x0c);
                u32 element = *(volatile u32 *)(array + index * 4u);
                if (element != 0) {
                    u32 vtable = *(volatile u32 *)element;
                    u32 target = *(volatile u32 *)vtable;
                    ((cm_fn_0057d0a0_slot0)target)((void *)element, 1u);
                }
                count = *(volatile s32 *)(self + 0x30);
                ++index;
            } while ((s32)index <= count);
        }
    }

    {
        u32 value = *(volatile u32 *)(self + 0x0c);
        if (value != 0) {
            cm_fn_009452ca((void *)value);
            *(volatile u32 *)(self + 0x0c) = 0;
        }
    }
    {
        u32 value = *(volatile u32 *)(self + 0x10);
        if (value != 0) {
            cm_fn_0068d040((void *)value);
            cm_fn_00944c94((void *)value);
        }
    }
    {
        u32 value = *(volatile u32 *)(self + 8);
        if (value != 0) {
            cm_fn_0049f450((void *)value);
            cm_fn_00944c94((void *)value);
        }
    }
    {
        u32 value = *(volatile u32 *)(self + 0xee);
        if (value != 0) {
            cm_fn_009452ca((void *)value);
        }
    }

    frame.state = 0xffffffffu;
    cm_fn_00682300((void *)self);
    CM_FS(u32, 0) = frame.previous;
}
