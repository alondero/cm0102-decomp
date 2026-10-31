#include "cmre.h"

/* Preferred-image VA 0x005fd6b0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The optional output is cleared before checking the person. The signed
   threshold branch can set it to one while returning a full DWORD zero. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_005fd6b0(u32 self, u32 person, u32 output)
{
    u8 scratch[512];
    u32 filename = (u32)scratch, extension = (u32)scratch + 256;
    u32 line, index, value, count, base;
    char *version;
    if (output) D(output) = 0;
    if (!person) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        if (!person) {
            filename = (u32)scratch + 256; extension = (u32)scratch;
            line = 0x4b4;
            goto error;
        }
    }
    value = D(person);
    count = D(0x00ae2364);
    index = value - count + 16u;
    if ((s32)index < 0) { line = 0x4c0; goto error; }
    count = D(self + 4) - 1u;
    if ((s32)index > (s32)count) { line = 0x4c0; goto error; }
    value = D(person + 0x24);
    if (value) {
        value = D(value + 0x53);
        if ((s32)D(value + 0x88) < 400) {
            if (output) D(output) = 1;
            return 0;
        }
    }
    base = D(self);
    return B(base + index * 42735u + 0x9a06) != 0;
error:
    cm_fn_00944cff(0x009cf11c, 0, 0, (void *)filename, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
