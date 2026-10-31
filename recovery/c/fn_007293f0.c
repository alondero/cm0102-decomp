/* Preferred-image VA 007293f0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00729ff0(u32 self, u32 match, u32 report, u32 flag);
extern u32 CM_THISCALL cm_fn_00729820(u32 self, u32 club, u32 report, u32 value);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007293f0(u32 self, u32 match, u32 report)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 line, text, club, special, id, value;
    if (match == 0) {
        name = work + 0x100;
        extension = work;
        line = 0xadu;
        goto diagnostic;
    }
    if (report == 0) {
        name = work;
        extension = work + 0x100;
        line = 0xb5u;
        goto diagnostic;
    }
    cm_fn_00729ff0(self, match, report, 0);
    club = D(match + 0x4792u);
    special = D(0x009cf684u);
    id = D(club);
    if (id != special || B(report + 0x4cu) == 0) {
        value = D(match + 0x8f7au);
        cm_fn_00729820(self, club, report, value);
    }
    return 1;

diagnostic:
    cm_fn_00944cff(0x00a17ba0u, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
