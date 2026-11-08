#include "cmre.h"

/* Preferred-image VA 00686a70, base 00400000, 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Calls bind to native boundaries. In particular 0090cfc0 takes ONE stack
   word here; its recovered C register-prefix parameters are not native ABI.
   The sole direct caller overwrites EAX at 00683708; the primary sections
   contain no literal pointer to this entry. Its result is void.
   No local x87 or SEH operations occur in the original routine. */
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *pointer);
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);
extern u32 CM_CDECL cm_fn_009469fa(u32 seed);
extern u32 CM_CDECL cm_fn_00946a04(void);
extern unsigned long long CM_STDCALL cm_fn_00945e80(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, s32);

#define D(address) CM_GLOBAL(volatile u32, (u32)(address))
#define W(address) CM_GLOBAL(volatile u16, (u32)(address))
#define B(address) CM_GLOBAL(volatile u8, (u32)(address))
#define COUNT() ((s32)(s16)W(receiver + 0x3eu))
/* REP MOVSD/MOVSW/MOVSB: forward, including overlapping records and faults. */
#define COPY59(destination, source) do { \
    u32 copy_dst = (destination), copy_src = (source), copy_i, copy_word; \
    for (copy_i = 0; copy_i != 14; ++copy_i) { \
        copy_word = D(copy_src); D(copy_dst) = copy_word; \
        copy_src += 4; copy_dst += 4; \
    } \
    copy_word = W(copy_src); W(copy_dst) = (u16)copy_word; \
    copy_word = B(copy_src + 2); B(copy_dst + 2) = (u8)copy_word; \
} while (0)
#define ERROR(first, second, line) do { \
    u32 error_version; \
    cm_fn_00944cff(0x009f15f4u, 0, 0, (void *)(first), (void *)(second)); \
    error_version = cm_fn_0090d130(); \
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_version, \
                  (first), (second), (u32)(line)); \
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0); \
    D(0x00b67a34u) = 0; \
} while (0)

