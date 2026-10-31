#include "cmre.h"

typedef u32 (CM_CDECL *record_predicate_fn)(u32 row, u32 record);
typedef void (CM_THISCALL *notify_fn)(u32 self, u32 record, u32 base);
extern void CM_THISCALL cm_fn_009315d0(u32 self, u8 *record, u32 base, record_predicate_fn predicate);

void CM_THISCALL cm_fn_004134c0(u32 self, u32 record, u32 base)
{
    u32 group = 0, table, entry, list;
    s32 index;
    if (*(volatile u8 *)(record + 0x3au) == 1) return;
    if (record != 0) {
        u32 p = *(volatile u32 *)(record + 0x14u);
        if (p != 0) {
            u32 q = *(volatile u32 *)(p + 0x5du);
            if (q != 0) group = q;
        }
    }
    table = *(volatile u32 *)(self + 8u);
    for (index = 0; index < 30; ++index, table += 4u) {
        entry = *(volatile u32 *)table;
        if (entry != 0 && *(volatile u32 *)(entry + 0xeu) == group) {
            u32 vtable;
            entry = *(volatile u32 *)table;
            vtable = *(volatile u32 *)entry;
            ((notify_fn)*(volatile u32 *)(vtable + 8u))(entry, record, base);
            break;
        }
    }
    list = *(volatile u32 *)(self + 0x10u);
    if (list != 0) cm_fn_009315d0(list, (u8 *)record, base, (record_predicate_fn)0x00414b20u);
    list = *(volatile u32 *)(self + 0xcu);
    if (list != 0) cm_fn_009315d0(list, (u8 *)record, base, (record_predicate_fn)0x00414b40u);
}
