#include "cmre.h"
/* Preferred-image VA 0x00859470, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The last row stores bytes four and five before bytes zero through three. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_FASTCALL cm_fn_00859470(u32 self)
{
    u8 scratch[512];
    u32 pointer, eax;
    char *version;
    B(self + 8) = 2;
    pointer = (u32)cm_fn_00944e46(24);
    D(self + 4) = pointer;
    if (!pointer) {
        cm_fn_00944cff(0x00a82ac8, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 256, scratch, 0x54);
        eax = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return eax;
    }
    B(pointer) = 22;
    B(pointer + 1) = 0;
    B(pointer + 2) = 3;
    B(pointer + 3) = 17;
    B(pointer + 4) = 5;
    B(pointer + 5) = 1;
    pointer = D(self + 4) + 6;
    B(pointer) = 22;
    B(pointer + 1) = 0;
    B(pointer + 2) = 4;
    B(pointer + 3) = 20;
    B(pointer + 4) = 7;
    B(pointer + 5) = 0;
    pointer = D(self + 4);
    B(pointer + 12) = 22;
    pointer += 12;
    B(pointer + 1) = 1;
    B(pointer + 2) = 0xff;
    B(pointer + 3) = 15;
    B(pointer + 4) = 11;
    B(pointer + 5) = 1;
    pointer = D(self + 4) + 18;
    B(pointer + 4) = 0;
    B(pointer + 5) = 0;
    B(pointer) = 22;
    B(pointer + 1) = 1;
    B(pointer + 2) = 0xff;
    B(pointer + 3) = 31;
    return pointer;
}
