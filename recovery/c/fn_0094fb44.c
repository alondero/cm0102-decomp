/* Preferred-image VA 0x0094fb44, image base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern void CM_CDECL cm_fn_0094fba8(u8 *out, const u8 *in);
/* 009525ff reads the exponent slot as DWORD, then 0095260e/00952614
   discard its high sixteen bits before any use. The slot is subsequently
   overwritten at 009527e6 before its address is forwarded. */
extern u32 CM_CDECL cm_fn_009525f9(u32 low, u32 high, u16 exponent,
                                  u32 digits, u32 options, u32 output);

u32 CM_CDECL cm_fn_0094fb44(u32 input_low, u32 input_high)
{
    u32 input[2] = { input_low, input_high };
    struct {
        volatile u32 low;
        volatile u32 high;
        volatile u16 exponent;
        u16 unused_padding;
    } converted;
    u32 low, high, status;
    u16 exponent;

    cm_fn_0094fba8((u8 *)&converted, (const u8 *)input);
    /* Original MOVSD/MOVSD/MOVSW copy ten bytes; padding remains unread. */
    low = converted.low;
    high = converted.high;
    exponent = converted.exponent;
    status = cm_fn_009525f9(low, high, exponent, 0x11u, 0, 0x00de3260u);
    CM_GLOBAL(volatile u32, 0x00de3288u) = status;
    CM_GLOBAL(volatile u32, 0x00de3280u) =
        (u32)(s32)CM_GLOBAL(volatile s8, 0x00de3262u);
    CM_GLOBAL(volatile u32, 0x00de3284u) =
        (u32)(s32)CM_GLOBAL(volatile s16, 0x00de3260u);
    CM_GLOBAL(volatile u32, 0x00de328cu) = 0x00de3264u;
    return 0x00de3280u;
}
