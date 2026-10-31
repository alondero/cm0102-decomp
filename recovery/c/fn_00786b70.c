#include "cmre.h"

/* Preferred-image VA 0x00786b70, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The destination remains in ECX through either successful search path. */
extern unsigned long long CM_THISCALL cm_fn_00786010(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_THISCALL cm_fn_00786b70(u32 self, u32 person, u32 type, u32 destination)
{
    u8 scratch[512];
    u8 *filename, *extension;
    u32 line, index, base, record, value, count;
    char *version;

    if (!type) {
        filename = scratch + 256; extension = scratch; line = 0x2252;
        goto error;
    }
    if (!destination) {
        filename = scratch; extension = scratch + 256; line = 0x225a;
        goto error;
    }
    if (person) {
        value = D(person);
        count = D(0x00ae2364);
        index = value - count + 16u;
        if ((s32)index < 0) {
            filename = scratch; extension = scratch + 256; line = 0x2268;
            goto error;
        }
        base = D(self + 8);
        value = D(base + index * 12u);
        if (value == 0xffffffffu) return 0;
        base = D(self);
        record = base + value * 223u;
        index = 0;
        do {
            if (D(record + 0xd6) == type) {
                cm_fn_00786010(destination, record);
                return 1;
            }
            ++index;
            record += 223;
        } while ((s32)index < 100);
        return 0;
    }
    count = D(self + 4);
    index = 0;
    if ((s32)count <= 0) return 0;
    record = D(self);
    do {
        if (D(record + 0xd6) == type) {
            cm_fn_00786010(destination, record);
            return 1;
        }
        ++index;
        record += 223;
    } while ((s32)index < (s32)count);
    return 0;
error:
    cm_fn_00944cff(0x00a693ec, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
