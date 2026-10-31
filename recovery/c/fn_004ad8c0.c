#include "cmre.h"
/* Preferred-image VA 0x004ad8c0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_CDECL cm_fn_004ad8c0(u32 object)
{
    u8 scratch[512];
    char *version;
    if (!object) {
        cm_fn_00944cff(0x0099ca94, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 256, scratch, 0x1a84);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    D(object + 0x189) = 0xbf800000u;
    D(object + 1) = 0xffffffffu;
    B(object) = 0xff;
    W(object + 5) = 0xffff;
    W(object + 0xd) = 0xffff;
    W(object + 0x15) = 0xffff;
    B(object + 0x1d) = 0xff;
    D(object + 0x25) = 0xffffffffu;
    D(object + 0x48) = 0xffffffffu;
    D(object + 0x6b) = 0xffffffffu;
    D(object + 0x7d) = 0xffffffffu;
    D(object + 0x9c) = 0xffffffffu;
    D(object + 0xb3) = 0xffffffffu;
    D(object + 0xcb) = 0xffffffffu;
    D(object + 0xe3) = 0xffffffffu;
    D(object + 0xfb) = 0xffffffffu;
    B(object + 0x113) = 0xff;
    B(object + 0x132) = 0xff;
    B(object + 0x16a) = 0xff;
    B(object + 0x1ac) = 0xff;
    B(object + 0x1cb) = 0xff;
    D(object + 0x213) = 0xffffffffu;
    D(object + 0x252) = 0xffffffffu;
    return 1;
}
