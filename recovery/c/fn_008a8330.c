/* Preferred-image VA 008a8330, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0053b640(u32 club, u8 kind, u32 first, u32 second);
extern u32 CM_CDECL cm_fn_0053b900(u32 input);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008a8330(u32 self, u32 destination, volatile s32 capacity,
                              volatile u32 first, volatile u32 second)
{
    u8 work[0x200];
    u32 a, b, club, format, version;
    s32 count;
    u8 kind;
    if (destination == 0 || capacity < 100)
        goto diagnostic;
    a = first;
    if (a == 0)
        goto diagnostic;
    b = second;
    if (b == 0)
        goto diagnostic;
    kind = B(self + 0xdebu);
    club = D(self);
    cm_fn_0053b640(club, kind, a, b);
    if (D(self + 0x14u) != 0) {
        format = 0x00a8c558u;
    } else if (D(self + 8u) != 0) {
        club = D(self);
        if (cm_fn_0053b900(club) == 0)
            format = 0x00a8c530u;
        else
            format = 0x00a8c510u;
    } else {
        format = 0x00a8c510u;
    }
    club = D(self);
    count = (s8)B(club + 0x37u);
    cm_fn_0066f4e0(destination, format, count, count, club + 4u);
    return 1;
diagnostic:
    cm_fn_00944cff(0x00a8b94cu, 0, 0, work + 0x100, work);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x1452u);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
