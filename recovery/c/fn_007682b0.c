#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0058e330(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
typedef u32 (CM_THISCALL *slot_fn)(u32, u32);

u32 CM_THISCALL cm_fn_007682b0(u32 self)
{
    s16 count = *(volatile s16 *)(self + 8);
    u32 i = 0, table, item, eax;
    *(volatile u32 *)self = 0x0096da10;
    if (count > 0) {
        do {
            table = *(volatile u32 *)(self + 4);
            item = *(volatile u32 *)(table + (u32)(s32)(s16)i * 4);
            if (item) {
                table = *(volatile u32 *)item;
                ((slot_fn)*(volatile u32 *)table)(item, 1);
            }
            ++i;
            count = *(volatile s16 *)(self + 8);
        } while ((s16)i < count);
    }
    eax = *(volatile u32 *)(self + 4);
    if (eax)
        eax = (u32)cm_fn_009452ca(eax);
    item = *(volatile u32 *)(self + 10);
    if (item) {
        cm_fn_0058e330(item);
        eax = (u32)cm_fn_00944c94((void *)item);
    }
    return eax;
}
