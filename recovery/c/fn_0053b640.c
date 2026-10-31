#include "cmre.h"
/* Preferred-image VA 0x0053b640, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_005e4800(u32, u32, u32, u32 *);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

u32 CM_CDECL cm_fn_0053b640(u32 club, u8 kind, u32 first, u32 second)
{
    u8 scratch[0x20c];
    u32 a, b, c, primary, secondary;
    u8 x, y, z;
    u16 initial;
    char *version;
    initial = CM_GLOBAL(volatile u16, 0x00aebdf0);
    W(first) = initial;
    initial = CM_GLOBAL(volatile u16, 0x00ae3184);
    W(second) = initial;
    if (club == 0) {
        cm_fn_00944cff(0x009b5ac8, 0, 0, scratch + 0x10c, scratch + 0xc);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                      version, scratch + 0x10c, scratch + 0xc, 0x3410);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return 0;
    }
    if (kind == 1) {
        primary = D(club + 0x83u);
        secondary = D(club + 0x87u);
    } else if (kind == 2 || kind == 3) {
        u32 offset = kind == 2 ? 0x8bu : 0x93u;
        primary = D(club + offset);
        secondary = D(club + offset + 4u);
        if (primary == 0 || secondary == 0) {
            primary = D(club + 0x87u);
            secondary = D(club + 0x83u);
        }
    } else {
        return 1;
    }
    /* Only the low bytes of these three stack DWORDs are initialized by the
       original. Retain the full pushed values at the callee boundary. */
    if (primary != 0) {
        x = B(primary + 0x37u);
        y = B(primary + 0x38u);
        z = B(primary + 0x39u);
        scratch[0] = x;
        scratch[4] = z;
        a = D((u32)scratch);
        c = D((u32)(scratch + 4));
        scratch[8] = y;
        b = D((u32)(scratch + 8));
        W(first) = (u16)cm_fn_005e4800(a, b, c, (u32 *)0);
    }
    if (secondary != 0) {
        z = B(secondary + 0x39u);
        x = B(secondary + 0x37u);
        y = B(secondary + 0x38u);
        scratch[4] = z;
        scratch[0] = x;
        c = D((u32)(scratch + 4));
        a = D((u32)scratch);
        scratch[8] = y;
        b = D((u32)(scratch + 8));
        W(second) = (u16)cm_fn_005e4800(a, b, c, (u32 *)0);
    }
    return 1;
}
