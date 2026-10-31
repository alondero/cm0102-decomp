#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00569910(u32 self, u32 arg);
extern u32 CM_FASTCALL cm_fn_0068d020(u32 self);
extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern void CM_CDECL cm_fn_00944c94(u32 value);
extern u32 CM_CDECL cm_fn_00944cf1(u32 size);
extern void CM_CDECL cm_fn_009452ca(u32 value);
extern u32 CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);

u32 CM_THISCALL cm_fn_005697e0(volatile u32 *self, u32 input)
{
    volatile struct {
        u32 previous;
        u32 handler;
        s32 state;
    } frame;
    u32 entry_previous = CM_FS(volatile u32, 0);
    volatile u32 argument_slot;
    (void)&argument_slot;

    frame.state = -1;
    frame.handler = 0x00958096u;
    frame.previous = entry_previous;
    CM_FS(volatile u32, 0) = (u32)&frame;

    *(volatile u32 *)(self + 1) = 0;
    *self = 0;
    if (input != 0) {
        u32 valid = cm_fn_00569910((u32)self, input);
        if ((u8)valid != 0) {
            u32 restore_previous = frame.previous;
            CM_FS(volatile u32, 0) = restore_previous;
            return (u32)self;
        }

        {
            u32 old_aux = *(volatile u32 *)(self + 1);
            if (old_aux != 0) {
                cm_fn_009452ca(old_aux);
                *(volatile u32 *)(self + 1) = 0;
            }
        }
        {
            u32 old_primary = *self;
            if (old_primary != 0) {
                cm_fn_0068d040(old_primary);
                cm_fn_00944c94(old_primary);
                *self = 0;
            }
        }
    }

    {
        u32 base = CM_GLOBAL(volatile u32, 0x00ae2364u);
        u32 aux = cm_fn_00945501(base, 1u);
        *(volatile u32 *)(self + 1) = aux;
        u32 allocation = cm_fn_00944cf1(0x0eu);
        argument_slot = allocation;
        frame.state = input == 0 ? 1 : 0;
        if (allocation != 0) {
            u32 primary = cm_fn_0068d020(allocation);
            *self = primary;
        } else {
            *self = 0;
        }
    }

    {
        u32 restore_previous = frame.previous;
        u32 result = (u32)self;
        CM_FS(volatile u32, 0) = restore_previous;
        return result;
    }
}
