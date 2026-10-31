#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00685d30(u32 self, u32 record, u32 option);
extern u32 CM_THISCALL cm_fn_00438760(u32 self, u8 option);
typedef void (CM_THISCALL *dispatch_fn)(u32 self, u32 record, u32 option);

u32 CM_THISCALL cm_fn_004386e0(u32 self, u32 record, u32 option)
{
    u32 table, saved, entry, vtable;
    cm_fn_00685d30(self, record, option);
    if (*(volatile u8 *)(record + 0x42u) != 0xffu) return 1;
    *(volatile u8 *)(record + 0x42u) = *(volatile s16 *)(record + 0x34u) >= 11;
    while (*(volatile s32 *)(self + 0x30u) < *(volatile s8 *)(record + 0x42u)) {
        u8 next = (u8)(*(volatile u8 *)(self + 0x30u) + 1u);
        cm_fn_00438760(self, next);
    }
    table = *(volatile u32 *)(self + 0xcu);
    saved = *(volatile u32 *)(self + 8u);
    *(volatile u32 *)(self + 8u) = 0;
    entry = *(volatile u32 *)(table + (u32)(s32)*(volatile s8 *)(record + 0x42u) * 4u);
    vtable = *(volatile u32 *)entry;
    ((dispatch_fn)*(volatile u32 *)(vtable + 4u))(entry, record, option);
    *(volatile u32 *)(self + 8u) = saved;
    *(volatile u8 *)(record + 0x42u) = 0xffu;
    return 1;
}
