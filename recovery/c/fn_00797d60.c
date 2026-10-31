#include "cmre.h"

extern void CM_THISCALL cm_fn_00687970(u8 *self, u32 buffer);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);
extern u32 CM_THISCALL cm_fn_004a1c50(u32 *self, u32 value);
extern void CM_FASTCALL cm_fn_005face0(u32 *self);
typedef void (CM_THISCALL *slot_call)(u32 *self, u32 value);

void CM_THISCALL cm_fn_00797d60(u32 *self)
{
    *(volatile u32 *)((u8 *)self + 0x4c) = 0;
    cm_fn_00687970((u8 *)self, 0);

    u32 value = *(volatile u32 *)((u8 *)self + 0xbau);
    if (value != 0) {
        (void)cm_fn_009452ca(value);
        *(volatile u32 *)((u8 *)self + 0xbau) = 0;
    }

    u32 owned = *(volatile u32 *)((u8 *)self + 8u);
    if (owned != 0)
        (void)cm_fn_004a1c50((u32 *)owned, 1u);

    s32 limit = *(volatile s32 *)((u8 *)self + 0x30);
    u32 index = 0;
    if (limit >= 0) {
        do {
            u32 array = *(volatile u32 *)((u8 *)self + 0x0c);
            u32 object = *(volatile u32 *)(array + index * 4u);
            if (object != 0) {
                u32 vtable = *(volatile u32 *)object;
                u32 slot = *(volatile u32 *)vtable;
                ((slot_call)slot)((u32 *)object, 1u);
            }
            limit = *(volatile s32 *)((u8 *)self + 0x30);
            index++;
        } while ((s32)index <= limit);
    }

    u16 age = *(volatile u16 *)((u8 *)self + 0x40);
    *(volatile u16 *)((u8 *)self + 0x40) = (u16)(age + 2u);
    *(volatile u32 *)((u8 *)self + 0x30) = 0xffffffffu;
    *(volatile u32 *)((u8 *)self + 0x45) = 0;

    u32 reset_object = CM_GLOBAL(volatile u32, 0x00b5e134u);
    cm_fn_005face0((u32 *)reset_object);

    u32 final_vtable = *(volatile u32 *)self;
    u32 final_slot = *(volatile u32 *)(final_vtable + 0x8cu);
    ((void (CM_THISCALL *)(u32 *))final_slot)(self);
}
