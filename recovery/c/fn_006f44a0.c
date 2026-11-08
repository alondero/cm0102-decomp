/* Preferred-image VA 0x006f44a0, base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original RNG boundary: one stack DWORD; only EAX is consumed here.
   This does not compose the accepted physical EDX:EAX RNG projection. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);

#define B(offset) CM_GLOBAL(volatile s8, self + (offset))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_006f44a0(u32 self)
{
    long double live;
    volatile long long converted;
    u32 bound, first, result;

    live = (long double)CM_GLOBAL(volatile float, self + 0x4du);
    live *= CM_GLOBAL(volatile float, 0x0096d758u);
    converted = (long long)live; /* Native __ftol CALL VA 0x006f44ad. */
    bound = (u32)(s32)(s16)(u32)converted;
    bound += (u32)(s32)B(0x10eu) - 25u;
    first = cm_fn_0090cfc0(bound);

    live = (long double)CM_GLOBAL(volatile float, self + 0x4du);
    live *= CM_GLOBAL(volatile float, 0x0096d758u);
    converted = (long long)live; /* Native __ftol CALL VA 0x006f44d1. */
    bound = (u32)(s32)(s16)(u32)converted;
    bound += (u32)(s32)B(0x10eu) - 25u;
    result = cm_fn_0090cfc0(bound);
    if ((s32)result <= (s32)first) result = first;
    B(0x104u) = (s8)result;

    bound = (u32)(s32)B(0x110u);
    first = cm_fn_0090cfc0(bound);
    bound = (u32)(s32)B(0x110u);
    result = cm_fn_0090cfc0(bound);
    if ((s32)result <= (s32)first) result = first;
    bound = (u32)(s32)B(0x111u);
    B(0x10au) = (s8)result;
    result = cm_fn_0090cfc0(bound);
    B(0x107u) = (s8)result;

    bound = (u32)(s32)B(0x112u);
    result = cm_fn_0090cfc0(bound);
    bound = (u32)(s32)B(0x113u);
    B(0x108u) = (s8)result;
    result = cm_fn_0090cfc0(bound);
    bound = (u32)(s32)B(0x114u);
    B(0x109u) = (s8)result;
    first = cm_fn_0090cfc0(bound);
    bound = (u32)(s32)B(0x114u);
    result = cm_fn_0090cfc0(bound);
    if ((s32)result <= (s32)first) result = first;

    bound = (u32)(s32)B(0x10fu);
    B(0x10bu) = (s8)result;
    first = cm_fn_0090cfc0(bound);
    bound = (u32)(s32)B(0x10fu);
    result = cm_fn_0090cfc0(bound);
    if ((s32)result <= (s32)first) result = first;
    B(0x106u) = (s8)result;

    bound = (u32)(s32)B(0x115u);
    result = cm_fn_0090cfc0(bound);
    bound = (u32)(s32)B(0x116u);
    B(0x105u) = (s8)result;
    result = cm_fn_0090cfc0(bound);
    bound = (u32)(s32)B(0x117u);
    B(0x10du) = (s8)result;
    result = cm_fn_0090cfc0(bound);
    B(0x10cu) = (s8)result;
    return result; /* VA 0x006f45e9 writes memory only; EAX remains whole. */
}
