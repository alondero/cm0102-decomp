#include "cmre.h"

/* Preferred-image VA007706C0/base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL6/RET24/full EAX: caller76BA85 changes AL, then returns at76BA9E.
   Original callees remain boundaries. Native785790 reads only index's BYTE.
   L is ESP immediately after SUB138; retain the complete adjacent scratch.
   The first different club terminates the candidate scan (770860->7709A5). */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern void CM_THISCALL cm_fn_0054a110(void *);
extern u32 CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern u32 CM_CDECL cm_fn_007e4750(u32, u32, void *);
extern u32 CM_CDECL cm_fn_007e4de0(void *, void *, void *);
extern u32 CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern s32 CM_CDECL cm_fn_0090cfc0(s32);
extern u32 CM_THISCALL cm_fn_007861e0(void *, u32, u32);

static void record_flags(u32 id, u32 side, volatile u32 *third,
                         u32 owner, u32 flags, u8 secondary, u32 slot)
{
    u32 i, base = 0, record = 0;
    D(flags) = 0;
    for (i = 0; i < 20; ++i) {
        base = *third;
        record = base + 0x190u + side * 1573u + i * 73u;
        if (D(record) == id) break;
    }
    if (i == 20) return;
    {
        u32 index = (u32)B(record + 0x21u) - 1u;
        if (index <= 7u) {
            u32 target = D((secondary ? 0x00770d00u : 0x00770ce0u) + index * 4u);
            switch (target) {
            case 0x007707cdu: case 0x007708fdu: D(flags) = 2; break;
            case 0x007707c3u: case 0x007708f5u: D(flags) = 8; break;
            case 0x007707b9u: case 0x007708edu: D(flags) = 0x20; break;
            case 0x007707afu: case 0x007708e5u: D(flags) = 0x40; break;
            }
        }
    }
    if (B(record + 0x34u) != 0xffu) D(flags) = D(flags) | 4u;
    if (secondary) {
        u32 person;
        base = *third;
        person = D(slot);
        base = D(base + 0x3cu);
        person = D(person + 0x61u);
        if (base == D(person)) D(flags) = D(flags) | 0x10u;
    } else {
        u32 person = D(owner + 0x61u);
        u32 other = D(base + 0x3cu);
        if (other == D(person)) D(flags) = D(flags) | 0x10u;
    }
    {
        u32 later = B(record + 0x24u);
        u32 earlier = B(record + 0x23u);
        if (earlier - later == 1u) D(flags) = D(flags) | 1u;
    }
}

static u32 next_index(u32 scratch)
{
    u8 index = B(scratch + 3u);
    B(scratch + 3u) = (u8)(index + 1u);
    return index;
}

