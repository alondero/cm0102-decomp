/* Preferred-image VA 0x005267c0, base 0x00400000.
   CM 01/02 3.9.68 SHA-256:
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern void CM_CDECL cm_fn_005a8f60(void);
extern void CM_CDECL cm_fn_005cc7d0(void);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern u32 CM_CDECL cm_fn_0060ba80(u32, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern void *CM_CDECL cm_fn_00945501(u32, u32);
extern unsigned long long CM_CDECL cm_fn_00945667(u32, u32);
extern void CM_CDECL cm_fn_00945934(u32);

#define G(a) CM_GLOBAL(volatile u32, a)
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define W(a) (*(volatile s16 *)(u32)(a))

static void report_error(u8 *first, u8 *second, u32 line)
{
    char *version;
    cm_fn_00944cff(0x009b5ac8, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                  version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0,
                  (const char *)0x00ae24d0, 0);
    G(0x00b67a34) = 0;
}

static void fatal_error(u8 *first, u8 *second, u32 line)
{
    report_error(first, second, line);
    cm_fn_005cc7d0();
    cm_fn_005a8f60();
    cm_fn_00945934(0xffffffffu);
}

static void remap_field(u32 address, u32 map)
{
    u32 old = D(address), value = 0;
    if (old != 0) {
        u32 id = D(old);
        value = D(map + id * 4u);
    }
    D(address) = value;
}

u32 CM_CDECL cm_fn_005267c0(u8 mode)
{
    u8 scratch[0x628];
    u32 map, mask, rows, base, count, last, record, nation, tag, row;
    u32 n, q, index, old_id, current_id, slot, cursor, cached_base;
    u32 i, offset, pointer, id, limit, destination, result, keep;
    u32 special, capacity = 20, kept = 0;
    s16 quality, old_quality;

    count = G(0x00ae235c);
    map = (u32)cm_fn_00944cf1(count * 4u);
    if (map == 0)
        return 0;
    if (G(0x00ae23a8) == 0) {
        report_error(scratch + 0x228, scratch + 0x328, 0x4f6);
        return (u32)cm_fn_00944c94((void *)map);
    }

    special = mode == 2 || mode == 1;
    count = G(0x00ae235c);
    mask = (u32)cm_fn_00945501(count, 4);
    if (mask == 0)
        fatal_error(scratch + 0x428, scratch + 0x528, 0x513);
    count = G(0x00ae235c);
    base = G(0x00ae23b4);
    last = base + count * 581u - 581u;
    count = G(0x00ae2350);
    rows = (u32)cm_fn_00945501(count, 208);
    if (rows == 0)
        fatal_error(scratch + 0x28, scratch + 0x128, 0x51b);
    if (special != 0)
        capacity = 15;

    record = G(0x00ae23b4);
    for (; record <= last; record += 581u) {
        nation = D(record + 0x53u);
        if (nation == 0)
            continue;
        tag = D(nation);
        q = B(nation + 0x11cu);
        row = rows + tag * 208u;
        if (q == 0 && tag != G(0x009cf2e4) && tag != G(0x009cf360)
            && tag != G(0x009cf314) && tag != G(0x009cf488)
            && tag != G(0x009cf300) && tag != G(0x009cf408)
            && tag != G(0x009cf260) && tag != G(0x009cf218)
            && tag != G(0x009cf2c0) && tag != G(0x009cf244)
            && tag != G(0x009cf338) && tag != G(0x009cf36c)
            && tag != G(0x009cf434) && tag != G(0x009cf45c)
            && tag != G(0x009cf4a8)) {
            if (mode == 2) {
                if ((s32)D(row + 0xc8u) > 4)
                    continue;
            } else if (mode == 1 && (s32)D(row + 0xc8u) > 10) {
                continue;
            }
        }
        n = D(row + 0xc8u);
        if ((s32)n < (s32)capacity) {
            id = D(record);
            D(row + n * 4u) = id;
            q = D(row + 0xc8u);
            if (q == 0) {
                D(row + 0xccu) = q;
            } else {
                index = D(row + 0xccu);
                base = G(0x00ae23b4);
                old_id = D(row + index * 4u);
                quality = W(record + 0x80u);
                old_quality = W(base + old_id * 581u + 0x80u);
                if (quality < old_quality)
                    D(row + 0xccu) = q;
            }
            D(row + 0xc8u) = q + 1u;
        } else {
            index = D(row + 0xccu);
            base = G(0x00ae23b4);
            old_id = D(row + index * 4u);
            slot = row + index * 4u;
            quality = W(record + 0x80u);
            old_quality = W(base + old_id * 581u + 0x80u);
            if (quality <= old_quality)
                continue;
            id = D(record);
            D(slot) = id;
            n = D(row + 0xc8u);
            if ((s32)n > 0) {
                cached_base = G(0x00ae23b4);
                cursor = row;
                for (i = 0; (s32)i < (s32)n; ++i, cursor += 4u) {
                    if (i == 0) {
                        D(row + 0xccu) = i;
                        cached_base = G(0x00ae23b4);
                    } else {
                        index = D(row + 0xccu);
                        old_id = D(row + index * 4u);
                        current_id = D(cursor);
                        quality = W(cached_base + current_id * 581u + 0x80u);
                        old_quality = W(cached_base + old_id * 581u + 0x80u);
                        if (quality < old_quality) {
                            D(row + 0xccu) = i;
                            cached_base = G(0x00ae23b4);
                        }
                    }
                }
            }
        }
    }

    count = G(0x00ae2350);
    cursor = rows + 0xc8u;
    for (i = 0; (s32)i < (s32)count; ++i, cursor += 208u) {
        n = D(cursor);
        pointer = cursor - 0xc8u;
        for (q = 0; (s32)q < (s32)n; ++q, pointer += 4u) {
            id = D(pointer);
            D(mask + id * 4u) = 1;
            n = D(cursor);
        }
        count = G(0x00ae2350);
    }
    cm_fn_009452ca(rows);

    count = G(0x00ae2364);
    base = G(0x00ae23bc);
    limit = base + count * 110u - 110u;
    for (record = base; record <= limit; record += 110u) {
        pointer = D(record + 0x39u);
        if (pointer != 0) {
            id = D(pointer);
            D(mask + id * 4u) = 1;
        }
        pointer = D(record + 0x24u);
        if (pointer != 0) {
            id = D(pointer);
            D(mask + id * 4u) = 1;
        }
    }
    count = G(0x00ae237c);
    base = G(0x00ae23d4);
    limit = base + count * 26u - 26u;
    for (record = base; record <= limit; record += 26u) {
        for (offset = 10; offset <= 18; offset += 4) {
            pointer = D(record + offset);
            if (pointer != 0) {
                id = D(pointer);
                count = G(0x00ae235c);
                if ((s32)id < (s32)count && (s32)id >= 0)
                    D(mask + id * 4u) = 1;
            }
        }
    }
    if (special == 0) {
        count = G(0x00ae2374);
        base = G(0x00ae23cc);
        limit = base + count * 17u - 17u;
        for (record = base; record <= limit; record += 17u) {
            pointer = D(record + 10u);
            if (pointer == 0)
                continue;
            q = D(record + 4u);
            if (q == 0)
                continue;
            nation = D(q + 0x1au);
            if (nation == 0 || B(nation + 0x11cu) == 0) {
                nation = D(pointer + 0x53u);
                if (nation == 0 || B(nation + 0x11cu) == 0)
                    continue;
            }
            id = D(pointer);
            D(mask + id * 4u) = 1;
        }
    }

    base = G(0x00ae23b4);
    record = destination = base;
    while (record <= last) {
        index = (u32)((s32)(destination - base) / 581);
        id = D(record);
        result = cm_fn_0060ba80(id, index);
        if (result != 0) {
            id = D(record);
            keep = 1;
        } else {
            id = D(record);
            keep = D(mask + id * 4u) != 0;
            if (keep == 0) {
                n = G(0x00ae2350);
                count = G(0x00ae235c);
                keep = (s32)id >= (s32)(count - n * 2u);
                if (keep == 0) {
                    nation = D(record + 0x53u);
                    if (nation != 0 && B(nation + 0x11cu) != 0) {
                        tag = D(nation);
                        q = G(0x009cf2e4);
                        keep = tag != q || W(record + 0x80u) > 500
                               || special == 0;
                    }
                }
            }
        }
        D(map + id * 4u) = keep != 0 ? destination : 0;
        if (keep != 0)
            destination += 581u;
        record += 581u;
        if (record > last)
            break;
        base = G(0x00ae23b4);
    }
    cm_fn_009452ca(mask);

    /* Each field reloads its global base. The same captured address is used
       for that field's read and store, including overlapping records. */
    count = G(0x00ae235c);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 581u) {
        base = G(0x00ae23b4); remap_field(base + offset + 0xb3u, map);
        base = G(0x00ae23b4); remap_field(base + offset + 0xb7u, map);
        base = G(0x00ae23b4); remap_field(base + offset + 0xbbu, map);
        count = G(0x00ae235c);
    }
    count = G(0x00ae2364);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 110u) {
        base = G(0x00ae23bc); remap_field(base + offset + 0x39u, map);
        base = G(0x00ae23bc); remap_field(base + offset + 0x24u, map);
        count = G(0x00ae2364);
    }
    count = G(0x00ae2370);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 52u) {
        for (q = 4; q <= 24; q += 4) {
            base = G(0x00ae23c8); remap_field(base + offset + q, map);
        }
        count = G(0x00ae2370);
    }
    count = G(0x00ae2374);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 17u) {
        base = G(0x00ae23cc); remap_field(base + offset + 10u, map);
        count = G(0x00ae2374);
    }
    count = G(0x00ae237c);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 26u) {
        for (q = 10; q <= 22; q += 4) {
            base = G(0x00ae23d4); remap_field(base + offset + q, map);
        }
        count = G(0x00ae237c);
    }

    count = G(0x00ae235c);
    n = 0;
    if ((s32)count > 0) {
        cursor = map;
        do {
            if (D(cursor) != 0)
                ++n;
            cursor += 4u;
        } while (--count != 0);
    }
    result = (u32)cm_fn_00944e46(n * 4u);
    G(0x00ae23fc) = result;
    if (result == 0)
        return (u32)cm_fn_00944c94((void *)map);

    count = G(0x00ae235c);
    cursor = map;
    for (i = 0, offset = 0; (s32)i < (s32)count;
         ++i, offset += 581u, cursor += 4u) {
        destination = D(cursor);
        if (destination != 0) {
            base = G(0x00ae23b4);
            pointer = base + offset;
            for (q = 0; q != 145; ++q) {
                id = D(pointer + q * 4u);
                D(destination + q * 4u) = id;
            }
            B(destination + 580u) = B(pointer + 580u);
            base = G(0x00ae23fc);
            D(base + kept * 4u) = i;
            destination = D(cursor);
            D(destination) = kept;
            ++kept;
        }
        count = G(0x00ae235c);
    }
    base = G(0x00ae23b4);
    G(0x00ae235c) = kept;
    result = (u32)cm_fn_00945667(base, kept * 581u);
    count = G(0x00ae235c);
    n = G(0x00ae2360);
    base = G(0x00ae23b4);
    G(0x00ae23b8) = base + (count - n) * 581u;
    if (result != base)
        fatal_error(scratch + 0x128, scratch + 0x28, 0x4ca);
    cm_fn_00944c94((void *)map);
    count = G(0x00ae235c);
    return cm_fn_00944c9f(0x00de1f64, 0x009b5af4, count);
}
