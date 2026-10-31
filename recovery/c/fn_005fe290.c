#include "cmre.h"

/* Preferred-image VA 0x005fe290, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Compare only the seven original fields, in their original access order. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_005fe290(u32 self, u32 packet, u32 person)
{
    static const struct { u32 width, packet_offset, record_offset; } fields[7] = {
        {1, 0x3f, 0x9b13}, {2, 0x2a, 0x9afe}, {2, 0x28, 0x9afc},
        {4, 0, 0x9ad4}, {2, 0x34, 0x9b08}, {4, 0x0c, 0x9ae0}, {4, 0x10, 0x9ae4}
    };
    u8 scratch[512];
    u32 filename = (u32)scratch, extension = (u32)scratch + 256;
    u32 line, index, value, count, record, other, i, a, b;
    char *version;
    if (!packet) {
        filename = (u32)scratch + 256; extension = (u32)scratch;
        line = 0x66e;
        goto error;
    }
    if (!person) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        if (!person) return 0;
    }
    value = D(person);
    count = D(0x00ae2364);
    index = value - count + 16u;
    if ((s32)index < 0) { line = 0x684; goto error; }
    count = D(self + 4) - 1u;
    if ((s32)index > (s32)count) { line = 0x684; goto error; }
    record = D(self);
    record += index * 42735u;
    for (i = 0; i < 7; ++i) {
        a = packet + fields[i].packet_offset;
        b = record + fields[i].record_offset;
        if (fields[i].width == 1) {
            value = B(a);
            other = B(b);
        } else if (fields[i].width == 2) {
            value = W(a);
            other = W(b);
        } else {
            value = D(a);
            other = D(b);
        }
        if (value != other) return 0;
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
