/* Preferred-image VA006b5040, base00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Ordered availability-table refresh. Original helpers remain boundaries. */
#include "cmre.h"
#define R8(a) CM_GLOBAL(volatile u8, (u32)(a))
#define R32(a) CM_GLOBAL(volatile u32, (u32)(a))
#define LOW8(a, b) (((a) & 0xffffff00u) | (u8)(b))
extern u8 CM_THISCALL cm_fn_0048db80(volatile u8 *, volatile u32 *);
extern u32 CM_THISCALL cm_fn_005fe420(void *, u32);
extern u32 CM_THISCALL cm_fn_005fe8d0(u32);
/* Physical third stack word: 006b5156 replaces CL in the table-base DWORD.
   Original00600a50 reads its low byte, but this caller retains the full PUSH. */
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_006b61e0(u32);

u32 CM_THISCALL cm_fn_006b5040(u32 self)
{
    u32 receiver = R32(0x00b63c98u);
    cm_fn_005fe8d0(receiver);
    u32 limit = R32(self + 9u);
    R8(self + 0x29u) = 0;
    R8(self + 0x2au) = 0;
    R32(self + 0x2bu) = 0;
    if ((s32)limit <= 0)
        return 1u;
    u32 outer = 0, outer_offset = 0;
    do {
        u32 row = R32(self + 1u) + outer_offset;
        u32 end = R32(row + 8u);
        R32(row + 0x14u) = 0;
        u32 index = R32(row + 4u);
        if ((s32)index <= (s32)end) {
            u32 entry_offset = index * 84u;
            do {
                u32 entry = R32(self + 0x0du) + entry_offset;
                u32 id = R32(entry);
                R32(entry + 0x0cu) = 0;
                R32(entry + 0x10u) = 0;
                if ((s32)id > -1) {
                    u32 table = R32(0x00adadfcu);
                    u32 object = R32(table + id * 4u);
                    for (u32 person_index = 0; person_index < 16u; ++person_index) {
                        u32 record_index = R32(0x00ae2364u) + person_index - 16u;
                        u32 people = R32(0x00ae23bcu);
                        u32 person = people + record_index * 110u;
                        u32 value = R32(person + 0x39u);
                        u32 found = 0;
                        if (value != 0)
                            found = cm_fn_0048db80((volatile u8 *)object,
                                                  (volatile u32 *)value);
                        if (!found) {
                            value = R32(person + 0x24u);
                            if (value != 0)
                                found = cm_fn_0048db80((volatile u8 *)object,
                                                      (volatile u32 *)value);
                        }
                        if (found) {
                            R32(entry + 0x0cu) = 1u;
                            R32(self + 0x2bu) = 1u;
                        }
                    }
                }

                u32 item_index = R32(entry + 4u);
                u32 item_end = R32(entry + 8u);
                if ((s32)item_index <= (s32)item_end) {
                    u32 item_offset = item_index * 105u;
                    do {
                        u32 base = R32(self + 0x19u);
                        u32 item = base + item_offset;
                        u32 flag = LOW8(base, R8(item + 0x44u));
                        u32 club = R32(item + 0x20u);
                        R32(item + 0x53u) = 0;
                        u32 value = cm_fn_00600a50(club, 0, flag, 0, 0);
                        if (!value) {
                            club = R32(item + 0x24u);
                            flag = R8(item + 0x45u);
                            value = cm_fn_00600a50(club, 0, flag, 0, 0);
                        }
                        if (!value) {
                            receiver = R32(0x00b63c98u);
                            value = cm_fn_005fe420((void *)receiver, item + 4u);
                        }
                        if (value) {
                            R32(item + 0x53u) = 1u;
                            if (R32(row + 0x14u) == 0)
                                R8(self + 0x29u) = (u8)(R8(self + 0x29u) + 1u);
                            if (R32(entry + 0x10u) == 0)
                                R8(self + 0x2au) = (u8)(R8(self + 0x2au) + 1u);
                            R32(row + 0x14u) = 1u;
                            R32(entry + 0x10u) = 1u;
                            R32(entry + 0x0cu) = 1u;
                            R32(self + 0x2bu) = 1u;
                        } else if (cm_fn_006b61e0(item + 4u) != 0) {
                            R32(entry + 0x0cu) = 1u;
                            R32(self + 0x2bu) = 1u;
                        }
                        item_end = R32(entry + 8u);
                        item_index += 1u;
                        item_offset += 105u;
                    } while ((s32)item_index <= (s32)item_end);
                }
                end = R32(row + 8u);
                index += 1u;
                entry_offset += 84u;
            } while ((s32)index <= (s32)end);
        }
        limit = R32(self + 9u);
        outer += 1u;
        outer_offset += 24u;
    } while ((s32)outer < (s32)limit);
    return 1u;
}
