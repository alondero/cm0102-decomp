#include "cmre.h"

/* Preferred-image VA 0x005fcb60, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Four recent paths; an absent match selects slot three. Each original copy
   remains a boundary, followed by fresh base/slot reads for its terminator. */
extern u32 CM_CDECL cm_fn_009466d0(u32, u32, u32);
extern s32 CM_CDECL cm_fn_00951c50(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

static u32 path_address(u32 self, u32 row, u8 type, u32 offset)
{
    u32 base = D(self);
    if (type == 1) {
        u32 slot = (u32)(s32)(s8)B(base + row + 0x9a69);
        return base + row + slot * 7885u + offset + 0x19f5;
    }
    return base + row + offset + (type == 3 ? 0xa1c0u : 0x9cacu);
}

u32 CM_THISCALL cm_fn_005fcb60(u32 self, u32 path, u32 person, u8 type)
{
    u8 scratch[0x20c];
    u32 filename = (u32)scratch + 12, extension = (u32)scratch + 0x10c;
    u32 line, index, value, limit, row, offset, selected, destination;
    char *version;

    if (!path) {
        filename = (u32)scratch + 0x10c; extension = (u32)scratch + 12;
        line = 0x328;
        goto error;
    }
    if (!person) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        if (!person) { line = 0x333; goto error; }
    }
    if (type == 4) { line = 0x33c; goto error; }
    value = D(person);
    limit = D(0x00ae2364);
    index = value - limit + 16u;
    if ((s32)index < 0) { line = 0x347; goto error; }
    limit = D(self + 4) - 1u;
    if ((s32)index > (s32)limit) { line = 0x347; goto error; }
    row = index * 42735u;
    offset = 0;
    selected = 0;
    for (;;) {
        destination = path_address(self, row, type, offset);
        if (!cm_fn_00951c50(destination, path)) break;
        offset += 260;
        ++selected;
        if ((s32)offset >= 1040) {
            selected = 3;
            break;
        }
    }
    offset = selected * 260u;
    while ((s32)selected > 0) {
        destination = path_address(self, row, type, offset);
        cm_fn_009466d0(destination, destination - 260u, 260);
        destination = path_address(self, row, type, offset);
        B(destination + 259) = 0;
        offset -= 260;
        --selected;
    }
    destination = path_address(self, row, type, 0);
    cm_fn_009466d0(destination, path, 260);
    destination = path_address(self, row, type, 0);
    B(destination + 259) = 0;
    return 1;
error:
    cm_fn_00944cff(0x009cf11c, 0, 0, (void *)filename, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
