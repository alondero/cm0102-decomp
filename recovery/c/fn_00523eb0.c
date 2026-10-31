#include "cmre.h"

/* Preferred-image VA 0x00523eb0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Club pointers are replaced with IDs before the bulk write, then rebuilt.
   Faults can therefore leave the original object partly converted. */
extern u32 CM_CDECL cm_fn_00945ce6(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

static const struct { u32 offset, width; } fields[15] = {
    {0x28,4}, {0x51,1}, {0x49,1}, {0x4a,1}, {0xb1,1},
    {0x3c,2}, {0x3e,2}, {0x40,2}, {0xab,1}, {0xac,1},
    {0x44,1}, {0x42,1}, {0x3a,2}, {0x1c,4}, {0x20,4}
};

static u32 report(u8 *filename, u8 *extension, u32 line)
{
    u32 eax;
    char *version;
    cm_fn_00944cff(0x009b5944u, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                  filename, extension, line);
    eax = cm_fn_005e8290((const char *)0x009870e0u,
                        (const char *)0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return eax;
}

u32 CM_THISCALL cm_fn_00523eb0(u32 self, u32 stream)
{
    u8 scratch[0x20c];
    u32 eax, i, base, table, pointer, value, limit, index, offset, bytes;

    D((u32)scratch + 8u) = 0xffffffffu;
    if (D(self + 0xa3u) == 0 || D(self + 0xa7u) == 0 || D(self + 0xadu) == 0)
        return report(scratch + 12, scratch + 0x10c, 0x19a);
    for (i = 0; i < 15; ++i)
        cm_fn_00945ce6(self + fields[i].offset, fields[i].width, 1, stream);
    pointer = D(self + 4u);
    if (pointer != 0) {
        cm_fn_00945ce6(pointer, 4, 1, stream);
    } else {
        report(scratch + 0x10c, scratch + 12, 0x1b9);
        cm_fn_00945ce6((u32)scratch + 8u, 4, 1, stream);
    }
    limit = (u32)(s32)(s16)W(self + 0x3cu);
    base = D(self + 0xa3u);
    eax = cm_fn_00945ce6(base, 104, limit, stream);
    index = 0;
    if ((s16)W(self + 0x3eu) > 0) {
        offset = 0;
        do {
            base = D(self + 0xa7u) + offset;
            pointer = D(base);
            if (pointer != 0) value = D(pointer);
            else value = 0xffffffffu;
            D(base) = value;
            limit = (u32)(s32)(s16)W(self + 0x3eu);
            ++index;
            offset += 6u;
        } while ((s32)index < (s32)limit);
    }
    limit = (u32)(s32)(s16)W(self + 0x3eu);
    base = D(self + 0xa7u);
    eax = cm_fn_00945ce6(base, 6, limit, stream);
    index = 0;
    if ((s16)W(self + 0x3eu) > 0) {
        offset = 0;
        do {
            base = D(self + 0xa7u) + offset;
            value = D(base);
            if (value != 0xffffffffu) {
                table = D(0x00ae23b4u);
                D(base) = table + value * 581u;
            } else {
                D(base) = 0;
            }
            limit = (u32)(s32)(s16)W(self + 0x3eu);
            ++index;
            offset += 6u;
            eax = index;
        } while ((s32)index < (s32)limit);
    }
    index = 0;
    if ((s16)W(self + 0x3cu) <= 0) return eax;
    offset = 0;
    do {
        base = D(self + 0xa3u);
        table = D(self + 0xadu);
        value = (u32)(s32)(s16)W(base + offset + 0x1au);
        bytes = value * 53u;
        pointer = D(table + index * 4u);
        eax = cm_fn_00945ce6(pointer, bytes, 1, stream);
        limit = (u32)(s32)(s16)W(self + 0x3cu);
        ++index;
        offset += 104u;
    } while ((s32)index < (s32)limit);
    return eax;
}
