/* Preferred-image VA 0090bf90, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_009475b5(u32, void *);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* REPNE SCASB with ECX=-1 and AL=0; count includes the terminator. */
static u32 scan(u32 address)
{
    u32 remaining = 0xffffffffu;
    while (remaining) {
        u8 value = B(address);
        ++address;
        --remaining;
        if (value == 0) break;
    }
    return ~remaining;
}

static void copy(u32 destination, u32 source, u32 count)
{
    u32 dwords = count >> 2;
    for (u32 i = 0; i < dwords; ++i, source += 4, destination += 4) {
        u32 value = D(source);
        D(destination) = value;
    }
    for (u32 i = 0; i < (count & 3); ++i, ++source, ++destination) {
        u8 value = B(source);
        B(destination) = value;
    }
}

u32 CM_CDECL cm_fn_0090bf90(u32 path)
{
    u8 scratch[0x224];
    u32 line;
    u8 *name = scratch + 0x124;
    u8 *extension = scratch + 0x24;
    if (!path) {
        line = 0x29d;
        goto error;
    }
    if (scan(path) - 1 == 0) {
        line = 0x2a4;
        name = scratch + 0x24;
        extension = scratch + 0x124;
        goto error;
    }
    u32 count = scan(path);
    u32 buffer = (u32)cm_fn_00944e46(count + 1);
    count = scan(path);
    copy(buffer, path, count);
    u32 length = scan(buffer) - 1;
    if (B(buffer + length - 1) == 0x5c) {
        length = scan(buffer) - 1;
        B(buffer + length - 1) = 0;
    }
    length = scan(buffer) - 1;
    if (B(buffer + length - 1) == 0x3a) {
        count = scan(0x009c3730);
        u32 end = buffer + scan(buffer) - 1;
        copy(end, 0x009c3730, count);
    }
    u32 status = cm_fn_009475b5(buffer, scratch);
    cm_fn_009452ca(buffer);
    if (status == 0xffffffffu) return 0;
    u32 flags = D((u32)scratch + 6);
    return (flags & 0x4000) != 0;
error:
    cm_fn_00944cff(0x00ad4134, 0, 0, name, extension);
    u32 text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
