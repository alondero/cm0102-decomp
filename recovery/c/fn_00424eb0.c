#include "cmre.h"

/* Callee at 0x004cf5a0: thiscall; receiver from global 0x00ae1050, one stack
   argument (the computed record pointer). */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 receiver, u32 arg);
/* Callee at 0x008e8800: thiscall with the object as ECX and four stack arguments. */
extern u32 CM_THISCALL cm_fn_008e8800(u32 self, u32 arg1, u32 arg2, u32 arg3, u32 arg4);

/* Virtual slot at object vtable + 0x10: thiscall, five stack arguments. The
   routine returns EAX from this call, so the slot's return value is forwarded. */
typedef u32 (CM_THISCALL *cm_slot10_fn)(u32 self, u32 arg1, u32 arg2, u32 arg3,
                                        u32 arg4, u32 arg5);

u32 CM_THISCALL cm_fn_00424eb0(u32 self, u32 param_2, u32 param_3, u32 param_4,
                               u32 param_5)
{
    u32 rec_c;
    u32 receiver;
    u32 base_bc;
    u32 item;
    u32 found;
    u32 base_b4;
    u32 scan;
    u32 field_8;
    u32 target;
    u32 field_4;
    u32 vtable;

    rec_c = *(volatile u32 *)(param_2 + 0x0cu);
    receiver = CM_GLOBAL(u32, 0x00ae1050u);
    base_bc = CM_GLOBAL(u32, 0x00ae23bcu);
    item = base_bc + rec_c * 0x6eu;
    found = cm_fn_004cf5a0(receiver, item);
    base_b4 = CM_GLOBAL(u32, 0x00ae23b4u);
    CM_GLOBAL(u32, 0x00ae1080u) = found;

    if (found == 0u || (*(volatile u8 *)(found + 0x35u) & 0x3fu) == 5u) {
        scan = 0u;
    } else {
        u32 index = *(volatile u32 *)(found + 4u);
        scan = base_b4 + index * 0x245u;
    }

    field_8 = *(volatile u32 *)(param_2 + 8u);
    target = base_b4 + field_8 * 0x245u;

    if (param_3 == target) {
        vtable = *(volatile u32 *)self;
        cm_slot10_fn slot = (cm_slot10_fn)(*(volatile u32 *)(vtable + 0x10u));
        return slot(self, target, item, scan, param_4, param_5);
    }

    field_4 = *(volatile u32 *)(param_2 + 4u);
    if (field_4 != 0u) {
        vtable = *(volatile u32 *)self;
        cm_slot10_fn slot = (cm_slot10_fn)(*(volatile u32 *)(vtable + 0x10u));
        return slot(self, scan, item, target, param_4, param_5);
    }

    return cm_fn_008e8800(self, param_2, param_3, param_4, param_5);
}