u32 CM_STDCALL cm_fn_007706c0(volatile u32 first, volatile u32 second,
                              volatile u32 third, volatile u32 fourth,
                              volatile u32 fifth, volatile u32 sixth)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 scratch[0x138];
    u32 l = (u32)scratch, object = l + 0x58u;
    u32 array, eax, count;
    u8 bl;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095dea6u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0054a110((void *)(l + 0x38u));
    bl = (u8)sixth;
    array = fifth;
    count = (u32)(s32)(s8)bl;
    frame.state = 0;
    D(l + 0x24u) = 0;
    eax = cm_fn_00945333(array, count, 4, 0x00778ef0u);
    /* Qsort945333->945345 returns without touching EAX when count is 0/1.
       Root7706F5 supplied precisely the sign-extended count. No memory use of
       that incoming EAX precedes qsort's full overwrite on its other path. */
    if (count < 2u) eax = count;
    if ((s8)bl > 0) {
        B(l + 6u) = 1;
        D(l + 0x20u) = array;
        for (;;) {
            u32 primary = D(array);
            D(l + 0x18u) = primary;
            if (primary != 0 && D(primary + 0x39u) != 0) {
                u32 second_value = second;
                u32 first_value = first;
                u32 side_other = D(second_value + 0x1cu);
                u32 id = D(primary);
                u32 reference, index, value, offset;
                record_flags(id, first_value != side_other, &third,
                             primary, l + 8u, 0, 0);
                reference = fourth;
                D(l + 0x28u) = primary;
                if (primary == reference) D(l + 0x28u) = primary;
                {
                    u8 scan = B(l + 6u);
                    u8 end = (u8)sixth;
                    bl = 1;
                    B(l + 4u) = bl;
                    B(l + 5u) = scan;
                    if ((s8)scan < (s8)end) {
                        for (;;) {
                            u32 input = fifth;
                            u32 slot = input + (u32)(s32)(s8)scan * 4u;
                            u32 candidate = D(slot);
                            D(l + 0x1cu) = slot;
                            if (candidate != 0) {
                                u32 cached = D(l + 0x18u);
                                u32 club = D(candidate + 0x39u);
                                if (club != D(cached + 0x39u)) break;
                                if ((s8)bl < 4) {
                                    u32 flags;
                                    second_value = second;
                                    first_value = first;
                                    offset = (u32)(s32)(s8)bl * 4u;
                                    side_other = D(second_value + 0x1cu);
                                    id = D(candidate);
                                    flags = l + 8u + offset;
                                    record_flags(id, first_value != side_other, &third,
                                                 0, flags, 1, slot);
                                    slot = D(l + 0x1cu);
                                    value = D(slot);
                                    first_value = fourth;
                                    D(l + 0x28u + offset) = value;
                                    if (value == first_value) {
                                        u32 previous = D(l + 0x28u);
                                        u32 previous_flags;
                                        D(l + 0x28u + offset) = previous;
                                        previous_flags = D(l + 8u);
                                        D(l + 0x28u) = value;
                                        value = D(flags);
                                        D(flags) = previous_flags;
                                        D(l + 8u) = value;
                                    }
                                    B(l + 4u) = (u8)(B(l + 4u) + 1u);
                                    reference = fourth;
                                    scan = B(l + 5u);
                                    bl = B(l + 4u);
                                } else {
                                    ++bl;
                                    B(l + 4u) = bl;
                                }
                                D(slot) = 0;
                            }
                            end = (u8)sixth;
                            ++scan;
                            B(l + 5u) = scan;
                            if ((s8)scan >= (s8)end) break;
                        }
                        primary = D(l + 0x18u);
                    }
                }
                cm_fn_0077bbe0((void *)object, 0x24, 0);
                value = D(l + 0x24u);
                B((u32)&frame.state) = 1;
                if (value == 0) {
                    u32 a = first;
                    u32 b = second;
                    D(l + 0x24u) = 1;
                    cm_fn_007e4750(b, a, (void *)(l + 0x38u));
                }
                B(l + 3u) = 0;
                cm_fn_007e4de0((void *)(l + 0x38u), (void *)object, (void *)(l + 3u));
                {
                    u32 a = D(reference + 0x39u);
                    u32 b = D(primary + 0x39u);
                    index = B(l + 3u);
                    if (a == b) {
                        value = D(reference);
                        B(l + 3u) = (u8)(index + 1u);
                        cm_fn_00785790((void *)object, index, value);
                        index = next_index(l);
                        value = D(reference + 4u);
                        value = D(value + 0x33u);
                        cm_fn_00785790((void *)object, index, value);
                        index = next_index(l);
                        value = D(reference + 8u);
                        value = D(value + 0x33u);
                        cm_fn_00785790((void *)object, index, value);
                        index = next_index(l);
                        value = D(reference + 0xcu);
                        value = D(value + 0x33u);
                        cm_fn_00785790((void *)object, index, value);
                    } else {
                        B(l + 3u) = (u8)(index + 1u);
                        cm_fn_00785790((void *)object, index, 0xffffffffu);
                        index = next_index(l);
                        cm_fn_00785790((void *)object, index, 0xffffffffu);
                        index = next_index(l);
                        cm_fn_00785790((void *)object, index, 0xffffffffu);
                        index = next_index(l);
                        cm_fn_00785790((void *)object, index, 0xffffffffu);
                    }
                }
                index = B(l + 3u);
                count = (u32)(s32)(s8)bl;
                B(l + 3u) = (u8)(index + 1u);
                cm_fn_00785790((void *)object, index, count);
                index = next_index(l);
                value = D(primary + 0x39u);
                value = D(value);
                cm_fn_00785790((void *)object, index, value);
                {
                    s32 bound = (s32)(count - 1u);
                    u8 outer = 1;
                    B(l + 5u) = outer;
                    if (bound > 1) for (;;) {
                        u8 next, inner, end;
                        if ((s8)outer >= 3) break;
                        end = B(l + 4u);
                        next = (u8)(outer + 1u);
                        B(l + 7u) = next;
                        inner = next;
                        if ((s8)inner < (s8)end) for (;;) {
                            u32 a, b, av, bv;
                            if ((s8)inner >= 4) break;
                            a = (u32)(s32)(s8)outer * 4u;
                            b = (u32)(s32)(s8)inner * 4u;
                            av = D(l + 8u + a);
                            bv = D(l + 8u + b);
                            if ((s32)av < (s32)bv) {
                                D(l + 8u + a) = bv;
                                bv = D(l + 0x28u + b);
                                D(l + 8u + b) = av;
                                av = D(l + 0x28u + a);
                                D(l + 0x28u + a) = bv;
                                D(l + 0x28u + b) = av;
                            }
                            end = B(l + 4u);
                            next = B(l + 7u);
                            ++inner;
                            if ((s8)inner >= (s8)end) break;
                            outer = B(l + 5u);
                        }
                        B(l + 5u) = next;
                        if ((s32)(s8)next >= bound) break;
                        outer = B(l + 5u);
                    }
                }
                bl = 0;
                for (;;) {
                    s8 limit = (s8)B(l + 4u);
                    if (limit > 4) limit = 4;
                    index = B(l + 3u);
                    if ((s8)bl >= limit) break;
                    offset = (u32)(s32)(s8)bl;
                    reference = D(l + 0x28u + offset * 4u);
                    B(l + 3u) = (u8)(index + 1u);
                    value = D(reference);
                    cm_fn_00785790((void *)object, index, value);
                    index = next_index(l);
                    value = D(reference + 4u);
                    value = D(value + 0x33u);
                    cm_fn_00785790((void *)object, index, value);
                    index = next_index(l);
                    value = D(reference + 8u);
                    value = D(value + 0x33u);
                    cm_fn_00785790((void *)object, index, value);
                    index = next_index(l);
                    value = D(reference + 0xcu);
                    value = D(value + 0x33u);
                    cm_fn_00785790((void *)object, index, value);
                    ++bl;
                }
                B(l + 3u) = (u8)(index + 1u);
                cm_fn_00785790((void *)object, index, 0x420e9u);
                index = next_index(l);
                value = (u32)cm_fn_0090cfc0(2);
                cm_fn_00785790((void *)object, index, value);
                bl = 0;
                for (;;) {
                    s8 limit = (s8)B(l + 4u);
                    if (limit > 4) limit = 4;
                    if ((s8)bl >= limit) break;
                    index = next_index(l);
                    value = D(l + 8u + (u32)(s32)(s8)bl * 4u);
                    cm_fn_00785790((void *)object, index, value);
                    ++bl;
                }
                value = D(primary + 0x39u);
                reference = D(0x00b7431cu);
                eax = cm_fn_007861e0((void *)reference, object, value);
                bl = (u8)sixth;
                B((u32)&frame.state) = 0;
            }
            array = D(l + 0x20u);
            {
                u8 previous = B(l + 6u);
                array += 4u;
                B(l + 6u) = (u8)(previous + 1u);
                eax = (eax & 0xffffff00u) | previous;
                D(l + 0x20u) = array;
                if ((s8)previous >= (s8)bl) break;
            }
        }
    }
    CM_FS(u32, 0) = frame.prev;
    return eax;
}
