#include "cmre.h"

/* Preferred-image VA 0x00600740, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The search returns a full DWORD boolean; a missing key/person returns zero
   without reporting. Preserve the byte-before-DWORD search-key stores. */
extern u32 CM_CDECL cm_fn_00946464(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_00600740(u32 self, u32 key, u32 person)
{
    u8 scratch[0x220];
    u32 work = (u32)scratch, index, value, count, base, array;
    char *version;
    if (!key) return 0;
    if (!person) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        if (!person) return 0;
    }
    value = D(person);
    count = D(0x00ae2364);
    index = value - count + 16u;
    if ((s32)index < 0) goto error;
    count = D(self + 4) - 1u;
    if ((s32)index > (s32)count) goto error;
    base = D(self);
    base += index * 42735u;
    value = D(base + 0xa6e3);
    if (!value) return 0;
    value = D(key);
    B(work + 4) = 0;
    D(work) = value;
    count = D(base + 0xa6eb);
    array = D(base + 0xa6e3);
    return cm_fn_00946464(work, array, count, 30, 0x00601a10) != 0;
error:
    cm_fn_00944cff(0x009cf11c, 0, 0, scratch + 0x120, scratch + 0x20);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                  scratch + 0x120, scratch + 0x20, 0xa96);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
