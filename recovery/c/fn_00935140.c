/* CM0102 3.9.68, preferred-image VA 00935140, base 00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Packed record accesses and contour traversal follow the original instructions.
   The input graphs are arbitrary test inputs, not a constructor contract. */
#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define SW(a) ((u32)(s32)(s16)W(a))

/* The three reductions at 009357da, 009358fb and 009359fa.  Products and
   sums wrap before the signed division; the denominator is narrowed to WORD.
   In particular, the -32768 absolute-value anomaly is retained. */
static __attribute__((always_inline)) inline u32
curve(u32 dx, u32 dy, u32 px, u32 py)
{
    u32 cross = py * dx - px * dy;
    u32 x = dx + px, y = dy + py;
    s32 small, large, denominator;
    if ((s16)x < 0) x = 0u - x;
    if ((s16)y < 0) y = 0u - y;
    if ((s16)x > (s16)y) {
        small = (s16)y; large = (s16)x;
    } else {
        small = (s16)x; large = (s16)y;
    }
    denominator = (s16)((small >> 1) + large);
    return (u32)((s32)cross / (denominator + 1));
}

u32 CM_CDECL cm_fn_00935140(u32 object)
{
    u32 flags, output, table, count, last, threshold, i, start, end;
    u32 xbase, pred_base, output_reload, contour, coordinate;
    u32 offset, next_offset, next, previous, current, previous_value;
    u32 value, before, after, bits, distance, limit, remaining;
    u32 a, b, c, d, p, q, r;
    s32 old, following;
    struct {
        u32 successor, predecessor, pred_base, out;
        u32 value, end, visits, start, xptr, pred_delta, y_delta;
        u32 out_delta, y_value, dy, ybase, xbase, next_offset;
        u32 outer, saved_successor, saved_center, saved_predecessor;
        u32 saved_curve, saved_sign, initial, new_sign;
    } s;

    /* 0093514b..009351be: keep the unconditional terminal endpoint read. */
    flags = D(object + 0x0eu);
    output = D(object + 0x28u);
    value = SW(object + 0x29cu);
    threshold = (u32)((s32)value / 1000 + 1);
    table = D(object + 0x0au);
    count = SW(object + 4u);
    last = SW(table + count * 2u - 2u);
    value = last + 4u;
    if ((s32)value >= 0) {
        table = output + value * 2u;
        ++value;
        do { W(table) = 0; table -= 2u; } while (--value != 0);
    }
    pred_base = D(object + 0x1cu);
    output_reload = D(object + 0x28u);
    contour = 0;
    if ((s16)W(object + 4u) > 0) {
        do {
            a = D(object + 6u);
            b = D(object + 0x0au);
            start = SW(a + contour * 2u);
            end = SW(b + contour * 2u);
            if ((s32)end > (s32)start) {
                /* The two identical extrema walks differ only in array/bit.
                   Their field loads remain at their own original sites. */
                xbase = D(object + 0x12u);
                current = start;
                offset = current * 2u;
                next_offset = offset;
                remaining = 0x80u;
                for (;;) {
                    table = D(object + 0x20u);
                    old = (s16)W(xbase + offset);
                    next = SW(table + next_offset);
                    next_offset = next * 2u;
                    following = (s16)W(xbase + next_offset);
                    if (following != old || next == current) break;
                    value = remaining--;
                    if ((s32)value <= 0) break;
                }
                previous_value = W(xbase + next * 2u);
                coordinate = W(xbase + offset);
                do {
                    previous = current;
                    before = coordinate;
                    do {
                        current = SW(pred_base + offset);
                        offset = current * 2u;
                        coordinate = W(xbase + offset);
                    } while ((s16)before == (s16)coordinate && current != previous);
                    if (((s16)before > (s16)coordinate &&
                         (s16)before > (s16)previous_value) ||
                        ((s16)before < (s16)coordinate &&
                         (s16)before < (s16)previous_value))
                        B(output_reload + previous * 2u) |= 4u;
                    previous_value = before;
                } while ((s32)current > (s32)previous);

                a = D(object + 6u);
                xbase = D(object + 0x16u);
                current = SW(a + contour * 2u);
                offset = current * 2u;
                next_offset = offset;
                remaining = 0x80u;
                for (;;) {
                    table = D(object + 0x20u);
                    old = (s16)W(xbase + offset);
                    next = SW(table + next_offset);
                    next_offset = next * 2u;
                    following = (s16)W(xbase + next_offset);
                    if (following != old || next == current) break;
                    value = remaining--;
                    if ((s32)value <= 0) break;
                }
                previous_value = W(xbase + next * 2u);
                coordinate = W(xbase + offset);
                do {
                    previous = current;
                    before = coordinate;
                    do {
                        current = SW(pred_base + offset);
                        offset = current * 2u;
                        coordinate = W(xbase + offset);
                    } while ((s16)before == (s16)coordinate && current != previous);
                    if (((s16)before > (s16)coordinate &&
                         (s16)before > (s16)previous_value) ||
                        ((s16)before < (s16)coordinate &&
                         (s16)before < (s16)previous_value))
                        B(output_reload + previous * 2u) |= 8u;
                    previous_value = before;
                } while ((s32)current > (s32)previous);
            }
            count = SW(object + 4u);
            ++contour;
        } while ((s32)contour < (s32)count);
    }

    /* 00935354..00935457: relocate extrema onto adjacent flagged points. */
    i = last;
    if ((s32)i >= 0) do {
        if (B(flags + i) == 0) {
            table = D(object + 0x1cu);
            before = SW(table + i * 2u);
            table = D(object + 0x20u);
            after = SW(table + i * 2u);
            bits = W(output + i * 2u);
            if (bits & 4u) {
                table = D(object + 0x12u);
                a = SW(table + before * 2u);
                b = SW(table + i * 2u);
                distance = b - a;
                if ((s32)distance < 0) distance = 0u - distance;
                if (B(flags + before) != 0 && (s32)distance <= (s32)threshold) {
                    W(output + i * 2u) = (u16)(bits & 0xfffbu);
                    B(output + before * 2u) |= 4u;
                }
                table = D(object + 0x12u);
                a = SW(table + after * 2u);
                b = SW(table + i * 2u);
                distance = b - a;
                if ((s32)distance < 0) distance = 0u - distance;
                if (B(flags + after) != 0 && (s32)distance <= (s32)threshold) {
                    W(output + i * 2u) &= 0xfffbu;
                    B(output + after * 2u) |= 4u;
                }
            }
            bits = W(output + i * 2u);
            if (bits & 8u) {
                table = D(object + 0x16u);
                a = SW(table + before * 2u);
                b = SW(table + i * 2u);
                distance = b - a;
                if ((s32)distance < 0) distance = 0u - distance;
                if (B(flags + before) != 0 && (s32)distance <= (s32)threshold) {
                    W(output + i * 2u) = (u16)(bits & 0xfff7u);
                    B(output + before * 2u) |= 8u;
                }
                table = D(object + 0x16u);
                a = SW(table + after * 2u);
                b = SW(table + i * 2u);
                distance = b - a;
                if ((s32)distance < 0) distance = 0u - distance;
                if (B(flags + after) != 0 && (s32)distance <= (s32)threshold) {
                    W(output + i * 2u) &= 0xfff7u;
                    B(output + after * 2u) |= 8u;
                }
            }
        }
    } while ((s32)--i >= 0);

    /* 00935465..0093550f: ordered products, mask and component thresholds. */
    i = last;
    if ((s32)i >= 0) {
        offset = output + i * 2u;
        do {
            a = D(object + 0x4cu);
            b = D(object + 0x44u);
            p = D(object + 0x40u);
            a = D(a + i * 4u);
            a *= D(b + i * 4u);
            b = D(object + 0x48u);
            b = D(b + i * 4u);
            b *= D(p + i * 4u);
            a = (a + b) & 0xffffc000u;
            if ((s32)a <= 0x0ff08000) B(offset + 1u) |= 1u;
            a = D(object + 0x40u);
            a = D(a + i * 4u);
            if ((s32)a > 0x3fc2 || (s32)a < -0x3fc2)
                B(offset + 1u) |= 8u;
            else {
                a = D(object + 0x44u);
                a = D(a + i * 4u);
                if ((s32)a > 0x3fc2 || (s32)a < -0x3fc2)
                    B(offset + 1u) |= 0x10u;
            }
            a = D(object + 0x48u);
            a = D(a + i * 4u);
            if ((s32)a > 0x3fc2 || (s32)a < -0x3fc2)
                B(offset + 1u) |= 0x20u;
            else {
                a = D(object + 0x4cu);
                a = D(a + i * 4u);
                if ((s32)a > 0x3fc2 || (s32)a < -0x3fc2)
                    B(offset + 1u) |= 0x40u;
            }
            --i; offset -= 2u;
        } while ((s32)i >= 0);
    }

    /* 00935513..009356c9: strict neighbor-distance tests. */
    limit = (u32)(((s32)SW(object + 0x29cu) >> 7) + 1);
    i = last;
    if ((s32)i >= 0) do {
        bits = W(output + i * 2u);
        if (!(bits & 0x100u)) {
            a = D(object + 0x1cu);
            b = D(object + 0x20u);
            before = SW(a + i * 2u);
            after = SW(b + i * 2u);
            if (B(flags + i) != 0) {
                if ((bits & 4u) && B(flags + before) == 0) {
                    table = D(object + 0x12u);
                    a = SW(table + i * 2u);
                    b = SW(table + before * 2u);
                    distance = b - a;
                    if ((s32)distance < 0) distance = 0u - distance;
                    if ((s32)distance < (s32)limit && B(flags + after) == 0) {
                        b = SW(table + after * 2u);
                        distance = b - a;
                        if ((s32)distance < 0) distance = 0u - distance;
                        if ((s32)distance < (s32)limit)
                            W(output + i * 2u) = (u16)(bits | 0x200u);
                    }
                }
                bits = W(output + i * 2u);
                if ((bits & 8u) && B(flags + before) == 0) {
                    table = D(object + 0x16u);
                    a = SW(table + i * 2u);
                    b = SW(table + before * 2u);
                    distance = b - a;
                    if ((s32)distance < 0) distance = 0u - distance;
                    if ((s32)distance < (s32)limit && B(flags + after) == 0) {
                        b = SW(table + after * 2u);
                        distance = b - a;
                        if ((s32)distance < 0) distance = 0u - distance;
                        if ((s32)distance < (s32)limit)
                            W(output + i * 2u) = (u16)(bits | 0x400u);
                    }
                }
            } else {
                if (bits & 4u) {
                    if (B(flags + before) == 0) {
                        table = D(object + 0x12u);
                        a = SW(table + before * 2u);
                        b = SW(table + i * 2u);
                        distance = a - b;
                        if ((s32)distance < 0) distance = 0u - distance;
                        if ((s32)distance < (s32)limit) goto mark_x;
                    }
                    if (B(flags + after) == 0) {
                        table = D(object + 0x12u);
                        a = SW(table + after * 2u);
                        b = SW(table + i * 2u);
                        distance = a - b;
                        if ((s32)distance < 0) distance = 0u - distance;
                        if ((s32)distance < (s32)limit) {
mark_x:                     W(output + i * 2u) = (u16)(bits | 0x200u);
                        }
                    }
                }
                bits = W(output + i * 2u);
                if (bits & 8u) {
                    if (B(flags + before) == 0) {
                        table = D(object + 0x16u);
                        a = SW(table + before * 2u);
                        b = SW(table + i * 2u);
                        distance = a - b;
                        if ((s32)distance < 0) distance = 0u - distance;
                        if ((s32)distance < (s32)limit) goto mark_y;
                    }
                    if (B(flags + after) == 0) {
                        table = D(object + 0x16u);
                        a = SW(table + after * 2u);
                        b = SW(table + i * 2u);
                        distance = a - b;
                        if ((s32)distance < 0) distance = 0u - distance;
                        if ((s32)distance < (s32)limit) {
mark_y:                     W(output + i * 2u) = (u16)(bits | 0x400u);
                        }
                    }
                }
            }
        }
    } while ((s32)--i >= 0);

    /* 009356c9..00935b7d.  The scans cache different bases, so retain their
       distinct ordered loads rather than merging them into a point accessor. */
    s.out = D(object + 0x28u);
    s.pred_base = D(object + 0x1cu);
    value = SW(object + 0x29cu);
    limit = (u32)((s32)value / 256);
    s.visits = 0;
    if (limit == 0) limit = 1;
    s.outer = 0;
    if ((s16)W(object + 4u) > 0) {
contour_begin:
        a = D(object + 6u);
        b = D(object + 0x0au);
        c = SW(a + s.outer * 2u);
        d = SW(b + s.outer * 2u);
        s.end = d;
        s.initial = c;
        if ((s32)d <= (s32)(c + 1u)) goto contour_done;
        a = D(object + 0x20u);
        s.value = 0;
        a = SW(a + c * 2u);
        s.successor = a;
        if ((s32)c > (s32)d) goto scan_all_begin;
        d = D(object + 0x12u);
        b = s.pred_base;
        q = a * 2u;
        a = 0u - d;
        b += a;
        p = d + c * 2u;
        s.pred_delta = b;
        b = D(object + 0x16u);
        b -= d;
        d = s.out;
        a += d;
        s.xptr = p;
        s.y_delta = b;
        s.out_delta = a;
scan_clear:
        a = s.pred_delta;
        d = SW(a + p);
        a = SW(p);
        s.predecessor = d;
        d = D(object + 0x12u);
        b = SW(d + q);
        d = a - b;
        b = s.y_delta;
        p = SW(b + p);
        b = D(object + 0x16u);
        s.y_value = p;
        b = SW(b + q);
        q = p - b;
        p = s.predecessor;
        b = D(object + 0x12u);
        b = SW(b + p * 2u);
        b -= a;
        a = D(object + 0x16u);
        a = SW(a + p * 2u);
        p = s.y_value;
        a -= p;
        p = s.xptr;
        s.dy = a;
        a = s.out_delta;
        if (B(a + p + 1u) & 1u) a = s.value;
        else {
            a = curve(d, q, b, s.dy);
            s.value = a;
            if ((s32)a < (s32)(0u - limit) || (s32)a > (s32)limit)
                goto scan_clear_done;
        }
        /* 00935836: cached predecessor of the next linear point. */
        p = s.xptr;
        d = s.end;
        s.successor = c;
        q = c * 2u;
        ++c;
        p += 2u;
        s.xptr = p;
        if ((s32)c <= (s32)d) goto scan_clear;
        goto scan_clear_exit;
scan_clear_done:
        d = s.end;
scan_clear_exit:
        if (a != 0) goto walk_begin;
        c = s.initial;
scan_all_begin:
        a = D(object + 0x20u);
        a = SW(a + c * 2u);
        s.successor = a;
        if ((s32)c > (s32)d) goto contour_done;
        q = D(object + 0x12u);
        b = D(object + 0x16u);
        d = s.pred_base;
        s.ybase = b;
        r = a * 2u;
        d -= q;
        a = q + c * 2u;
        b -= q;
        s.xbase = q;
        s.next_offset = r;
        s.xptr = a;
        s.pred_delta = d;
        s.y_delta = b;
        goto scan_all;
scan_all_reload:
        d = s.pred_delta;
        q = s.xbase;
scan_all:
        p = SW(a);
        r = SW(r + q);
        b = SW(a + d);
        d = p;
        s.predecessor = b;
        d -= r;
        r = s.y_delta;
        q = SW(q + b * 2u);
        a = SW(a + r);
        r = s.ybase;
        s.dy = a;
        a = s.next_offset;
        q -= p;
        p = s.dy;
        a = SW(a + r);
        r = s.dy - a;
        a = s.ybase;
        b = SW(a + b * 2u);
        b -= p;
        a = curve(d, r, q, b);
        if (a != 0) { d = s.end; goto walk_begin; }
        a = s.xptr;
        d = s.end;
        s.successor = c;
        r = c * 2u;
        ++c;
        a += 2u;
        s.next_offset = r;
        s.xptr = a;
        if ((s32)c <= (s32)d) goto scan_all_reload;
        goto contour_done;

walk_begin:
        if ((s32)c > (s32)d) goto contour_done;
        s.saved_center = c;
        s.saved_successor = s.successor;
        s.saved_predecessor = s.predecessor;
        s.saved_curve = a;
        s.start = c;
        s.saved_sign = (s32)a < 0 ? 0xffffffffu : 1u;
walk:
        a = D(object + 0x20u);
        d = s.pred_base;
        q = SW(a + c * 2u);
        a = D(object + 0x12u);
        s.successor = q;
        p = SW(d + c * 2u);
        d = SW(a + c * 2u);
        b = SW(a + q * 2u);
        s.initial = d;
        d -= b;
        b = D(object + 0x16u);
        s.predecessor = p;
        r = SW(b + c * 2u);
        q = SW(b + q * 2u);
        b = SW(b + p * 2u);
        s.y_value = r;
        r -= q;
        q = SW(a + p * 2u);
        a = s.initial;
        p = s.y_value;
        q -= a;
        b -= p;
        p = curve(d, r, q, b);
        s.value = p;
        s.new_sign = (s32)p < 0 ? 0xffffffffu : 1u;
        if (p == 0 || ((s32)p >= (s32)(0u - limit) && (s32)p <= (s32)limit))
            goto walk_flags;
        if (s.new_sign == s.saved_sign ||
            ((s32)s.saved_curve >= (s32)(0u - limit) &&
             (s32)s.saved_curve <= (s32)limit)) goto walk_update;
        r = s.out;
        d = s.saved_predecessor;
        b = s.saved_successor;
        q = s.predecessor;
        a = W(r + d * 2u);
        d = s.saved_center;
        a |= W(r + d * 2u);
        a |= W(r + b * 2u);
        if (a & 0x100u) goto walk_update;
        d = s.successor;
        a = W(r + q * 2u);
        a |= W(r + d * 2u);
        a |= W(r + c * 2u);
        if (a & 0x100u) goto walk_update;
        d = 0;
        a = b;
        if (b != q) {
            do {
                p = s.pred_base;
                ++d;
                a = SW(p + a * 2u);
            } while (a != q);
            p = s.value;
        }
        a = d + 1u;
        a = (u32)((s32)a / 2);
        d = b;
        if ((s32)a > 0) do {
            b = s.pred_base;
            --a;
            d = SW(b + d * 2u);
        } while (a != 0);
        a = W(r + d * 2u);
        if (!(a & 0x100u)) W(r + d * 2u) = (u16)(a | 0x80u);
walk_update:
        s.saved_successor = s.successor;
        s.saved_center = c;
        s.saved_predecessor = s.predecessor;
        s.saved_curve = p;
        s.saved_sign = s.new_sign;
walk_flags:
        if (B(s.out + c * 2u + 1u) & 1u) s.saved_curve = 0;
        a = s.visits + 1u;
        s.visits = a;
        if ((s32)a > 1000) s.visits = a + 1u;
        a = s.pred_base;
        c = SW(a + c * 2u);
        if (c != s.start) goto walk;
contour_done:
        a = s.outer;
        c = SW(object + 4u);
        ++a;
        s.outer = a;
        if ((s32)a < (s32)c) goto contour_begin;
    }

    /* 00935b82..00935be4. */
    value = last + 4u;
    if ((s32)value >= 0) {
        table = output + value * 2u;
        remaining = value + 1u;
        do {
            bits = W(table);
            if (bits & 0x104u) W(table) = (u16)(bits | 1u);
            bits = W(table);
            if (bits & 0x108u) W(table) = (u16)(bits | 2u);
            table -= 2u;
        } while (--remaining != 0);
    }
    W(output + last * 2u + 2u) |= 1u;
    W(output + last * 2u + 4u) |= 1u;
    value = SW(object + 0x29cu);
    threshold = (u32)((s32)value / 10);
    i = last;
    if ((s32)i >= 0) do {
        table = D(object + 0x1cu);
        before = SW(table + i * 2u);
        if (B(flags + i) != 0 && B(flags + before) != 0) {
            table = D(object + 0x16u);
            xbase = D(object + 0x12u);
            a = W(table + i * 2u);
            a = (u16)(a - W(table + before * 2u));
            b = W(xbase + i * 2u);
            b = (u16)(b - W(xbase + before * 2u));
            if ((s16)b < 0) b = 0u - b;
            if ((s16)a < 0) a = 0u - a;
            if ((s16)b > (s16)a) {
                a = (u16)((s16)a >> 1);
                value = a + b;
            } else {
                b = (u16)((s16)b >> 1);
                value = b + a;
            }
            if ((s32)(s16)value >= (s32)threshold) {
                table = D(object + 0x40u);
                value = D(table + i * 4u);
                if ((s32)value >= -0x2cb && (s32)value <= 0x2cb) bits = 1;
                else {
                    table = D(object + 0x44u);
                    value = D(table + i * 4u);
                    bits = (s32)value >= -0x2cb && (s32)value <= 0x2cb ? 2u : 3u;
                }
                B(output + i * 2u) |= (u8)bits;
                B(output + before * 2u) |= (u8)bits;
            }
        }
    } while ((s32)--i >= 0);
    return i;
}
