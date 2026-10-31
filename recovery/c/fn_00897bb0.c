#include "cmre.h"

/* Preferred-image VA 0x00897bb0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   0x008982b0 pops two arguments and does not consume incoming ECX, including
   through 0x00600a50. Another caller observes EDX, so retain its wide result. */
extern unsigned long long CM_STDCALL cm_fn_008982b0(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_THISCALL cm_fn_00897bb0(u32 self, u32 person, s8 row, s8 column,
                             u32 replacement, u32 lookup_flag)
{
    u8 scratch[512];
    u8 *filename, *extension;
    u32 line, index, base, row_offset, record_offset, slot, other, value;
    char *version;

    if (person == 0) {
        filename = scratch + 256; extension = scratch; line = 0x605;
        goto error;
    }
    if (row < 0 || row > 4) {
        filename = scratch; extension = scratch + 256; line = 0x60d;
        goto error;
    }
    if (column < 0 || column > 1) {
        filename = scratch; extension = scratch + 256; line = 0x615;
        goto error;
    }
    index = (u32)cm_fn_008982b0(person, lookup_flag);
    if (index == 0xffffffffu) return 0;
    row_offset = (u32)row * 8u;
    slot = row_offset + (u32)column * 4u;
    base = D(self + 0x601u);
    record_offset = index * 145u;
    if (replacement != 0) {
        value = D(replacement);
        D(base + slot + record_offset + 0x5eu) = value;
        return 1;
    }
    D(base + slot + record_offset + 0x5eu) = 0xffffffffu;
    if (column >= 1) return 1;
    column = (s8)(column + 1);
    if (column >= 2) return 1;
    base = D(self + 0x601u);
    do {
        other = record_offset + row_offset + (u32)column * 4u;
        value = D(base + other + 0x5eu);
        if ((s32)value > -1) {
            value = D(base + other + 0x5eu);
            D(record_offset + base + slot + 0x5eu) = value;
            base = D(self + 0x601u);
            D(other + base + 0x5eu) = 0xffffffffu;
            return 1;
        }
        column = (s8)(column + 1);
    } while (column < 2);
    return 1;

error:
    cm_fn_00944cff(0x00a8b908u, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                  filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0u,
                  (const char *)0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
