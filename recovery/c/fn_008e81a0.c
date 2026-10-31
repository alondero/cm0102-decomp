/* Preferred-image VA 008e81a0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 list);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32 list, u32 entry);
extern u32 CM_THISCALL cm_fn_0068d040(u32 list);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 allocation);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 person);
extern u32 CM_THISCALL cm_fn_004d0c60(u32 self, u32 person, u32 club, u32 zero, u32 minus_one, u32 last);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008e81a0(u32 self, volatile u32 argument)
{
    u8 work[0x200];
    u32 list, cursor, entry, record, left, right, manager, contract, index;
    u32 club, base, result, total, table, address, version;
    u8 count;
    list = D(self + 4u);
    if (list == 0) {
        cm_fn_00944cff(0x00a9e318u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x677u);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    cursor = D(list + 2u);
    D(list + 10u) = cursor;
    list = D(self + 4u);
    entry = cm_fn_0068d1b0(list);
    record = argument;
    while (entry != 0) {
        left = D(entry);
        right = D(record);
        if (left == right) {
            list = D(self + 4u);
            cm_fn_0068d1f0(list, entry);
        }
        list = D(self + 4u);
        entry = cm_fn_0068d1b0(list);
    }
    list = D(self + 4u);
    if (W(list) == 0) {
        if (list != 0) {
            cm_fn_0068d040(list);
            cm_fn_00944c94(list);
        }
        D(self + 4u) = 0;
    }
    manager = D(0x00ae1050u);
    contract = cm_fn_004cf5a0(manager, record);
    D(0x00ae1080u) = contract;
    if (contract != 0) {
        index = D(contract + 4u);
        base = D(0x00ae23b4u);
        club = base + index * 581u;
    } else {
        club = 0;
    }
    manager = D(0x00ae1050u);
    result = cm_fn_004d0c60(manager, record, club, 0, 0xffffffffu, 0);
    total = D(self + 0x1bu);
    D(self + 0x1bu) = total + result;
    manager = D(0x00de1f54u);
    index = D(record);
    table = D(manager + 0x86eu);
    address = table + index * 8u + 2u;
    count = B(address);
    B(address) = (u8)(count - 1u);
    return address;
}
