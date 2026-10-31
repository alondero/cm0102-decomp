#include "cmre.h"
/* Preferred-image VA 0x0074af60, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern unsigned long long CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_STDCALL cm_fn_0074af60(u32 report, u32 mode, u32 output)
{
    u32 id, amount, base, club, name, format;
    s32 byte;
    id = (u32)cm_fn_00785830(report, 0);
    amount = (u32)cm_fn_00785830(report, 1);
    cm_fn_00785830(report, 2);
    base = D(0x00ae23b4);
    club = base + id * 581u;
    byte = (s8)B(club + 0x52);
    name = club + 0x38;
    if (mode) {
        if ((s32)amount > 6750) format = 0x00a436fc;
        else if ((s32)amount > 3250) format = 0x00a43644;
        else format = 0x00a43594;
    } else {
        format = (s32)amount > 6750 ? 0x00a43560 : 0x00a43528;
    }
    cm_fn_0066f4e0(0x00de1f64, format, byte, byte, name);
    cm_fn_004ae660(output, 0x00de1f64);
    return cm_fn_004ae8a0(output, name, 0x7d5, club);
}
