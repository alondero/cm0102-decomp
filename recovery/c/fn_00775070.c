/* Preferred-image VA 00775070, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Record selection and transfer; original callees remain explicit boundaries. */
#include "cmre.h"
#define R32(a) CM_GLOBAL(volatile u32, (u32)(a))
extern u32 CM_CDECL cm_fn_0053b3d0(u32 nation);
extern u32 CM_THISCALL cm_fn_006a3770(u32 self, u32 record, u32 flag);

u32 CM_THISCALL cm_fn_00775070(u32 self, volatile u32 filter, volatile u32 mode)
{
    u32 limit = R32(0x00ae2350u);
    u32 index = 0, row_offset = 0, slot_offset = 0;
    if ((s32)limit <= 0)
        return limit;
    do {
        u32 rows = R32(0x00ae23a8u);
        u32 category = R32(rows + row_offset + 0x71u);
        if (category == filter) {
            u32 slots = R32(self + 4u);
            u32 slot = slots + slot_offset + 4u;
            u32 id = R32(slot);
            if (id != 0xffffffffu) {
                u32 person_offset = id * 107u;
                if (*(volatile u8 *)&mode == 0) {
                    u32 people = R32(0x00ae23d0u);
                    if (R32(people + person_offset + 0x59u) == 0)
                        R32(slot) = 0xffffffffu;
                } else {
                    u32 first = R32(0x009cfa10u);
                    u32 second = R32(0x009cfa10u);
                    u32 teams = R32(0x00ae23a4u);
                    u32 team = teams + (first + second * 32u) * 6u;
                    u32 people = R32(0x00ae23d0u);
                    if (R32(people + person_offset + 0x59u) == team)
                        R32(slot) = 0xffffffffu;
                }
            }

            slots = R32(self + 8u);
            id = R32(slots + index * 4u);
            slot = slots + index * 4u;
            if (id != 0xffffffffu) {
                u32 person_offset = id * 107u;
                if (*(volatile u8 *)&mode == 0) {
                    u32 people = R32(0x00ae23d0u);
                    if (R32(people + person_offset + 0x59u) == 0)
                        R32(slot) = 0xffffffffu;
                } else {
                    u32 first = R32(0x009cfa10u);
                    u32 second = R32(0x009cfa10u);
                    u32 teams = R32(0x00ae23a4u);
                    u32 team = teams + (first + second * 32u) * 6u;
                    u32 people = R32(0x00ae23d0u);
                    if (R32(people + person_offset + 0x59u) == team)
                        R32(slot) = 0xffffffffu;
                }
            }

            /* Both pointer fields are reloaded after potentially aliased stores. */
            slots = R32(self + 8u);
            id = R32(slots + index * 4u);
            if (id != 0xffffffffu) {
                slots = R32(self + 4u);
                slot = slots + slot_offset + 4u;
                if (R32(slot) == 0xffffffffu) {
                    R32(slot) = id;
                    slots = R32(self + 8u);
                    R32(slots + index * 4u) = 0xffffffffu;
                    if (R32(0x00b67a44u) != 0) {
                        rows = R32(0x00ae23a8u);
                        u32 record = cm_fn_0053b3d0(rows + row_offset);
                        u32 receiver = R32(0x00b67a44u);
                        cm_fn_006a3770(receiver, record, 1u);
                    }
                }
            }
        }
        limit = R32(0x00ae2350u);
        index += 1u;
        slot_offset += 0x6eu;
        row_offset += 0x122u;
    } while ((s32)index < (s32)limit);
    return limit;
}
