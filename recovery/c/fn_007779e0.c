#include "cmre.h"

/* Preferred-image VA 007779e0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch290: count BYTE L1, sort BYTEs L2/L3, current slot L4,
   outputs L8/Lc, group L10, sort bounds L14/L1c, saved list L18,
   message L20, index array L100. Sorting excludes the last array entry. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0068d080(u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u8, u32);
extern u8 CM_THISCALL cm_fn_005fa7d0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern unsigned long long CM_THISCALL cm_fn_00786640(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00786780(void *, u32);

u32 CM_THISCALL cm_fn_007779e0(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x290];
    u32 l = (u32)scratch, list = self + 0x28, eax, cursor, group, value;
    u32 base, pointer, target, first, lhs, rhs, message = l + 0x20;
    s32 count, inner, inner_bound, outer_bound;
    u16 word;
    u8 index, outer, next, position;
    s16 left, right;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095dfbb;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    word = W(self + 0x28);
    eax = (frame.prev & 0xffff0000u) | word;
    D(l + 0x18) = list;
    if (word == 0) goto done;
    cm_fn_0077bbe0((void *)message, 0x2c, 0);
    frame.state = 0;
    for (;;) {
        value = D(list + 2);
        group = 0;
        D(l + 0x10) = 0;
        D(list + 0xa) = value;
        cursor = cm_fn_0068d1b0(list);
        if (cursor == 0) break;
        do {
            if (D(cursor) != 0) {
                if (group == 0) {
                    group = D(cursor + 4);
                    value = D(group);
                    cm_fn_00785790((void *)message, 0, value);
                    index = 0;
                } else {
                    if (D(cursor + 4) != group) goto advance;
                    index = B(l + 1);
                }
                pointer = D(cursor);
                position = index;
                value = D(pointer);
                ++index;
                D(l + 0x100 + (u32)(s32)(s8)position * 4u) = value;
                B(l + 1) = index;
                D(cursor) = 0;
            }
advance:
            cursor = cm_fn_0068d1b0(list);
        } while (cursor != 0);
        D(l + 0x10) = group;
        if (group == 0) break;
        count = (s8)B(l + 1);
        cm_fn_00785790((void *)message, 1, (u32)count);
        value = cm_fn_0090cfc0(4);
        cm_fn_00785790((void *)message, 2, value);
        value = cm_fn_0090cfc0(2);
        cm_fn_00785790((void *)message, 3, value);
        D(l + 8) = 0;
        D(l + 0xc) = 0xffffffffu;
        value = D(group);
        base = D(0x00b5e134);
        cm_fn_005fa7d0(base, value, l + 8, l + 0xc);
        value = D(l + 8);
        cm_fn_00785790((void *)message, 4, value);
        value = D(l + 0xc);
        cm_fn_00785790((void *)message, 5, value);
        value = D(group);
        base = D(0x00adadfc);
        pointer = D(base + value * 4u);
        value = (u32)(s32)(s16)W(pointer + 0x40);
        cm_fn_00785790((void *)message, 6, value);
        outer_bound = count - 2;
        D(l + 0x1c) = (u32)outer_bound;
        outer = 0;
        if (outer_bound > 0) {
            inner_bound = count - 1;
            base = D(0x00ae23b4);
            D(l + 0x14) = (u32)inner_bound;
            do {
                next = (u8)(outer + 1);
                inner = (s8)next;
                B(l + 3) = next;
                B(l + 2) = next;
                if (inner < inner_bound) {
                    D(l + 4) = l + 0x100 + (u32)(s32)(s8)outer * 4u;
                    for (;;) {
                        pointer = D(l + 4);
                        target = l + 0x100 + (u32)inner * 4u;
                        lhs = D(pointer);
                        rhs = D(target);
                        left = (s16)W(base + lhs * 0x245u + 0x80);
                        right = (s16)W(base + rhs * 0x245u + 0x80);
                        if (left < right) {
                            pointer = D(l + 4);
                            D(pointer) = rhs;
                            D(target) = lhs;
                        }
                        position = B(l + 2);
                        inner_bound = (s32)D(l + 0x14);
                        ++position;
                        B(l + 2) = position;
                        inner = (s8)position;
                        if (inner >= inner_bound) break;
                        base = D(0x00ae23b4);
                    }
                    group = D(l + 0x10);
                    base = D(0x00ae23b4);
                    outer_bound = (s32)D(l + 0x1c);
                    next = B(l + 3);
                }
                outer = next;
            } while ((s32)(s8)outer < outer_bound);
            list = D(l + 0x18);
        }
        position = B(l + 1);
        index = 0;
        if ((s8)position > 0) {
            pointer = l + 0x100;
            do {
                value = D(pointer);
                position = (u8)(index + 7);
                cm_fn_00785790((void *)message, position, value);
                position = B(l + 1);
                ++index;
                pointer += 4;
            } while ((s8)index < (s8)position);
        }
        value = D(group);
        first = D(0x009cf7a4);
        if (value == first) {
            value = D(0x009cfa10);
            base = D(0x00ae23a4);
            target = base + value * 0xc6u;
            base = D(0x00b7431c);
            cm_fn_00786640((void *)base, message, target);
        } else {
            base = D(0x00b7431c);
            cm_fn_00786780((void *)base, message);
        }
    }
    eax = cm_fn_0068d080(list);
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
