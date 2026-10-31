#include "cmre.h"
/* Preferred-image VA 0x005315a0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define G(a) CM_GLOBAL(volatile u32, a)
#define D(a) (*(volatile u32 *)(u32)(a))

/* Each invocation captures the source base for its read/store pair. A zero
   target_global reuses that captured base, as the original instructions do. */
static void resolve(u32 source_global, u32 offset, u32 field,
                    u32 target_global, u32 stride, u32 bound, int inclusive)
{
    u32 base = G(source_global), id = D(base + offset + field), value = 0;
    if ((s32)id >= 0) {
        if (bound != 0) {
            u32 limit = G(bound);
            if (inclusive ? (s32)id > (s32)limit : (s32)id >= (s32)limit)
                goto store;
        }
        value = (target_global == 0 ? base : G(target_global)) + id * stride;
    }
store:
    D(base + offset + field) = value;
}

static void club_warning(u32 offset, u32 field, u32 target, u32 stride,
                         u32 bound, u32 format, u32 bad_field)
{
    u32 base = G(0x00ae23b4), id = D(base + offset + field), value = 0;
    if ((s32)id >= 0) {
        u32 limit = G(bound);
        if ((s32)id > (s32)limit) {
            cm_fn_00944c9f(0x00de1f64, format, id);
            base = G(0x00ae23b4);
            D(base + offset + bad_field) = 0;
            return;
        }
        value = G(target) + id * stride;
    }
    D(base + offset + field) = value;
}

/* Most staff arrays load the staff count before the field and test the upper
   bound before the sign. The manager slot at +CF reverses those two tests. */
static void club_staff(u32 offset, u32 field, u32 club_index,
                       u32 format, int manager)
{
    u32 base = G(0x00ae23b4), limit, id, value = 0;
    if (!manager)
        limit = G(0x00ae2364);
    id = D(base + offset + field);
    if (manager) {
        if ((s32)id < 0)
            goto store;
        limit = G(0x00ae2364);
    }
    if ((s32)id >= (s32)limit) {
        cm_fn_00944c9f(0x00de1f64, format, club_index, id);
        base = G(0x00ae23b4);
    } else if ((s32)id >= 0) {
        value = G(0x00ae23bc) + id * 110u;
    }
store:
    D(base + offset + field) = value;
}

