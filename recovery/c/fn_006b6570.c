#include "cmre.h"
/* Preferred-image VA 0x006b6570, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve the reporter's upper EAX bits on the default return. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

u32 CM_CDECL cm_fn_006b6570(s16 code, u8 flag)
{
    u8 scratch[512];
    u32 eax;
    char *version;
    switch (code) {
    case 0: return flag == 1 ? 5 : 4;
    case 1: return flag == 1 ? 11 : 10;
    case 0x1ef: return flag == 1;
    case 0x3de: return flag == 1 ? 3 : 2;
    case 0x483: return flag == 1 ? 7 : 6;
    case 0x528: return flag == 1 ? 9 : 8;
    default: break;
    }
    cm_fn_00944cff(0x00a15708, 0, 0, scratch + 256, scratch);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 256, scratch, 0x913);
    eax = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return eax | 0xffu;
}
