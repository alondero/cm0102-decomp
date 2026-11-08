/* Preferred-image VA 0x00543280, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve full native EAX: each exit overwrites AL only. The helper boundary
   below intentionally captures all of EAX, including its incidental high bits.
   Signed remainders reproduce the original AND/sign-correction sequences. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_005afde0(u32 base, u32 *argument);
extern u32 CM_THISCALL cm_fn_005b02f0(u32 self, u32 argument);

u32 CM_CDECL cm_fn_00543280(u32 first, u32 second, u8 code)
{
    u32 eax = first;
    u32 context;
    u8 status;
    s32 residue;
    s32 value;
    s32 divisor;

    if (eax == 0u)
        goto yes;
    if (second == 0u)
        goto yes;
    context = CM_GLOBAL(volatile u32, 0x00ae2a60);
    if (context == 0u)
        goto yes;
    if (CM_GLOBAL(volatile u8, 0x009cc270) == 0u)
        goto yes;
    eax = cm_fn_005afde0(context, (u32 *)first);
    if (eax == 0u)
        goto yes;
    eax = cm_fn_005b02f0(eax, second);
    status = (u8)eax;
    if (status == 3u)
        goto yes;
    if (status != 2u && status != 1u)
        goto no;
    if (code == 1u || code == 0x11u || code == 0x15u ||
        code == 0x1au || code == 0x1bu)
        goto yes;

    residue = *(volatile s32 *)second % 4;
    value = (s32)(s8)code;
    divisor = status == 2u ? 2 : 4;
    if (residue == 0) {
        eax = (u32)value;
        if (value % divisor == 0)
            goto yes;
        ++value;
    } else {
        if (residue == 1)
            ++value;
        else if (residue == 2)
            value += 2;
        else
            value += 3;
        eax = (u32)value;
        if (value % divisor == 0)
            goto yes;
    }
    eax = (u32)(value / 3);
    if (value % 3 == 0)
        goto yes;
no:
    return eax & 0xffffff00u;
yes:
    return (eax & 0xffffff00u) | 1u;
}
