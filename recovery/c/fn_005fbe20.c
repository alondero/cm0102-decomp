#include "cmre.h"

/* Preferred-image VA 0x005fbe20, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep repeated helper calls and the different incidental EAX results left
   by the five original store blocks. */
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern u32 CM_CDECL cm_fn_005409b0(u32 *);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_005fbe20(u32 self, u32 flag, u32 club, u32 person)
{
    u8 scratch[512];
    u32 index, value, count, base, row;
    char *version;
    if (!person) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        if (!person) return 0;
    }
    if (!club) club = D(person + 0x39);
    value = D(person);
    count = D(0x00ae2364);
    index = value - count + 16u;
    if ((s32)index < 0) goto error;
    count = D(self + 4) - 1u;
    if ((s32)index > (s32)count) goto error;
    row = index * 42735u;
    if (club && cm_fn_005409b0((u32 *)club)) {
        base = D(self);
        B(base + row + 0x9a69) = 4;
        return base;
    }
    if (flag && club) {
        if (cm_fn_0053b900((u32 *)club)) {
            base = D(self);
            B(base + row + 0x9a69) = 3;
            return index * 8547u;
        }
    }
    if (club && cm_fn_0053b900((u32 *)club)) {
        base = D(self);
        B(base + row + 0x9a69) = 2;
        return row;
    }
    base = D(self);
    if (flag) {
        B(base + row + 0x9a69) = 1;
        return base;
    }
    B(base + row + 0x9a69) = 0;
    return index * 8547u;
error:
    cm_fn_00944cff(0x009cf11c, 0, 0, scratch + 256, scratch);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 256, scratch, 0x167);
    value = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return value;
}
