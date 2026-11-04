/* CM0102 3.9.68, preferred-image VA 00935cb0, base 00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Static offline authorship only; original body 723 bytes. */
#include "cmre.h"
/* Full independent native/source review complete; canonical gate pending. */

extern u32 CM_CDECL cm_fn_00935f90(u32 state, u32 first_index,
                                  u32 second_index, u32 arg4, u32 arg5);

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

u32 CM_CDECL cm_fn_00935cb0(u32 state)
{
    /* One uninitialized 0x40-byte frame, offsets from L = entry ESP-0x40.
       After the four saves, native [ESP+n] is L+n-0x10. */
    struct {
        u32 other;        /* L+00 */
        u32 current;      /* L+04 */
        u32 distance;     /* L+08 */
        u32 best_a;       /* L+0c */
        u32 best_b;       /* L+10 */
        u32 equal;        /* L+14 */
        u32 count;        /* L+18 */
        u32 flags_base;   /* L+1c */
        u32 a_y;          /* L+20: state[44][current] */
        u32 a_x;          /* L+24: state[40][current] */
        u32 mask_28;      /* L+28 */
        u32 mask_50;      /* L+2c */
        u32 b_x;          /* L+30: state[4c][current] */
        u32 b_y;          /* L+34: state[48][current] */
        u32 inner;        /* L+38 */
        u32 outer;        /* L+3c */
    } scratch;
    u32 n, table, flags_base, count, scan, list, a_base, b_base;
    u32 current, flags_slot, other, bits, dx, dy, abs_x, abs_y;
    u32 value, cross, score, result, output, copy_base;
    s32 last;

    n = (u32)(s32)(s16)W(state + 4u);
    table = D(state + 0x0au);
    flags_base = D(state + 0x28u);
    scratch.flags_base = flags_base;
    last = (s32)(s16)W(table + n * 2u - 2u);
    scan = 0;
    count = 0;
    scratch.count = 0;
    if (last >= 0) {
        do {
            output = D(state + 0x34u);
            W(output + scan * 2u) = 0xffffu;
            output = D(state + 0x38u);
            W(output + scan * 2u) = 0xffffu;
            bits = W(flags_base + scan * 2u);
            if ((bits & 0x83u) && (bits & 0x7800u)) {
                table = D(state + 0x1cu);
                value = (u32)(s32)(s16)W(table + scan * 2u);
                if (value != scan) {
                    list = D(state + 0x24u);
                    ++count;
                    W(list + count * 2u - 2u) = (u16)scan;
                }
            }
            ++scan;
        } while ((s32)scan <= last);
        scratch.count = count;
    }
    scratch.outer = 0;
    if ((s32)count <= 0)
        return scan; /* EAX survives the no-candidates path. */

    value = 0;
    flags_slot = state;
    for (;;) {
        list = D(state + 0x24u);
        a_base = D(state + 0x40u);
        b_base = D(state + 0x4cu);
        current = (u32)(s32)(s16)W(list + value * 2u);
        scratch.current = current;
        bits = W(flags_base + current * 2u);
        scratch.b_x = D(b_base + current * 4u);
        /* 00935d47 writes only AX into the original argument slot. */
        flags_slot = (flags_slot & 0xffff0000u) | bits;
        value = D(a_base + current * 4u);
        a_base = D(state + 0x44u);
        scratch.a_x = value;
        scratch.a_y = D(a_base + current * 4u);
        b_base = D(state + 0x48u);
        scratch.b_y = D(b_base + current * 4u);
        scratch.equal = (scratch.b_y == scratch.a_x &&
                         scratch.b_x == scratch.a_y);
        scratch.inner = 0;
        scratch.best_b = 0x7fffu;
        scratch.best_a = 0x7fffu;
        value = 0;
        for (;;) {
            list = D(state + 0x24u);
            other = (u32)(s32)(s16)W(list + value * 2u);
            scratch.other = other;
            if (other != current) {
                flags_base = scratch.flags_base;
                bits = W(flags_base + other * 2u);
                scratch.mask_50 = bits & 0x5000u;
                scratch.mask_28 = bits & 0x2800u;
                if ((scratch.mask_28 && (flags_slot & 0x2800u)) ||
                    (scratch.mask_50 && (flags_slot & 0x5000u))) {
                    table = D(state + 0x2cu);
                    dx = (u32)(s32)(s16)W(table + other * 2u);
                    value = (u32)(s32)(s16)W(table + current * 2u);
                    table = D(state + 0x30u);
                    dx -= value;
                    dy = (u32)(s32)(s16)W(table + other * 2u);
                    value = (u32)(s32)(s16)W(table + current * 2u);
                    dy -= value;
                    abs_x = (s32)dx < 0 ? 0u - dx : dx;
                    abs_y = (s32)dy < 0 ? 0u - dy : dy;
                    if ((s32)abs_x > (s32)abs_y)
                        value = (abs_y >> 2) + (abs_x >> 1);
                    else
                        value = (abs_y >> 1) + (abs_x >> 2);
                    scratch.distance = value;

                    if ((s32)value < (s32)scratch.best_a &&
                        ((scratch.mask_28 && (flags_slot & 0x0800u)) ||
                         (scratch.mask_50 && (flags_slot & 0x1000u)))) {
                        cross = dy * scratch.a_x - dx * scratch.a_y;
                        cross = (u32)((s32)cross >> 14);
                        if ((s32)cross < 0)
                            cross = 0u - cross;
                        score = cross + value;
                        if ((s32)score < (s32)scratch.best_a) {
                            /* Native 00935e8b: five cdecl DWORD arguments. */
                            result = cm_fn_00935f90(state, current, scratch.other,
                                                    scratch.a_x, scratch.a_y);
                            if (result) {
                                output = D(state + 0x34u);
                                current = scratch.current;
                                other = scratch.other;
                                scratch.best_a = score;
                                W(output + current * 2u) = (u16)other;
                            }
                            current = scratch.current;
                            value = scratch.distance;
                        }
                        other = scratch.other;
                    }
                    if (!scratch.equal && (s32)value < (s32)scratch.best_b &&
                        ((scratch.mask_28 && (flags_slot & 0x2000u)) ||
                         (scratch.mask_50 && (flags_slot & 0x4000u)))) {
                        cross = dy * scratch.b_y - dx * scratch.b_x;
                        cross = (u32)((s32)cross >> 14);
                        if ((s32)cross < 0)
                            cross = 0u - cross;
                        score = cross + value;
                        if ((s32)score < (s32)scratch.best_b) {
                            /* Native 00935f11: reversed coefficient pair. */
                            result = cm_fn_00935f90(state, current, other,
                                                    scratch.b_y, scratch.b_x);
                            if (result) {
                                output = D(state + 0x38u);
                                current = scratch.current;
                                other = scratch.other;
                                scratch.best_b = score;
                                W(output + current * 2u) = (u16)other;
                            }
                            current = scratch.current;
                        }
                    }
                }
            }
            ++scratch.inner;
            value = (u32)(s32)(s16)scratch.inner;
            if ((s32)value >= (s32)scratch.count)
                break;
        }
        if (scratch.equal) {
            copy_base = D(state + 0x34u);
            output = D(state + 0x38u);
            bits = W(copy_base + current * 2u);
            W(output + current * 2u) = (u16)bits;
        }
        ++scratch.outer;
        value = (u32)(s32)(s16)scratch.outer;
        if ((s32)value >= (s32)scratch.count)
            return value;
        flags_base = scratch.flags_base; /* Native backward target 00935d29. */
    }
}

#undef W
#undef D