static void report_error(u8 *first, u8 *second, u32 line)
{
    char *version;
    cm_fn_00944cff(0x009b5ac8, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
}

static void staff_data(u32 offset, u32 index, u32 field, u32 bound,
                       u32 target, u32 stride, u32 format,
                       u8 *first, u8 *second, u32 line)
{
    u32 base = G(0x00ae23bc), id = D(base + offset + field), value = 0;
    if ((s32)id >= 0) {
        u32 limit = G(bound);
        if ((s32)id >= (s32)limit) {
            cm_fn_00944c9f(0x00de1f64, format, index, id);
            report_error(first, second, line);
            base = G(0x00ae23bc);
            G(0x00b67a34) = 0;
        } else {
            value = G(target) + id * stride;
        }
    }
    D(base + offset + field) = value;
}

u32 CM_CDECL cm_fn_005315a0(u32 mode)
{
    u8 scratch[0x604];
    u32 i, offset, q, base, id, count, value, limit;
    static const u32 club_comp[] = {0x83,0x8b,0x93,0x87,0x8f,0x97};
    static const u32 nation_comp[] = {0x90,0x98,0xa0,0x94,0x9c,0xa4};

    count = G(0x00ae235c);
    for (i = 0; (s32)i < (s32)count; ++i) {
        offset = i * 581u;
        club_warning(offset, 0x53, 0x00ae23a8, 290, 0x00ae2350, 0x009b62b0, 0x53);
        resolve(0x00ae23b4, offset, 0x57, 0x00ae23d0, 107, 0x00ae2378, 1);
        /* Original bug: high +5B index clears +60, leaving +5B untouched. */
        club_warning(offset, 0x5b, 0x00ae23d0, 107, 0x00ae2378, 0x009b6274, 0x60);
        resolve(0x00ae23b4, offset, 0x60, 0x00ae23d0, 107, 0x00ae2378, 1);
        club_warning(offset, 0x69, 0x00ae23b0, 78, 0x00ae2358, 0x009b6240, 0x69);
        resolve(0x00ae23b4, offset, 0x6e, 0x00ae23b0, 78, 0x00ae2358, 1);
        for (q = 0; q != 6; ++q)
            resolve(0x00ae23b4, offset, club_comp[q], 0x00ae23ec, 58, 0x00ae2394, 1);
        for (q = 0x9b; q <= 0xaf; q += 4)
            resolve(0x00ae23b4, offset, q, 0x00ae23bc, 110, 0x00ae2364, 1);
        for (q = 0xb3; q <= 0xbb; q += 4)
            resolve(0x00ae23b4, offset, q, 0, 581, 0x00ae235c, 1);

        for (q = 0; q != 50; ++q) {
            base = G(0x00ae23b4);
            if (mode == 0) {
                id = D(base + offset + 0xd7u + q * 4u);
                limit = G(0x00ae2364);
                if ((s32)id >= (s32)limit) {
                    limit += 0x52du;
                    if ((s32)id > (s32)limit) {
                        cm_fn_00944c9f(0x00de1f64, 0x009b6218, i, id);
                        base = G(0x00ae23b4);
                    }
                    D(base + offset + 0xd7u + q * 4u) = 0;
                    continue;
                }
            }
            id = D(base + offset + 0xd7u + q * 4u);
            value = (s32)id < 0 ? 0 : G(0x00ae23bc) + id * 110u;
            D(base + offset + 0xd7u + q * 4u) = value;
        }
        for (q = 0; q != 7; ++q)
            club_staff(offset, 0x1b3u + q * 4u, i, 0x009b61f4, 0);
        for (q = 0; q != 5; ++q)
            club_staff(offset, 0x19fu + q * 4u, i, 0x009b61d0, 0);
        for (q = 0; q != 3; ++q)
            club_staff(offset, 0x1cfu + q * 4u, i, 0x009b61ac, 0);
        for (q = 0; q != 3; ++q)
            club_staff(offset, 0xc3u + q * 4u, i, 0x009b6188, 0);
        club_staff(offset, 0xcf, i, 0x009b6164, 1);
        club_staff(offset, 0xbf, i, 0x009b6140, 0);
        club_staff(offset, 0xd3, i, 0x009b6110, 0);
        for (q = 0; q != 20; ++q)
            club_staff(offset, 0x1e0u + q * 4u, i, 0x009b60dc, 0);
        count = G(0x00ae235c);
    }

    count = G(0x00ae2350);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 290u) {
        resolve(0x00ae23a8, offset, 0x71, 0x00ae23a4, 198, 0, 0);
        resolve(0x00ae23a8, offset, 0x7a, 0x00ae23ac, 56, 0, 0);
        resolve(0x00ae23a8, offset, 0x80, 0x00ae23b0, 78, 0, 0);
        for (q = 0; q != 6; ++q)
            resolve(0x00ae23a8, offset, nation_comp[q], 0x00ae23ec, 58, 0, 0);
        for (q = 0x110; q <= 0x118; q += 4)
            resolve(0x00ae23a8, offset, q, 0, 290, 0, 0);
        count = G(0x00ae2350);
    }
    count = G(0x00ae2390);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 43u) {
        resolve(0x00ae23e8, offset, 4, 0x00ae23f0, 60, 0, 0);
        resolve(0x00ae23e8, offset, 8, 0x00ae23f4, 60, 0, 0);
        resolve(0x00ae23e8, offset, 0x16, 0x00ae23a8, 290, 0, 0);
        resolve(0x00ae23e8, offset, 0x1a, 0x00ae23ac, 56, 0, 0);
        count = G(0x00ae2390);
    }
    count = G(0x00ae2358);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 78u) {
        resolve(0x00ae23b0, offset, 0x38, 0x00ae23ac, 56, 0x00ae2354, 1);
        base = G(0x00ae23b0);
        id = D(base + offset + 0x48u);
        if ((s32)id < 0) {
            D(base + offset + 0x48u) = 0;
        } else {
            limit = G(0x00ae2358);
            if ((s32)id > (s32)limit)
                D(base + offset + 0x38u) = 0; /* Original adjacent-field bug. */
            else
                D(base + offset + 0x48u) = base + id * 78u;
        }
        count = G(0x00ae2358);
    }
    count = G(0x00ae2370);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 52u) {
        for (q = 4; q <= 24; q += 4)
            resolve(0x00ae23c8, offset, q, 0x00ae23b4, 581, 0, 0);
        for (q = 28; q <= 48; q += 4)
            resolve(0x00ae23c8, offset, q, 0x00ae23bc, 110, 0, 0);
        count = G(0x00ae2370);
    }
    count = G(0x00ae2368);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 68u) {
        for (q = 0x23; q <= 0x3f; q += 4)
            resolve(0x00ae23c0, offset, q, 0x00ae23bc, 110, 0x00ae2364, 0);
        count = G(0x00ae2368);
    }
    count = G(0x00ae2364);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 110u) {
        resolve(0x00ae23bc, offset, 4, 0x00ae23f0, 60, 0, 0);
        resolve(0x00ae23bc, offset, 8, 0x00ae23f4, 60, 0, 0);
        resolve(0x00ae23bc, offset, 12, 0x00ae23f8, 60, 0, 0);
        resolve(0x00ae23bc, offset, 0x1a, 0x00ae23a8, 290, 0, 0);
        resolve(0x00ae23bc, offset, 0x39, 0x00ae23b4, 581, 0, 0);
        resolve(0x00ae23bc, offset, 0x1e, 0x00ae23a8, 290, 0, 0);
        resolve(0x00ae23bc, offset, 0x24, 0x00ae23b4, 581, 0, 0);
        staff_data(offset, i, 0x61, 0x00ae236c, 0x00ae23c4, 70, 0x009b60b4,
                   scratch + 0x204, scratch + 0x404, 0x18f6);
        staff_data(offset, i, 0x65, 0x00ae2370, 0x00ae23c8, 52, 0x009b6088,
                   scratch + 0x104, scratch + 4, 0x1907);
        staff_data(offset, i, 0x69, 0x00ae2368, 0x00ae23c0, 68, 0x009b605c,
                   scratch + 0x504, scratch + 0x304, 0x1918);
        count = G(0x00ae2364);
    }
    count = G(0x00ae2388);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 101u) {
        resolve(0x00ae23e0, offset, 0x53, 0x00ae23a4, 198, 0x00ae234c, 0);
        resolve(0x00ae23e0, offset, 0x57, 0x00ae23a8, 290, 0x00ae2350, 0);
        resolve(0x00ae23e0, offset, 0x5b, 0x00ae23ec, 58, 0x00ae2394, 0);
        resolve(0x00ae23e0, offset, 0x5f, 0x00ae23ec, 58, 0x00ae2394, 0);
        count = G(0x00ae2388);
    }
    count = G(0x00ae2374);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 17u) {
        resolve(0x00ae23cc, offset, 4, 0x00ae23bc, 110, 0, 0);
        resolve(0x00ae23cc, offset, 10, 0x00ae23b4, 581, 0, 0);
        count = G(0x00ae2374);
    }
    count = G(0x00ae2354);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 56u) {
        resolve(0x00ae23ac, offset, 0x1f, 0x00ae23a8, 290, 0, 0);
        count = G(0x00ae2354);
    }
    count = G(0x00ae237c);
    for (i = 0, offset = 0; (s32)i < (s32)count; ++i, offset += 26u) {
        resolve(0x00ae23d4, offset, 4, 0x00ae23d0, 107, 0, 0);
        resolve(0x00ae23d4, offset, 10, 0x00ae23b4, 581, 0, 0);
        resolve(0x00ae23d4, offset, 14, 0x00ae23b4, 581, 0, 0);
        resolve(0x00ae23d4, offset, 18, 0x00ae23b4, 581, 0x00ae235c, 1);
        resolve(0x00ae23d4, offset, 22, 0x00ae23b4, 581, 0x00ae235c, 1);
        count = G(0x00ae237c);
    }
    count = G(0x00ae2378);
    if ((s32)count <= 0)
        return count;
    i = offset = 0;
    do {
        resolve(0x00ae23d0, offset, 0x5d, 0x00ae23a8, 290, 0, 0);
        resolve(0x00ae23d0, offset, 0x59, 0x00ae23a4, 198, 0, 0);
        resolve(0x00ae23d0, offset, 0x61, 0x00ae23ec, 58, 0, 0);
        resolve(0x00ae23d0, offset, 0x65, 0x00ae23ec, 58, 0, 0);
        count = G(0x00ae2378);
        ++i;
        offset += 107u;
    } while ((s32)i < (s32)count);
    return offset;
}
