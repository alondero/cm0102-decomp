/* Preferred-image VA 0x00669950, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u8 CM_CDECL cm_fn_00669360(u32 club);
u32 CM_CDECL __attribute__((regparm(3))) cm_fn_00669950(
    u32 incoming_eax, u32 incoming_edx, u32 incoming_ecx, u32 club)
{
    u16 day = CM_GLOBAL(volatile u16, 0x00ae2c92u);
    u32 eax = incoming_eax, group, record, marker;
    (void)incoming_edx;
    if (!club) goto fallback;
    eax = cm_fn_0053b900((u32 *)club);
    if (eax) goto fallback;
    group = (u32)(s32)(s8)cm_fn_00669360(club);
    marker = CM_GLOBAL(volatile u32, 0x00b63d77u + group * 72u);
    record = 0x00b63d60u + group * 72u;
    eax = record;
    if (!marker && CM_GLOBAL(volatile u16, record + 0x15u) >= 0x3cu) goto fallback;
    if ((s32)(s16)CM_GLOBAL(volatile u16, 0x00ae2c90u) >=
        (s32)CM_GLOBAL(volatile u16, record + 0x15u)) goto fallback;
    return ((incoming_ecx & 0xffff0000u) | day) + 0xffffu;
fallback:
    return (eax & 0xffff0000u) | day;
}
