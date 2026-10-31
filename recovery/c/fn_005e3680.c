/* Preferred-image VA 0x005E3680, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl, ret 0 (the caller pops). Five stack arguments: arg1, arg2,
   arg3, arg4 (signed dwords) and arg5, a pointer to four dwords of output.
   Globals: int at 0x00AEBE0C (any non-zero value returns 0 at once), int at
   0x00AEBE08 and int at 0x00AEBDD0 (upper bounds, each minus one).
   A null arg5 returns 0. Otherwise it writes four dwords, in order:
     out[0] = min(arg1, arg3)   out[1] = min(arg2, arg4)
     out[2] = max(arg1, arg3)   out[3] = max(arg2, arg4)
   then clamps: out[0] = 0 when min(arg1, arg3) < 0; out[1] = 0 when
   min(arg2, arg4) < 0; out[2] = (0x00AEBE08 - 1) when max(arg1, arg3) is above
   it; out[3] = (0x00AEBDD0 - 1) when max(arg2, arg4) is above it. Any clamp
   makes the result 0; otherwise the result is 1.
   The listing keeps the result 1 in a stack slot (written before the pushes
   and reloaded at the end), so it is a local here. Stores are volatile so
   -O2 keeps the listing's order at a fault. Field meanings remain unknown. */
#include "cmre.h"

u32 CM_CDECL cm_fn_005e3680(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 *arg5)
{
    volatile s32 *out;
    s32 flag;
    s32 lo13;
    s32 lo24;
    s32 hi13;
    s32 hi24;
    s32 bound;

    if (CM_GLOBAL(s32, 0x00aebe0c) != 0) {
        return 0;
    }
    if (arg5 == 0) {
        return 0;
    }

    flag = 1;
    out = (volatile s32 *)arg5;
    lo13 = (arg1 < arg3) ? arg1 : arg3;
    lo24 = (arg2 < arg4) ? arg2 : arg4;
    hi13 = (arg1 > arg3) ? arg1 : arg3;
    hi24 = (arg2 > arg4) ? arg2 : arg4;

    out[0] = lo13;
    out[1] = lo24;
    out[2] = hi13;
    out[3] = hi24;

    if (lo13 < 0) {
        out[0] = 0;
        flag = 0;
    }
    if (lo24 < 0) {
        out[1] = 0;
        flag = 0;
    }
    bound = CM_GLOBAL(s32, 0x00aebe08) - 1;
    if (hi13 > bound) {
        out[2] = bound;
        flag = 0;
    }
    bound = CM_GLOBAL(s32, 0x00aebdd0) - 1;
    if (hi24 > bound) {
        out[3] = bound;
        flag = 0;
    }
    return (u32)flag;
}
