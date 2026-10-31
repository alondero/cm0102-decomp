#include "cmre.h"

/* WIP: O0 events mismatch, case 2/index20: original CRT consumes unwritten
   Find-output FILETIMEs (read 00946985); O2 passes. One failed invocation.
   Keep inputs unchanged; require original-derived import-output evidence. */
/* Preferred-image VA0079BF60/base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/RET0/void: both direct callers overwrite EAX immediately.
   L is the original 20C-byte scratch base, excluding four saved registers. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern void CM_CDECL cm_fn_0079caa0(void);
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945501(u32, u32);
extern u32 CM_THISCALL cm_fn_0079c530(u32, u32);
extern void CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern void CM_CDECL cm_fn_005cc7d0(void);
extern void CM_CDECL cm_fn_005a8f60(void);
extern void CM_CDECL cm_fn_00945934(u32);
extern u32 CM_CDECL cm_fn_004b0700(u32);
extern u32 CM_THISCALL cm_fn_005c6710(u32, u32);
/* Native 79C5C0 reads two stack arguments, overwrites ECX before use, RET8. */
extern void CM_STDCALL cm_fn_0079c5c0(u32, u32);

static void allocation_error(u8 *name, u8 *extension, u32 line)
{
    u32 text;
    cm_fn_00944cff(0x00a6a22cu, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, (u32)name,
                  (u32)extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    cm_fn_005cc7d0();
    cm_fn_005a8f60();
    cm_fn_00945934(0xffffffffu);
}

void CM_THISCALL cm_fn_0079bf60(u32 self)
{
    u8 scratch[0x20c];
    u32 flags, compact, kept, group, group_count, cursor, i, off, dst;
    u32 count, base, p, q, country, nation_off, slot, value, other, nation;
    u32 nation_record_off, j, slot_cursor, row;
    s32 n, divisor, quotient;

    cm_fn_0079caa0();
    count = D(0x00ae2390u);
    base = D(0x00ae23e8u);
    cm_fn_00945333(base, count, 0x2b, 0x0079d4e0u);
    count = D(0x00ae2390u);
    flags = cm_fn_00945501(count, 1);
    base = D(0x00ae23e8u);
    kept = 0;
    group = D(base + 0x16u);
    cursor = base + 0x16u;
    i = 0;
    group_count = 0;
    count = D(0x00ae2390u);
    D((u32)scratch + 8u) = flags;
    D((u32)scratch) = kept;
    if ((s32)count > 0) {
        D((u32)scratch) = cursor;
        do {
            country = D(cursor);
            if (country != group) {
                group_count = 0;
                group = country;
            } else if (country == 0) {
                B(flags + i) = 0;
            } else {
                value = B(country + 0x11cu);
                if ((s32)group_count <= (value == 0 ? 5 : 60)) {
                    ++kept;
                    B(flags + i) = 1;
                } else {
                    B(flags + i) = 0;
                }
                ++group_count;
            }
            cursor = D((u32)scratch);
            ++i;
            cursor += 0x2bu;
            count = D(0x00ae2390u);
            D((u32)scratch) = cursor;
        } while ((s32)i < (s32)count);
        D((u32)scratch) = kept;
    }
    compact = cm_fn_00945501(kept, 0x2b);
    count = D(0x00ae2390u);
    i = 0;
    D((u32)scratch + 4u) = compact;
    if ((s32)count > 0) {
        off = 0;
        dst = compact;
        do {
            flags = D((u32)scratch + 8u);
            if (B(flags + i) != 0) {
                base = D(0x00ae23e8u);
                p = dst;
                dst += 0x2bu;
                cm_fn_0079c530(p, base + off);
            }
            count = D(0x00ae2390u);
            ++i;
            off += 0x2bu;
        } while ((s32)i < (s32)count);
        kept = D((u32)scratch);
    }
    base = D(0x00ae23e8u);
    cm_fn_009452ca(base);
    count = D(0x00ae2350u);
    compact = D((u32)scratch + 4u);
    D(0x00ae23e8u) = compact;
    D(0x00ae2390u) = kept;
    p = cm_fn_00944e46(count * 0x102u);
    D(self + 0xcu) = p;
    if (p == 0)
        allocation_error(scratch + 0x10c, scratch + 0xc, 0x1ff);

    count = D(0x00ae2350u);
    i = 0;
    if ((s32)count > 0) {
        off = 0;
        do {
            base = D(self + 0xcu);
            W(base + off + 0x10u) = 0;
            base = D(self + 0xcu);
            D(base + off + 4u) = 0;
            base = D(self + 0xcu);
            D(base + off + 8u) = 1000000;
            base = D(self + 0xcu);
            D(base + off + 0xcu) = 0;
            base = D(self + 0xcu);
            D(base + off) = i;
            slot = off;
            j = 15;
            do {
                base = D(self + 0xcu);
                slot += 0x10u;
                --j;
                D(base + slot + 2u) = 1000000;
                base = D(self + 0xcu);
                D(base + slot + 6u) = 0;
                base = D(self + 0xcu);
                D(base + slot + 0xau) = 0;
                base = D(self + 0xcu);
                D(base + slot + 0xeu) = 0xffffffffu;
            } while (j != 0);
            count = D(0x00ae2350u);
            ++i;
            off += 0x102u;
        } while ((s32)i < (s32)count);
    }
    count = D(0x00adadf8u);
    i = 0;
    if ((s32)count > 0) {
        off = 0;
        do {
            base = D(0x00adadfcu);
            if (D(base + i * 4u) != 0 && cm_fn_004b0700(i) != 0) {
                base = D(0x00ae23d0u);
                p = D(0x00ae2c74u);
                if (cm_fn_005c6710(p, base + off) == 0) {
                    base = D(0x00ae23d0u);
                    country = D(base + off + 0x5du);
                    if (country != 0) {
                        nation = D(country);
                        base = D(self + 0xcu);
                        nation_off = nation * 0x102u;
                        n = (s16)W(base + nation_off + 0x10u);
                        slot = nation_off + (u32)n * 0x10u;
                        D(base + slot + 0x1eu) = i;
                        p = D(0x00adadfcu);
                        p = D(p + i * 4u);
                        n = (s16)W(p + 0x3eu);
                        base = D(self + 0xcu);
                        value = (u32)(n / 2) + 10u;
                        D(base + slot + 0x1au) = value;
                        base = D(self + 0xcu);
                        value = D(base + slot + 0x1au);
                        other = D(base + nation_off + 0xcu);
                        if ((s32)value > (s32)other)
                            D(base + nation_off + 0xcu) = value;
                        base = D(self + 0xcu);
                        W(base + nation_off + 0x10u) =
                            (u16)(W(base + nation_off + 0x10u) + 1u);
                    }
                }
            }
            count = D(0x00adadf8u);
            ++i;
            off += 0x6bu;
        } while ((s32)i < (s32)count);
    }
    count = D(0x00ae2390u);
    i = 0;
    if ((s32)count > 0) {
        cursor = D(0x00ae23e8u) + 0x16u;
        do {
            country = D(cursor);
            cursor += 0x2bu;
            nation = D(country);
            base = D(self + 0xcu);
            p = base + nation * 0x102u + 4u;
            value = D(p);
            ++value;
            ++i;
            D(p) = value;
            count = D(0x00ae2390u);
        } while ((s32)i < (s32)count);
    }
    count = D(0x00ae2350u);
    i = 0;
    if ((s32)count > 0) {
        off = 0;
        nation_record_off = 0;
        do {
            base = D(self + 0xcu);
            row = base + off;
            n = (s16)W(base + off + 0x10u);
            if (n > 0) {
                value = (u32)n * D(row + 0xcu);
                D(row + 0xcu) = value;
                base = D(self + 0xcu);
                value = D(base + off + 0xcu);
                other = D(base + off + 4u);
                if ((s32)value > (s32)other) {
                    p = D(0x00ae23a8u);
                    cm_fn_0079c5c0(value - other, p + nation_record_off);
                    base = D(self + 0xcu);
                    row = base + off;
                    value = D(row + 0xcu);
                    D(row + 4u) = value;
                }
            } else if ((s32)D(row + 4u) < 5) {
                p = D(0x00ae23a8u);
                if (W(p + nation_record_off + 0x8eu) == 0) {
                    W(p + nation_record_off + 0x8eu) = 1;
                    p = D(0x00ae23a8u);
                }
                n = (s16)W(p + nation_record_off + 0x8eu);
                if (n >= 5) n = 5;
                cm_fn_0079c5c0((u32)n, p + nation_record_off);
                p = D(0x00ae23a8u);
                n = (s16)W(p + nation_record_off + 0x8eu);
                if (n >= 5) n = 5;
                base = D(self + 0xcu);
                D(base + off + 4u) = (u32)n;
            }
            count = D(0x00ae2350u);
            ++i;
            nation_record_off += 0x122u;
            off += 0x102u;
        } while ((s32)i < (s32)count);
    }
    count = D(0x00ae2390u);
    i = 0;
    if ((s32)count > 0) {
        off = 0;
        do {
            base = D(0x00ae23e8u);
            off += 0x2bu;
            D(base + off - 0x2bu) = i;
            count = D(0x00ae2390u);
            ++i;
        } while ((s32)i < (s32)count);
    }
    count = D(0x00ae2390u);
    cursor = D(0x00ae23e8u);
    i = 0;
    if ((s32)count > 0) {
        do {
            country = D(cursor + 0x16u);
            nation = D(country);
            base = D(self + 0xcu);
            p = base + nation * 0x102u + 8u;
            value = D(p);
            other = D(cursor);
            if ((s32)other < (s32)value) D(p) = other;
            count = D(0x00ae2390u);
            ++i;
            cursor += 0x2bu;
        } while ((s32)i < (s32)count);
    }
    count = D(0x00ae2350u);
    D((u32)scratch + 4u) = 0;
    if ((s32)count > 0) {
        off = 0;
        do {
            base = D(self + 0xcu);
            row = base + off;
            n = (s16)W(base + off + 0x10u);
            if (n != 0) {
                cm_fn_00945333(row + 0x12u, (u32)n, 0x10, 0x0079d410u);
                base = D(self + 0xcu);
                j = 0;
                divisor = (s16)W(base + off + 0x10u);
                row = base + off;
                if (divisor > 0) {
                    D((u32)scratch) = off;
                    do {
                        n = (s32)D(row + 4u);
                        quotient = n / divisor;
                        slot_cursor = D((u32)scratch);
                        D(base + slot_cursor + 0x16u) = (u32)quotient;
                        base = D(self + 0xcu);
                        value = D(base + off + 4u);
                        value *= j;
                        divisor = (s16)W(base + off + 0x10u);
                        quotient = (s32)value / divisor;
                        value = D(base + off + 8u);
                        slot_cursor = D((u32)scratch);
                        ++j;
                        D(base + slot_cursor + 0x12u) = (u32)quotient + value;
                        base = D(self + 0xcu);
                        slot_cursor += 0x10u;
                        divisor = (s16)W(base + off + 0x10u);
                        D((u32)scratch) = slot_cursor;
                        row = base + off;
                    } while ((s32)j < divisor);
                }
            }
            i = D((u32)scratch + 4u);
            count = D(0x00ae2350u);
            ++i;
            off += 0x102u;
            D((u32)scratch + 4u) = i;
        } while ((s32)i < (s32)count);
    }
    count = D(0x00ae2390u);
    D(self + 4u) = count;
    q = cm_fn_00945501(count, 1);
    D(self + 8u) = q;
    if (q == 0)
        allocation_error(scratch + 0xc, scratch + 0x10c, 0x2dd);
    flags = D((u32)scratch + 8u);
    cm_fn_009452ca(flags);
}
