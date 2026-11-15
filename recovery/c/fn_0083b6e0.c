/* Preferred-image VA 0083b6e0, base 00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, one stack word, RET4. Indexed caller 0083a76b forwards
   EAX through 0083a730; its caller 005cdd15 overwrites AL and, on the
   successors, EAX before use. Other indexed callers discard the result.
   Original callees remain boundaries. Three compiler-inserted __ftol calls
   are source conversions under ADR0006; their sites are marked below. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern u32 CM_THISCALL cm_fn_005a0590(u32, u32);
extern long long CM_THISCALL cm_fn_00598800(u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
    cm_fn_00945333(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008c61e0(void *, u32);
extern void CM_THISCALL cm_fn_0083f3a0(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_CDECL cm_fn_00544590(u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32, u32, u32);
extern long double CM_THISCALL cm_fn_00846920(u32, u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
    cm_fn_0090cfc0(u32, u32, u32);
extern s32 CM_THISCALL cm_fn_0085fa00(u32);
extern u32 CM_THISCALL cm_fn_008c5cf0(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32, u32, u32);

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define F64(a) CM_GLOBAL(volatile double, (a))

void CM_THISCALL cm_fn_0083b6e0(u32 self, volatile u32 club)
{
    /* Argument cached in ESI only for the initial helper calls. */
    u32 initial_club = club;
    u8 eligible = 0, small_allocations = 0, end = 8;
    u32 sum = 0, distributed = 0;
    u8 scratch[0x2c];
    if (cm_fn_00600a50(initial_club, 0, 1, 0, 0)) return;
    if (cm_fn_0053b900((u32 *)initial_club)) return;
    u32 receiver = D(0x00ae2a38u);
    u32 budget_object = cm_fn_005a0590(receiver, initial_club);
    if (!budget_object) return;
    unsigned long long returned = (unsigned long long)
        cm_fn_00598800(budget_object, 0, 0, 0, 1, 0);
    u32 budget = (u32)returned;
    if ((s32)budget < 0) budget = 0;
    u32 id = D(initial_club);
    u32 rows_base = D(self + 0xdcu);
    u32 slot = rows_base + id * 9u;
    if (B(slot + 4u) != 1) return;
    u32 array = D(slot);
    cm_fn_00945333(array, (u32)(returned >> 32), array, 40, 0x25, 0x0083b690u);
    array = D(slot);

    u8 row = 0, special_count = 0;
    do {
        u32 record = array + (u32)(s32)(s8)row * 37u;
        if (D(record + 0xfu) == 0xffffffffu) {
            if (B(record) == 11) {
                special_count = (u8)(special_count + 1u);
                if (special_count == 4) end = (u8)(row + 1u);
            }
            u32 amount = D(record + 0xbu);
            eligible = (u8)(eligible + 1u);
            sum += amount;
        }
        row = (u8)(row + 1u);
    } while ((s8)row < (s8)end);

    if ((s8)end > 0) {
        u32 offset = 0;
        u32 remaining = (u32)(s32)(s8)end;
        do {
            u32 base = D(slot);
            u32 record = base + offset;
            if (D(record + 0xfu) == 0xffffffffu) {
                u8 kind = B(record);
                u32 divisor = 0;
                u32 value = 0;
                u32 write = 0;
                u32 divide = 0;
                if (kind == 11) {
                    u32 variant = (B(record + 0x24u) >> 1) & 15u;
                    u32 table_index = variant - 2u;
                    if (table_index <= 3u) {
                        /* Guarded original DWORD target-table read. */
                        (void)D(0x0083bcd0u + table_index * 4u);
                        if (variant <= 3u) divisor = (u32)(s32)(s8)eligible * 2u;
                        else if (variant == 4u) divisor = (u32)(s32)(s8)eligible * 4u;
                        else divisor = (u32)(s32)(s8)eligible * 10u;
                        write = 1;
                        divide = 1;
                    }
                } else if (D(record + 0x13u) == 0xffffffffu) {
                    u32 table_index = (u32)(s32)(s8)kind - 6u;
                    if (table_index <= 4u) {
                        (void)D(0x0083bce0u + table_index * 4u);
                        u32 current_club = club;
                        if (B(current_club + 0x64u) == 1) {
                            u32 factor = kind == 7 ? 1000u : (kind == 6 || kind == 8 ? 10u : 20u);
                            divisor = (u32)(s32)(s8)eligible * factor;
                            divide = 1;
                        }
                        write = 1;
                    }
                }
                if (write) {
                    if (divide) value = (u32)((s32)budget / (s32)divisor);
                    D(record + 7u) = value;
                    distributed += value;
                }
            }
            offset += 37u;
            --remaining;
        } while (remaining);
    }
    budget -= distributed;

    if ((s8)end > 0) {
        u32 offset = 0;
        u32 remaining = (u32)(s32)(s8)end;
        do {
            u32 base = D(slot);
            u32 record = base + offset;
            u8 kind = B(record);
            if (kind != 11 && (s32)D(record + 0x13u) > -1) goto next_row;
            u32 person_id = D(record + 0xfu);
            if ((s32)person_id > -1 || kind == 7) goto next_row;
            u32 reference = D(record + 0x17u);
            if ((s32)reference > -1) {
                if (person_id == 0xffffffffu) D(record + 0x17u) = person_id;
                else if (kind == 11) {
                    receiver = D(0x00de1f54u);
                    u32 result = cm_fn_008c61e0((void *)receiver, reference);
                    u32 object = D(result);
                    if (object) budget -= D(object + 0x1bu);
                } else {
                    u32 people = D(0x00ae23bcu);
                    budget -= D(people + person_id * 110u + 0x52u);
                }
                goto next_row;
            }

            u32 current_club = club;
            if (B(current_club + 0x64u) != 1 && kind != 11) D(record + 7u) = 0;
            else {
                u32 denominator = (s32)distributed < 1000 ? 1000u : distributed;
                u32 old = D(record + 7u);
                long double t = (s32)old;
                t /= (s32)denominator;
                t *= (s32)budget;
                t *= F64(0x00969540u);
                u32 converted_0083b9f4 = (u32)(long long)t; /* __ftol VA0083b9f4 */
                D(record + 7u) = old - converted_0083b9f4;
            }
            base = D(slot);
            record = base + offset;
            u8 flags = B(record + 0x24u) & 0x1eu;
            if (flags > 6 || (s8)small_allocations < 3) {
                u32 denominator = (s32)sum < 1 ? 1u : sum;
                if (flags <= 6) small_allocations = (u8)(small_allocations + 1u);
                u32 amount = D(record + 0xbu);
                u32 multiplier = (s32)amount < 1 ? 1u : amount;
                long double t = (s32)budget;
                t /= (s32)denominator;
                t *= (s32)multiplier;
                if (flags > 6) t *= F64(0x00968968u);
                /* Keep the conversion before the following volatile operand
                   read of the original ADD [record+7],EAX (0083ba93). */
                volatile u32 converted_0083ba8e = (u32)(long long)t; /* __ftol VA0083ba8e */
                u32 old = D(record + 7u);
                D(record + 7u) = old + converted_0083ba8e;
            }
            base = D(slot);
            u8 old_flags = B(base + offset + 0x24u);
            B(base + offset + 0x24u) = old_flags & 0xfeu;
            base = D(slot);
            current_club = club;
            cm_fn_0083f3a0(self, base + offset, current_club, 0);
            base = D(slot);
            record = base + offset;
            person_id = D(record + 0xfu);
            if ((s32)person_id > -1 && !(B(record + 0x24u) & 1u)) {
                u32 people = D(0x00ae23bcu);
                long double t = (s32)D(people + person_id * 110u + 0x52u);
                t *= F64(0x0096f878u);
                u32 converted_0083bae9 = (u32)(long long)t; /* __ftol VA0083bae9 */
                budget += converted_0083bae9;
            }
            sum -= D(record + 0xbu);
next_row:
            offset += 37u;
            --remaining;
        } while (remaining);
    }

    row = end;
    while ((s8)row < 40 && (s32)budget > 0) {
        u32 offset = (u32)(s32)(s8)row * 37u;
        u32 base = D(slot);
        u32 record = base + offset;
        if (B(record) != 11) goto next_candidate;
        u32 person_id = D(record + 0xfu);
        if ((s32)person_id <= -1 || D(record + 0x17u) != 0xffffffffu) goto next_candidate;
        u32 people = D(0x00ae23bcu);
        u32 current_club = club;
        u32 person = people + person_id * 110u;
        if (D(person + 0x39u) != current_club) goto next_candidate;
        receiver = D(0x00ae1050u);
        u32 association = cm_fn_004cf5a0(receiver, person);
        D(0x00ae1080u) = association;
        u32 associated_club = 0;
        if (association) {
            u32 associated_id = D(association + 4u);
            u32 clubs = D(0x00ae23b4u);
            associated_club = clubs + associated_id * 581u;
        }
        current_club = club;
        if (associated_club == current_club) goto next_candidate;
        if (cm_fn_00544590(person, 0)) goto next_candidate;
        current_club = club;
        receiver = D(0x00ae1050u);
        u32 relation = cm_fn_004cf5f0(receiver, person, current_club);
        u32 category = relation ? (B(relation + 0x4fu) >> 4) : 4u;
        current_club = club;
        long double rating = cm_fn_00846920(self, person, current_club, 1, 0, category, 0);
        if (!(rating >= F64(0x00968e58u))) goto next_candidate;
        if (relation) {
            /* Positive fixed bounds: incoming EAX/EDX are not consumed by
               0090cfc0 on these paths (its zero-bound path alone reads EDX). */
            u32 random = (u32)cm_fn_0090cfc0(0, 0, 10);
            u32 threshold = 20u - random;
            s32 assessment = cm_fn_0085fa00(relation);
            if ((s32)(s16)assessment < (s32)threshold) goto select_candidate;
        }
        receiver = D(0x00de1f54u);
        if (cm_fn_008c5cf0(receiver, person, 0, 0)) goto select_candidate;
        if (relation) {
            u32 random = (u32)cm_fn_0090cfc0(0, 0, 100);
            u32 result = cm_fn_0054c8e0(0x00ae2c90u, (u32)scratch + 0x18u, random + 30u);
            u32 date = D(result);
            u32 extra = D(result + 4u);
            u16 year = W(relation + 0x27u);
            D((u32)scratch + 0x20u) = date;
            D((u32)scratch + 0x24u) = extra;
            u16 result_year = W((u32)scratch + 0x22u);
            if (result_year > year) goto select_candidate;
            if (result_year == year && (s16)date > (s16)W(relation + 0x25u))
                goto select_candidate;
        }
next_candidate:
        row = (u8)(row + 1u);
        continue;
select_candidate:
        base = D(slot);
        record = base + offset;
        u32 amount = D(record + 3u);
        D(record + 0xbu) = amount;
        base = D(slot);
        D(base + offset + 7u) = budget;
        return;
    }
}
