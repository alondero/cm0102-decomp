/* Preferred-image VA 008c5fc0/base 00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern void CM_FASTCALL cm_fn_0068d0d0(u32, u32, u32, u32);

/* THIS2/RET8/fullEAX count, all three callers test EAX. Append's ECX is
   the output list and EDX is the just-loaded matching payload ID. Both
   EDX:EAX return words are discarded. Keep cached vector base, late signed
   count reloads, iterator reset/reloads and increment after any append result. */
u32 CM_THISCALL cm_fn_008c5fc0(u32 self, volatile u32 person_argument,
                              volatile u32 output_argument)
{
    u32 matches = 0, input = person_argument, id, limit, receiver, record;
    u32 base, club, club_id, vector, index, entry, category, states, state;
    u32 list, head, payload, output;
    u8 kind;
    if (input != 0) {
        id = D(input);
        limit = D(0x00ae2364u);
        if ((s32)id >= (s32)limit) return 0;
    }
    receiver = D(0x00ae1050u);
    record = cm_fn_004cf5a0(receiver, input);
    D(0x00ae1080u) = record;
    if (record == 0) return 0;
    id = D(record + 4u);
    base = D(0x00ae23b4u);
    club = base + id * 581u;
    if (club == 0) return 0;
    club_id = D(club);
    limit = D(self + 0x10u);
    vector = D(self + 0xcu);
    index = 0;
    if ((s32)limit <= 0) return 0;
    do {
        entry = D(vector);
        if (entry == 0) goto next;
        if (D(entry + 8u) != club_id) goto next;
        category = D(entry + 0x20u);
        states = D(self);
        state = D(states + category * 4u);
        if (state == 0) goto next;
        kind = B(state + 0x2cu);
        if (kind == 2 || kind == 3 || kind == 20 || kind == 16 || kind == 4) goto next;
        if (kind == 15 && B(state + 0x2eu) >= 3) goto next;
        list = D(entry + 4u);
        if (list == 0) goto next;
        head = D(list + 2u);
        D(list + 0xau) = head;
        list = D(entry + 4u);
        payload = cm_fn_0068d1b0(list);
        while (payload != 0) {
            id = D(payload);
            input = person_argument;
            if (id == D(input)) {
                output = output_argument;
                if (output != 0)
                    cm_fn_0068d0d0(output, id, entry + 0x20u, 4);
                ++matches;
                break;
            }
            list = D(entry + 4u);
            payload = cm_fn_0068d1b0(list);
        }
next:
        ++index;
        vector += 4u;
        limit = D(self + 0x10u);
    } while ((s32)index < (s32)limit);
    return matches;
}
