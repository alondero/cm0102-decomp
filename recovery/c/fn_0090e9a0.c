/* Preferred-image VA 0090e9a0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve full incidental EAX on the early gap exit and AL-only loop update. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00896c60(u32 self, u32 club, u32 person, u32 first, u32 second);
extern volatile u8 *CM_THISCALL cm_fn_005335f0(volatile u8 *self, const volatile u8 *source);

u32 CM_STDCALL cm_fn_0090e9a0(u32 club, u32 person_id, u32 detail_id)
{
    u8 work[8];
    u32 slot, person_offset, detail_offset, result, id, total, receiver, people, source;
    u32 person, details, destination, value, i;
    u16 word;
    u8 iteration;
    slot = club + 0xd7u;
    B((u32)work + 3u) = 0;
    D((u32)work + 4u) = slot;
    person_offset = person_id * 110u;
    detail_offset = detail_id * 70u;
    do {
        result = D(slot);
        if (result != 0) {
            id = D(result);
            total = D(0x00ae2364u);
            if ((s32)id >= (s32)total) {
                if ((s32)(id - person_id) < 25)
                    return result;
                receiver = D(0x00de1d08u);
                cm_fn_00896c60(receiver, club, result, 1, 1);
                result = D(slot);
                receiver = D(0x00de1d08u);
                cm_fn_00896c60(receiver, club, result, 0, 1);
                people = D(0x00ae23bcu);
                source = D(slot);
                cm_fn_005335f0((volatile u8 *)(people + person_offset), (const volatile u8 *)source);
                people = D(0x00ae23bcu);
                D(people + person_offset) = person_id;
                people = D(0x00ae23bcu);
                person = people + person_offset;
                person_offset += 110;
                D(slot) = person;
                details = D(0x00ae23c4u);
                source = D(person + 0x61u);
                destination = details + detail_offset;
                for (i = 0; i < 17; ++i) {
                    value = D(source + i * 4u);
                    D(destination + i * 4u) = value;
                }
                word = W(source + 68u);
                W(destination + 68u) = word;
                details = D(0x00ae23c4u);
                ++person_id;
                D(details + detail_offset) = detail_id;
                details = D(0x00ae23c4u);
                slot = D((u32)work + 4u);
                people = D(0x00ae23bcu);
                result = detail_id + 1u;
                D(people + person_offset - 13u) = details + detail_offset;
                detail_id = result;
                detail_offset += 70;
            }
        }
        iteration = B((u32)work + 3u);
        slot += 4;
        iteration = (u8)(iteration + 1u);
        result = (result & 0xffffff00u) | iteration;
        D((u32)work + 4u) = slot;
        B((u32)work + 3u) = iteration;
    } while ((s8)iteration < 50);
    return result;
}
