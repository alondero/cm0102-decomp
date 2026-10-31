#include "cmre.h"

/* Preferred-image VA 0x006005e0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The five-byte search key and error buffers retain their original adjacency.
   The comparator at VA 00601a10 remains original code. */
extern u32 CM_CDECL cm_fn_00946464(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_006005e0(u32 self, u32 key, u32 person)
{
    u8 scratch[0x220];
    u32 work = (u32)scratch, filename = work + 0x20, extension = work + 0x120;
    u32 line, index, value, count, base, array, result;
    char *version;
    if (!key) return 0;
    if (!person) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        if (!person) {
            filename = work + 0x120; extension = work + 0x20;
            line = 0xa4b;
            goto error;
        }
    }
    value = D(person);
    count = D(0x00ae2364);
    index = value - count + 16u;
    if ((s32)index < 0) { line = 0xa57; goto error; }
    count = D(self + 4) - 1u;
    if ((s32)index > (s32)count) { line = 0xa57; goto error; }
    base = D(self);
    base += index * 42735u;
    value = D(base + 0xa6e3);
    if (!value) return 0;
    value = D(key);
    B(work + 4) = 0;
    D(work) = value;
    count = D(base + 0xa6eb);
    array = D(base + 0xa6e3);
    result = cm_fn_00946464(work, array, count, 30, 0x00601a10);
    if (!result) return 0;
    return result + 4u;
error:
    cm_fn_00944cff(0x009cf11c, 0, 0, (void *)filename, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
