#include "cmre.h"

/* Preferred-image VA 006306E0, image base 00400000; CM0102 3.9.68
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL one DWORD, RET4. Full EAX is retained, including partial AL writes.
   The pinned .text tables contain 77 DWORDs each and are read-only in the
   original image. The actual unmodified-image M1 inputs cannot rewrite them.
   Each guarded original table read remains an ordered volatile DWORD read;
   dispatch depends on the value read, rather than discarding that value.
   The five labels below cover every actual slot in all three pinned tables.
   Arbitrary host-forced code/table patches outside that domain are unproved. */
extern u32 CM_CDECL cm_fn_0066f4d0(void);

u32 CM_STDCALL cm_fn_006306e0(volatile u32 argument)
{
    u32 eax, table, target;

    eax = cm_fn_0066f4d0(); /* 006306E0: original full EAX, no arguments. */
    if (eax == 9u) {
        eax = (u32)(s32)(s8)argument; /* 006306EA: sign-extend low BYTE. */
        if (eax > 0x4cu) goto return_one; /* unsigned JA at 006306F2. */
        table = 0x0063074cu;
        goto dispatch;
    }

    eax = cm_fn_0066f4d0(); /* 00630700, only after the first comparison. */
    if (eax == 3u) {
        eax = (u32)(s32)(s8)argument; /* 0063070A. */
        if (eax > 0x4cu) goto return_one; /* 00630712. */
        table = 0x00630880u;
        goto dispatch;
    }

    eax = cm_fn_0066f4d0(); /* 00630725, only after both comparisons. */
    if (eax != 5u) goto return_zero; /* 0063072D. */
    eax = (u32)(s32)(s8)argument; /* 0063072F. */
    if (eax > 0x4cu) goto return_one; /* 00630737. */
    table = 0x006309b4u;

dispatch:
    /* Original indexed JMP sites 006306F4 / 00630714 / 00630739.
       The preceding unsigned guard admits exactly the 77 actual slots. */
    target = CM_GLOBAL(volatile u32, table + eax * 4u);
    switch (target) {
    case 0x006306fbu: /* MOV AL,3; RET4. */
        return (eax & 0xffffff00u) | 3u;
    case 0x0063071bu: /* MOV AL,2; RET4. */
        return (eax & 0xffffff00u) | 2u;
    case 0x00630720u: /* MOV AL,4; RET4. */
        return (eax & 0xffffff00u) | 4u;
    case 0x00630740u:
        goto return_one;
    case 0x00630745u:
        goto return_zero;
    default:
        /* Unreachable for every pinned slot and unchanged supported baseline.
           This is not a fallback for altered original code or table bytes. */
        __builtin_unreachable();
    }

return_one: /* 00630740: MOV AL,1; RET4. */
    return (eax & 0xffffff00u) | 1u;
return_zero: /* 00630745: XOR AL,AL; RET4. */
    return eax & 0xffffff00u;
}
