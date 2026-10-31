#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_008e5710(u32);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_004ccb30(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cca70(u32, u32, u32);

u32 CM_THISCALL cm_fn_008d0f60(u32 self, u32 record)
{
    u32 index, table, child, value, list, node, other, id, base, person;
    u32 context, result, club;
    index = D(record + 0x28);
    if (index != 0xffffffffu) {
        table = D(self + 0xc);
        child = D(table + index * 4u);
        if (child != 0) {
            cm_fn_008e5710(child);
            cm_fn_00944c94(child);
        }
        index = D(record + 0x28);
        table = D(self + 0xc);
        D(table + index * 4u) = 0;
        value = D(self + 0x14) - 1u;
        D(self + 0x14) = value;
    }
    if (B(record + 0x1f) == 1) {
        list = D(self + 0x946);
        if (list != 0) {
            value = D(list + 2);
            D(list + 10) = value;
            list = D(self + 0x946);
            node = cm_fn_0068d1b0(list);
            while (node != 0) {
                value = D(node + 4);
                other = D(record + 8);
                if (value == other) {
                    value = D(node);
                    other = D(record + 0xc);
                    if (value == other) {
                        list = D(self + 0x946);
                        cm_fn_0068d1f0(list, node);
                        break;
                    }
                }
                list = D(self + 0x946);
                node = cm_fn_0068d1b0(list);
            }
        }
    }
    list = D(record + 4);
    if (list != 0) {
        value = D(list + 2);
        D(list + 10) = value;
        list = D(record + 4);
        node = cm_fn_0068d1b0(list);
        while (node != 0) {
            id = D(record + 0xc);
            base = D(0x00ae23bc);
            person = base + id * 110u;
            context = D(0x00ae1050);
            result = cm_fn_004cf5a0(context, person);
            D(0x00ae1080) = result;
            if (result != 0) {
                id = D(result + 4);
                base = D(0x00ae23b4);
                club = base + id * 581u;
                if (club != 0) {
                    id = D(node);
                    value = D(record + 0x20);
                    base = D(0x00ae23bc);
                    person = base + id * 110u;
                    context = D(0x00ae1050);
                    cm_fn_004ccb30(context, person, value);
                }
            }
            id = D(node);
            table = D(self + 0x86e);
            value = B(table + id * 8u + 2);
            B(table + id * 8u + 2) = (u8)(value - 1u);
            list = D(record + 4);
            node = cm_fn_0068d1b0(list);
        }
    }
    id = D(record + 8);
    base = D(0x00ae23b4);
    club = base + id * 581u;
    id = D(record + 0xc);
    base = D(0x00ae23bc);
    person = base + id * 110u;
    context = D(0x00ae1050);
    cm_fn_004cca70(context, person, club);
    id = D(record + 0xc);
    table = D(self + 0x86e);
    value = B(table + id * 8u + 1);
    B(table + id * 8u + 1) = (u8)(value - 1u);
    value = D(record + 0x20);
    if ((s32)value < (s32)D(self + 0x85e))
        D(self + 0x85e) = value;
    index = D(record + 0x20);
    table = D(self);
    D(table + index * 4u) = 0;
    if (record != 0) {
        cm_fn_008e5710(record);
        cm_fn_00944c94(record);
    }
    result = D(self + 8) - 1u;
    D(self + 8) = result;
    return result;
}
