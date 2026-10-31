/* Preferred-image VA 00452fe0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_00452fe0(u32 record)
{
    u8 work[0x200];
    u32 version;
    if (record == 0) {
        cm_fn_00944cff(0x0098b6a0u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x17edu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    D(record + 0x1f1u) = 0xbf800000u;
    D(record + 1u) = 0xffffffffu;
    D(record + 8u) = 0xffffffffu;
    D(record + 14u) = 0xffffffffu;
    D(record + 0x31u) = 0xffffffffu;
    D(record + 0x54u) = 0xffffffffu;
    D(record + 0x77u) = 0xffffffffu;
    D(record + 0x85u) = 0xffffffffu;
    D(record + 0xa4u) = 0xffffffffu;
    D(record + 0xc3u) = 0xffffffffu;
    D(record + 0xe2u) = 0xffffffffu;
    D(record + 0x101u) = 0xffffffffu;
    D(record + 0x118u) = 0xffffffffu;
    D(record + 0x12cu) = 0xffffffffu;
    D(record + 0x140u) = 0xffffffffu;
    D(record + 0x154u) = 0xffffffffu;
    B(record + 0x168u) = 0xff;
    B(record + 0x183u) = 0xff;
    B(record + 0x19eu) = 0xff;
    B(record + 0x1d6u) = 0xff;
    B(record + 0x210u) = 0xff;
    B(record + 0x22bu) = 0xff;
    W(record + 0x247u) = 0xffff;
    W(record + 0x261u) = 0xffff;
    D(record + 0x2a3u) = 0xffffffffu;
    D(record + 0x2e2u) = 0xffffffffu;
    W(record + 0x2f9u) = 0xffff;
    D(record + 0x313u) = 0xffffffffu;
    D(record + 0x33cu) = 0xffffffffu;
    D(record + 0x365u) = 0xffffffffu;
    D(record + 0x36bu) = 0xffffffffu;
    D(record + 0x37du) = 0xffffffffu;
    return 1;
}
