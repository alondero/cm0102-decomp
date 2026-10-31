#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944250(u32, u32, u32);
extern u32 CM_CDECL cm_fn_009443a0(u32, u32);
extern u32 CM_CDECL cm_fn_00940160(u32);
extern void CM_CDECL cm_fn_00940290(u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00940820(u32, u32, u32, u32, u32, u32, u32);

/* Preferred-image VA0093fd10/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL12/full pointer EAX. Entry PUSH ECX is unused scratch, not a receiver.
   WORD contour indices are signed. Coordinate additions/subtractions wrap
   before signed comparisons and arithmetic shifts. Keep the original two
   scans, bucket stores, and mixed line/curve traversal, including its bounds.
   All callees remain original-code boundaries. */
u32 CM_CDECL cm_fn_0093fd10(volatile u32 pool, volatile u32 count,
    volatile u32 starts, volatile u32 ends, volatile u32 x_input,
    volatile u32 y_input, volatile u32 tags_input, volatile u32 flag,
    volatile u32 mode, volatile u32 option, volatile u32 other,
    volatile u32 threshold)
{
    u32 saved_pool = pool;
    u32 object = cm_fn_00944250(saved_pool, 0x2226, 4);
    u32 saved_flag = (u8)flag;
    u32 limit = threshold;
    u32 p, q, x, y, tags, word_count, last, axis;
    u32 minimum, maximum, value, scan, remaining;
    u32 lower, upper, span, buckets, i;
    u32 contour, start, end, control, current, next;
    u32 px, py, cx, cy, nx, ny, mx, my;

    D(object + 0x2222) = saved_pool;
    B(object + 0x2200) = (u8)saved_flag;
    D(object + 0x21fc) = 4;
    if ((s32)limit > 4) D(object + 0x21fc) = limit;
    limit = D(object + 0x21fc);
    if ((s32)limit > 16) D(object + 0x21fc) = 16;
    B(object + 0x2201) = (u8)option;
    value = (u8)other;
    B(object + 0x2202) = (u8)value;
    p = starts;
    y = y_input;
    x = x_input;
    B(object + 0x2203) = value != 0 || saved_flag != 0;
    word_count = (u16)count;
    D(object + 0x220c) = p;
    q = ends;
    W(object + 0x2214) = (u16)word_count;
    D(object + 0x221a) = y;
    tags = tags_input;
    D(object + 0x2204) = 0;
    D(object + 0x2208) = 0;
    D(object + 0x2210) = q;
    D(object + 0x2216) = x;
    D(object + 0x221e) = tags;
    D(object + 0x20) = 0;
    last = (u32)(s32)(s16)W(q + (u32)(s32)(s16)word_count * 2u - 2u);
    p = x;
    for (axis = 0; axis != 2; ++axis) {
        maximum = D(p);
        minimum = maximum;
        remaining = last;
        if ((s32)remaining >= 1) {
            scan = p + 4u;
            do {
                value = D(scan);
                if ((s32)value > (s32)maximum) maximum = value;
                else if ((s32)value < (s32)minimum) minimum = value;
                scan += 4u;
                --remaining;
            } while (remaining != 0);
        }
        if (axis == 0) {
            p = y_input;
            D(object + 0x34) = minimum;
            D(object + 0x38) = maximum;
        } else {
            D(object + 0x3c) = minimum;
            D(object + 0x40) = maximum;
        }
    }
    upper = D(object + 0x38);
    lower = D(object + 0x34);
    D(object + 0x44) = 0;
    if ((s32)(upper - lower) >= 0xb504) D(object + 0x44) = 1;
    else {
        maximum = D(object + 0x40);
        minimum = D(object + 0x3c);
        if ((s32)(maximum - minimum) >= 0xb504) D(object + 0x44) = 1;
    }
    for (axis = 0; axis != 2; ++axis) {
        upper = (u32)((s32)(upper + 0x40u) >> 6);
        lower = (u32)((s32)(lower - 0x40u) >> 6);
        span = upper - lower + 1u;
        if (axis == 0 && B(object + 0x2203) == 0) buckets = 0;
        else {
            if ((s32)span <= 32) buckets = object + (axis == 0 ? 0xd8u : 0x48u);
            else buckets = cm_fn_009443a0(pool, span * 4u);
            if ((s32)span > 0) {
                for (i = 0; i != span; ++i) D(buckets + i * 4u) = 0;
            }
        }
        if (axis == 0) {
            p = buckets - lower * 4u;
            D(object + 0x15c) = buckets;
            D(object + 0x160) = lower;
            lower = D(object + 0x3c);
            D(object + 0x164) = upper;
            upper = D(object + 0x40);
            D(object + 0x158) = p;
        } else {
            D(object + 0xcc) = buckets;
            D(object + 0xd0) = lower;
            D(object + 0xc8) = buckets - lower * 4u;
            D(object + 0xd4) = upper;
        }
    }
    D(object + 0x21f4) = 0;
    cm_fn_00940160(object);
    value = (u8)mode;
    D(object + 0x1c) = 0;
    if (value == 3) return object;
    x = D(object + 0x2216);
    y = D(object + 0x221a);
    tags = D(object + 0x221e);
    for (contour = 0; ; ++contour) {
        word_count = (u32)(s32)(s16)W(object + 0x2214);
        if ((s32)contour >= (s32)word_count) return object;
        p = D(object + 0x220c);
        q = D(object + 0x2210);
        start = (u32)(s32)(s16)W(p + contour * 2u);
        end = (u32)(s32)(s16)W(q + contour * 2u);
        current = start;
        if (B(tags + start) != 0) {
            control = 0xffffffffu;
            cy = 0;
            cx = 0;
            px = D(x + start * 4u);
            py = D(y + start * 4u);
        } else {
            cx = D(x + start * 4u);
            cy = D(y + start * 4u);
            px = D(x + end * 4u);
            py = D(y + end * 4u);
            if (B(tags + end) == 0) {
                px = (u32)((s32)(px + cx + 1u) >> 1);
                py = (u32)((s32)(cy + py + 1u) >> 1);
            }
            control = start;
            current = end;
        }
dispatch:
        if ((s32)control < 0) goto line;
curve:
        next = control + 1u;
        if ((s32)next > (s32)end) next = start;
        nx = D(x + next * 4u);
        ny = D(y + next * 4u);
        if (B(tags + next) == 0) {
            mx = (u32)((s32)(nx + cx + 1u) >> 1);
            my = (u32)((s32)(ny + cy + 1u) >> 1);
            cm_fn_00940820(object, px, py, cx, cy, mx, my);
            if (next == start) continue;
            px = mx;
            py = my;
            cx = nx;
            cy = ny;
            current = control;
            control = next;
            if ((s32)next >= 0) goto curve;
        } else {
            cm_fn_00940820(object, px, py, cx, cy, nx, ny);
            if (next == start) continue;
            px = nx;
            py = ny;
            current = next;
        }
line:
        control = current + 1u;
        if ((s32)control > (s32)end) control = start;
        cx = D(x + control * 4u);
        cy = D(y + control * 4u);
        if (B(tags + control) != 0) {
            cm_fn_00940290(object, px, py, cx, cy);
            if (control == start) continue;
            current = control;
            px = cx;
            py = cy;
            goto line;
        }
        if (control == start) continue;
        goto dispatch;
    }
}
