/* Preferred-image VA 0062e700, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve full incidental EAX, including the last table-base reload. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0058e340(u32 self, u32 first, u32 second, u32 record);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern void CM_THISCALL cm_fn_00630ee0(u32 self, u32 person, u32 one, u8 type, u32 zero, u32 last);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0062e700(u32 self, u32 person, u32 flag)
{
    u8 work[0x200];
    u32 result, count, table, offset, row, record, receiver, club, version;
    u8 type;
    if (person == 0) {
        cm_fn_00944cff(0x009e7f28u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0xc87u);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    result = D(person);
    count = D(0x00ae2364u);
    if ((s32)result >= (s32)count)
        return result;
    table = D(self);
    offset = result * 31u;
    result = D(table + offset + 0xeu);
    row = table + offset;
    if (result == 0xffffffffu)
        return result;
    table = D(self + 4u);
    record = table + result * 48u;
    if (B(record) != 0x51) {
        receiver = D(self + 0xcu);
        cm_fn_0058e340(receiver, row + 0x14u, row + 0x18u, record);
    }
    if (flag != 0) {
        club = D(person + 0x39u);
        if (club != 0 && cm_fn_00600a50(club, 0, 1, 0, 0) != 0) {
            type = B(record);
            cm_fn_00630ee0(self, person, 1, type, 0, 0);
        }
    }
    B(record) = 0xff;
    offset = D(person) * 31u;
    result = D(self);
    D(result + offset + 0xeu) = 0xffffffffu;
    return result;
}
