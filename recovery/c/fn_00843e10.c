#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_FASTCALL cm_fn_0068d020(u32);
extern u32 CM_FASTCALL cm_fn_0068d040(u32);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_00842180(u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00946464(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0083f3a0(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32, u32, u32);

/* Preferred-image VA00843e10/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full incidental EAX: destructor EAX with AL set to 0 or 1.
   The wrapper at8bfca2 forwards full EAX. SEH962518/scratch5c: countL0,
   searchkeyL4,L8, date pairs Lc/L14,L1c/L24,L2c/L34,L3c/L44, listL4c.
   Original 844264 DWORD dispatch table and all five target reads retained.
   Date helpers initialize eight output bytes;417c10 copies into the record
   and the second temporary. Original list destructor/recursive return and
   repeated field reloads retained. Other helpers remain original boundaries. */
u32 CM_THISCALL cm_fn_00843e10(u32 self, volatile u32 input_person,
    volatile u32 input_club, volatile u32 input_kind, volatile u32 input_selector)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[0x5c / 4];
    u32 l = (u32)scratch, list = l + 0x4c;
    u32 club, person, kind, node, id, base, value, count, index, found, pass;
    u32 table_offset, count_offset, callback, header, relation, offset, selector;
    u32 record, target, date, date_output, copied_output, flag, low, receiver, eax;
    u32 success = 1;
    frame.state = 0xffffffffu;
    frame.handler = 0x00962518;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0068d020(list);
    club = input_club;
    person = input_person;
    frame.state = 0;
    D(l) = 0;
    if (club == 0 || club == D(person + 0x39)) {
        kind = (u8)input_kind;
        if (kind == 4) {
            cm_fn_00842180(self, person, list, 0, 3, 0);
            D(l + 0x56) = D(l + 0x4e);
            node = cm_fn_0068d1b0(list);
            while (node != 0) {
                id = D(node + 4);
                base = D(0x00ae23b4);
                cm_fn_00843e10(self, person, base + id * 0x245u, 1, 2);
                node = cm_fn_0068d1b0(list);
            }
            kind = (u8)input_kind;
        }
    } else kind = (u8)input_kind;
    if (club == 0) goto cleanup;
    if (kind == 5 || (kind != 4 && club != D(person + 0x39))) {
        base = D(self);
        if (base == 0) { success = 0; goto cleanup; }
        if (D(self + 4) == 0) { success = 0; goto cleanup; }
        id = D(person);
        value = D(club);
        D(l + 4) = id;
        count = D(self + 0x10);
        D(l + 8) = value;
        for (pass = 0; pass != 2; ++pass) {
            table_offset = pass * 4u;
            count_offset = 0x10u + pass * 4u;
            callback = pass == 0 ? 0x00757ea0 : 0x00844740;
            if (pass != 0) {
                count = D(self + 0x14);
                base = D(self + 4);
            }
            found = cm_fn_00946464(l + 4, base, count, 8, callback);
            if (found != 0) {
                base = D(self + table_offset);
                count = D(self + count_offset);
                index = (found - base) >> 3;
                while ((s32)index < (s32)(count - 1u)) {
                    base = D(self + table_offset);
                    ++index;
                    value = D(base + index * 8u);
                    D(base + index * 8u - 8u) = value;
                    value = D(base + index * 8u + 4u);
                    D(base + index * 8u - 4u) = value;
                    count = D(self + count_offset);
                }
                count = D(self + count_offset);
                base = D(self + table_offset);
                D(base + count * 8u - 4u) = 0;
                count = D(self + count_offset);
                base = D(self + table_offset);
                D(base + count * 8u - 8u) = 0;
                count = D(self + count_offset);
                D(self + count_offset) = count - 1u;
            } else D(l) = 1;
        }
        value = cm_fn_00600a50(club, 0, 1, 0, 0);
        if (value == 0 && D(l) == 0) {
            id = D(person);
            base = D(0x00ae2d0c);
            record = base + id * 0x4fu + 0xb;
            value = B(record);
            B(record) = (u8)((((value - 1u) ^ value) & 0xfu) ^ value);
        }
    }
    id = D(club);
    base = D(self + 0xdc);
    receiver = D(0x00ae1050);
    header = base + id * 9u;
    relation = cm_fn_004cf5f0(receiver, person, club);
    offset = 0;
    D(l) = 40;
    do {
        selector = (u8)input_selector;
        if (selector == 1) {
            base = D(header);
            if (B(base + offset) == 11) goto next_record;
        } else if (selector == 0) {
            base = D(header);
            if (B(base + offset) != 11) goto next_record;
        }
        base = D(header);
        value = D(base + offset + 0xf);
        record = base + offset;
        id = D(person);
        if (value != id) {
            if ((u8)input_kind != 4) goto next_record;
            if (D(record + 0x13) != id) goto next_record;
        }
        kind = (u32)(s32)(s8)input_kind - 1u;
        if (kind > 4) goto next_record;
        target = D(0x00844264 + kind * 4u);
        switch (target) {
        case 0x008440ca:
            D(record + 0xf) = 0xffffffffu;
            base = D(header);
            D(base + offset + 0x17) = 0xffffffffu;
            base = D(header);
            value = D(base + offset + 0x13);
            record = base + offset + 0x13;
            if (value == D(person)) D(record) = 0xffffffffu;
            base = D(header);
            flag = B(base + offset + 0x24);
            B(base + offset + 0x24) = (u8)(flag & 0xfeu);
            club = input_club;
            base = D(header);
            cm_fn_0083f3a0(self, base + offset, club, person);
            break;
        case 0x00844112:
        case 0x0084412a:
            date_output = l + (target == 0x00844112 ? 0xcu : 0x1cu);
            copied_output = l + (target == 0x00844112 ? 0x14u : 0x24u);
            date = cm_fn_0054c5b0(0x00ae2c90, date_output,
                                  target == 0x00844112 ? 120 : 14);
            base = D(header);
            cm_fn_00417c10(base + offset + 0x1b, copied_output, date);
            base = D(header);
            id = D(person);
            D(base + offset + 0x13) = id;
            base = D(header);
            D(base + offset + 0x17) = 0xffffffffu;
            base = D(header);
            D(base + offset + 0xb) = 0;
            base = D(header);
            D(base + offset + 7) = 0;
            if (relation != 0) {
                base = D(header);
                low = B(relation + 0x4f);
                flag = B(base + offset + 0x24);
                B(relation + 0x4f) = (u8)((low & 0xfu) | (((flag >> 1) & 0xfu) << 4));
            }
            B(header + 8) = 100;
            break;
        case 0x00844196:
            D(record + 0xf) = 0xffffffffu;
            base = D(header);
            D(base + offset + 0x17) = 0xffffffffu;
            date = cm_fn_0054c8e0(0x00ae2c90, l + 0x2c, 120);
            base = D(header);
            cm_fn_00417c10(base + offset + 0x1b, l + 0x34, date);
            base = D(header);
            value = D(base + offset + 0x13);
            record = base + offset + 0x13;
            if (value == D(person)) D(record) = 0xffffffffu;
            break;
        case 0x008441db:
            if (D(record + 0x13) == id) D(record + 0x13) = 0xffffffffu;
            base = D(header);
            id = D(person);
            value = D(base + offset + 0xf);
            record = base + offset + 0xf;
            if (value == id) {
                D(record) = 0xffffffffu;
                base = D(header);
                D(base + offset + 0x17) = 0xffffffffu;
                date = cm_fn_0054c8e0(0x00ae2c90, l + 0x3c, 120);
                base = D(header);
                cm_fn_00417c10(base + offset + 0x1b, l + 0x44, date);
            }
            break;
        default:
            /* The pinned, read-only five-entry original table selects one
               of the five target addresses above. */
            __builtin_unreachable();
        }
next_record:
        count = D(l);
        offset += 0x25u;
        --count;
        D(l) = count;
    } while (count != 0);
cleanup:
    frame.state = 0xffffffffu;
    eax = cm_fn_0068d040(list);
    eax = (eax & 0xffffff00u) | success;
    u32 predecessor = frame.prev;
    CM_FS(u32, 0) = predecessor;
    return eax;
}
