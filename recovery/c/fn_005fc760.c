#include "cmre.h"

/* Preferred-image VA 0x005fc760, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   REP scans/copies retain byte/DWORD widths, forward overlap, and wrapping.
   The buffer-size guard does not bound the subsequent string copy. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern void CM_CDECL cm_fn_00945b13(u32, u32, u32, u32, u32);
extern s32 CM_CDECL cm_fn_00951c50(u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

static u32 scan_count(u32 address)
{
    u32 remaining = 0xffffffffu;
    do {
        u8 value = B(address);
        ++address;
        --remaining;
        if (!value) break;
    } while (remaining);
    return ~remaining;
}

static void forward_copy(u32 destination, u32 source, u32 count)
{
    u32 words = count >> 2, bytes = count & 3u;
    while (words--) {
        u32 value = D(source);
        D(destination) = value;
        source += 4;
        destination += 4;
    }
    while (bytes--) {
        u8 value = B(source);
        B(destination) = value;
        ++source;
        ++destination;
    }
}

u32 CM_THISCALL cm_fn_005fc760(u32 self, s8 category, u32 output, s32 capacity,
                             u32 relative, u32 person, u8 type)
{
    u8 scratch[0x70c];
    u32 work = (u32)scratch, filename, extension, line;
    u32 index, count, value, base, row, slot, source, path;
    char *version;

    filename = work + 4;
    extension = work + 0x104;
    if (!output || capacity < 260) { line = 0x286; goto error; }
    B(output) = 0;
    if (category < 0 || category > 3) { line = 0x291; goto error; }
    if (!person) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        if (!person) {
            filename = work + 0x104; extension = work + 4; line = 0x29c;
            goto error;
        }
    }
    if (type == 4) { line = 0x2a5; goto error; }
    value = D(person);
    count = D(0x00ae2364);
    index = value - count + 16u;
    if ((s32)index < 0) { line = 0x2b0; goto error; }
    count = D(self + 4) - 1u;
    if ((s32)index > (s32)count) { line = 0x2b0; goto error; }
    row = index * 42735u;
    base = D(self);
    if (type == 1) {
        slot = (u32)(s32)(s8)B(base + row + 0x9a69);
        source = base + slot * 7885u + (u32)(s32)category * 260u + row + 0x19f5;
    } else {
        source = base + row + (u32)(s32)category * 260u;
        source += type == 3 ? 0xa1c0u : 0x9cacu;
    }
    count = scan_count(source);
    forward_copy(output, source, count);
    if (!relative) return 1;

    cm_fn_00944cff(output, work, work + 0x308,
                  (void *)(work + 0x508), (void *)(work + 0x408));
    cm_fn_00945b13(work + 0x608, work, work + 0x308, 0x00ad9c64, 0x00ad9c64);
    base = D(self);
    if (type == 1) {
        slot = (u32)(s32)(s8)B(base + row + 0x9a69);
        source = base + slot * 7885u + row + 0x18f1;
    } else {
        source = base + row + (type == 3 ? 0xa0bcu : 0x9ba8u);
    }
    count = scan_count(source);
    path = work + 0x204;
    forward_copy(path, source, count);
    count = scan_count(path);
    if (B(path + count - 2u) != 0x5c) {
        count = scan_count(0x009c3730);
        value = scan_count(path);
        forward_copy(path + value - 1u, 0x009c3730, count);
    }
    if (!cm_fn_00951c50(work + 0x608, path)) {
        cm_fn_00945b13(output, 0x00ad9c64, 0x00ad9c64, work + 0x508, work + 0x408);
    }
    return 1;
error:
    cm_fn_00944cff(0x009cf11c, 0, 0, (void *)filename, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
