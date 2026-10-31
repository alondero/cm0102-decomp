#include "cmre.h"

/* Preferred-image VA 0x00601070, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Slot one maps to zero before the lookup. Preserve the wrapped address from
   the global record table; a sentinel or diagnostic returns zero. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_00601070(u32 self, s8 column, u32 person)
{
    u8 scratch[512];
    u32 filename = (u32)scratch, extension = (u32)scratch + 256;
    u32 line, index, value, count, base, slot, result;
    char *version;
    if (!person) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        if (!person) {
            filename = (u32)scratch + 256; extension = (u32)scratch;
            line = 0xbf6;
            goto error;
        }
    }
    value = D(person);
    count = D(0x00ae2364);
    index = value - count + 16u;
    if ((s32)index < 0) { line = 0xc02; goto error; }
    count = D(self + 4) - 1u;
    if ((s32)index > (s32)count) { line = 0xc02; goto error; }
    if (column < 0 || column >= 50) { line = 0xc09; goto error; }
    base = D(self);
    slot = B(base + index * 42735u + 0x9a69);
    if (slot == 1) slot = 0;
    slot = (u32)(s32)(s8)slot;
    result = base + slot * 7885u + (u32)(s32)column * 4u;
    value = D(result + index * 42735u + 0x1e05);
    if (value == 0xffffffffu) return 0;
    base = D(0x00ae23bc);
    return base + value * 110u;
error:
    cm_fn_00944cff(0x009cf11c, 0, 0, (void *)filename, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