void CM_THISCALL cm_fn_00686a70(u32 receiver, volatile s32 stack_count)
{
    /* L is immediately after SUB ESP,268h. The shuffle record is L+2Ch;
       reporting buffers are L+68h and L+168h. No scratch initialization. */
    u8 scratch[0x268];
    u32 shuffle = (u32)(scratch + 0x2c);
    u32 buffer_a = (u32)(scratch + 0x68);
    u32 buffer_b = (u32)(scratch + 0x168);
    s32 initial_count = COUNT();
    s32 half_count = initial_count / 2;
    u32 used = (u32)cm_fn_00944cf1((u32)initial_count * 4u);
    u32 filled = (u32)cm_fn_00944cf1((u32)COUNT() * 4u);
    u32 temporary = (u32)cm_fn_00944cf1((u32)COUNT() * 59u);
    u32 paired = 0, index, offset, pointer, base, source, destination;
    s32 count;

    if (!temporary || !used || !filled) {
        ERROR(buffer_a, buffer_b, 0xed3);
        cm_fn_00944c94((void *)filled);
        cm_fn_00944c94((void *)used);
        cm_fn_00944c94((void *)temporary);
        return;
    }
    index = 0;
    if (COUNT() > 0) {
        do {
            D(used + index * 4u) = 0;
            D(filled + index * 4u) = 0;
            count = COUNT();
            ++index;
        } while ((s32)index < count);
    }
    {
        u32 restore_seed = cm_fn_0090cfc0(30000u);
        u32 seed_base = D(0x00dd7120u);
        u32 season = (u32)(s32)(s16)W(receiver + 0x40u);
        ++restore_seed;
        cm_fn_009469fa(season + seed_base);
        index = 0;
        if (COUNT() > 0) {
            do {
                u32 random_value = cm_fn_00946a04();
                unsigned long long product;
                u32 first_index, second_index;
                count = COUNT();
                product = (unsigned long long)((long long)(s32)random_value * count);
                first_index = (u32)cm_fn_00945e80((u32)product, (u32)(product >> 32), 0x8000u, 0);
                random_value = cm_fn_00946a04();
                count = COUNT();
                product = (unsigned long long)((long long)(s32)random_value * count);
                second_index = (u32)cm_fn_00945e80((u32)product, (u32)(product >> 32), 0x8000u, 0);
                base = D(receiver + 0xb1u);
                destination = base + first_index * 59u;
                offset = second_index * 59u;
                COPY59(shuffle, destination);
                COPY59(destination, base + offset);
                base = D(receiver + 0xb1u);
                COPY59(base + offset, shuffle);
                count = COUNT();
                ++index;
            } while ((s32)index < count);
        }
        cm_fn_009469fa(restore_seed);
    }
    if (W(receiver + 0xd9u) & 0x100u) {
        index = 0;
        pointer = temporary;
        if (COUNT() > 0) {
            do {
                D(pointer) = 0;
                ++index;
                count = COUNT();
                pointer += 59u;
            } while ((s32)index < count);
        }
        index = offset = 0;
        if (COUNT() > 0) {
            do {
                u32 vtable, argument, target;
                s32 slot;
                base = D(receiver + 0xb1u);
                vtable = D(receiver);
                argument = D(base + offset);
                target = D(vtable + 0xa8u);
                slot = (s32)(s8)((u8 (CM_THISCALL *)(u32, u32))target)(receiver, argument);
                if (slot >= 0) {
                    count = COUNT();
                    if (slot < count) {
                        destination = temporary + (u32)slot * 59u;
                        if (!D(destination)) {
                            base = D(receiver + 0xb1u);
                            COPY59(destination, base + offset);
                            base = D(receiver + 0xb1u);
                            D(base + offset) = 0;
                        }
                    }
                }
                offset += 59u;
                count = COUNT();
                ++index;
            } while ((s32)index < count);
        }
        count = COUNT();
        index = offset = 0;
        if (count > 0) {
            do {
                base = D(receiver + 0xb1u);
                source = base + offset;
                if (D(source) && count > 0) {
                    u32 slot = 0;
                    pointer = temporary;
                    while (D(pointer)) {
                        ++slot;
                        pointer += 59u;
                        if ((s32)slot >= count) goto virtual_fill_next;
                    }
                    COPY59(temporary + slot * 59u, source);
                }
virtual_fill_next:
                offset += 59u;
                count = COUNT();
                ++index;
            } while ((s32)index < count);
        }
        index = 0;
        if (COUNT() > 0) {
            pointer = temporary;
            do {
                u32 argument = D(pointer);
                u32 vtable = D(receiver);
                u32 target = D(vtable + 0xacu);
                ((void (CM_THISCALL *)(u32, u32, u32))target)(receiver, argument, index);
                count = COUNT();
                ++index;
                pointer += 59u;
            } while ((s32)index < count);
        }
    } else {
        u32 competition = D(receiver + 4u);
        u32 special_competition = D(0x009cf5dcu);
        u32 competition_id = D(competition);
        if (competition_id == special_competition) {
            s16 scan_count = (s16)W(receiver + 0x3eu);
            u8 first = 0xff, second = 0xff, scan = 0;
            if (scan_count > 0) {
                base = D(receiver + 0xb1u);
                do {
                    u32 team = D(base + (u32)(s32)(s8)scan * 59u);
                    u32 first_id = D(0x009d0118u);
                    u32 team_id = D(team);
                    if (team_id == first_id) first = scan;
                    if (team_id == D(0x009d011cu)) second = scan;
                    scan = (u8)(scan + 1u);
                } while ((s16)(s8)scan < scan_count);
                if (second != 0xff && first != 0xff) {
                    u32 choice = cm_fn_0090cfc0(2);
                    u32 selected;
                    s32 half;
                    if (choice) {
                        base = D(receiver + 0xb1u);
                        half = stack_count;
                        COPY59(temporary, base + (u32)(s32)(s8)first * 59u);
                        half /= 2;
                        selected = (u32)(s32)(s8)second;
                    } else {
                        base = D(receiver + 0xb1u);
                        half = stack_count;
                        COPY59(temporary, base + (u32)(s32)(s8)second * 59u);
                        half /= 2;
                        selected = (u32)(s32)(s8)first;
                    }
                    base = D(receiver + 0xb1u);
                    paired = 1;
                    COPY59(temporary + (u32)half * 59u, base + selected * 59u);
                    D(filled + (u32)half * 4u) = 1;
                    D(filled) = 1;
                    D(used + (u32)(s32)(s8)first * 4u) = 1;
                    D(used + (u32)(s32)(s8)second * 4u) = 1;
                }
            }
        }
        /* Equality pass, then the related-group pass. Their missing-group
           branches differ: only the second abandons its inner search. */
        {
            u32 pass;
            for (pass = 0; pass != 2; ++pass) {
                u32 outer_used, next_filled, next_record, next_index;
                count = COUNT();
                if (count - 1 <= 0) continue;
                outer_used = used;
                offset = 0;
                next_index = 1;
                next_filled = filled + paired * 4u;
                next_record = temporary + paired * 59u;
                do {
                    if (!D(outer_used)) {
                        u32 inner = next_index;
                        if ((s32)inner < count) {
                            u32 inner_used = outer_used + 4u;
                            u32 inner_offset = offset + 59u;
                            do {
                                if (!D(inner_used)) {
                                    u32 a, b, matched = 0;
                                    base = D(receiver + 0xb1u);
                                    a = D(base + offset);
                                    a = D(a + 0x69u);
                                    if (!a) {
                                        if (pass) break;
                                    } else {
                                        b = D(base + inner_offset);
                                        b = D(b + 0x69u);
                                        if (!b) {
                                            if (pass) break;
                                        } else if (!pass) {
                                            matched = (a == b);
                                        } else {
                                            matched = D(a + 0x48u) == b;
                                            if (!matched) matched = D(b + 0x48u) == a;
                                        }
                                    }
                                    if (matched) {
                                        if ((s32)paired >= half_count) {
                                            if (!pass) { ERROR(buffer_b, buffer_a, 0xf76); }
                                            else { ERROR(buffer_a, buffer_b, 0xfa3); }
                                        } else {
                                            u32 output;
                                            s32 argument;
                                            base = D(receiver + 0xb1u);
                                            argument = stack_count;
                                            COPY59(next_record, base + offset);
                                            output = (u32)(argument / 2) + paired;
                                            base = D(receiver + 0xb1u);
                                            source = base + inner * 59u;
                                            ++paired;
                                            next_record += 59u;
                                            COPY59(temporary + output * 59u, source);
                                            D(filled + output * 4u) = 1;
                                            D(next_filled) = 1;
                                            next_filled += 4u;
                                            D(used + inner * 4u) = 1;
                                            D(outer_used) = 1;
                                        }
                                        break;
                                    }
                                }
                                ++inner;
                                inner_used += 4u;
                                inner_offset += 59u;
                            } while ((s32)inner < count);
                        }
                    }
                    index = next_index;
                    ++next_index;
                    offset += 59u;
                    outer_used += 4u;
                    count = COUNT();
                } while ((s32)index < count - 1);
            }
        }
        count = COUNT();
        index = offset = 0;
        pointer = used;
        if (count > 0) {
            do {
                if (!D(pointer) && count > 0) {
                    u32 slot = 0, scan_pointer = filled;
                    while (D(scan_pointer)) {
                        ++slot;
                        scan_pointer += 4u;
                        if ((s32)slot >= count) goto fill_next;
                    }
                    base = D(receiver + 0xb1u);
                    COPY59(temporary + slot * 59u, base + offset);
                    D(filled + slot * 4u) = 1;
                    D(pointer) = 1;
                }
fill_next:
                count = COUNT();
                ++index;
                offset += 59u;
                pointer += 4u;
            } while ((s32)index < count);
        }
    }
    index = offset = 0;
    if (COUNT() > 0) {
        do {
            base = D(receiver + 0xb1u);
            ++index;
            COPY59(base + offset, temporary + offset);
            count = COUNT();
            offset += 59u;
        } while ((s32)index < count);
    }
    cm_fn_00944c94((void *)filled);
    cm_fn_00944c94((void *)used);
    cm_fn_00944c94((void *)temporary);
}

#undef ERROR
#undef COPY59
#undef COUNT
#undef B
#undef W
#undef D
