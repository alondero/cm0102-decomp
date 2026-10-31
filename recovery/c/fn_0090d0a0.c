#include "cmre.h"

extern u32 CM_CDECL cm_fn_00946ebb(u32 *output);
extern void CM_CDECL cm_fn_009469fa(u32 value);
extern u32 CM_CDECL cm_fn_00946a04(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

u32 CM_CDECL cm_fn_0090d0a0(u32 seed)
{
    s32 random_value;
    s32 remainder;
    u32 old_pointer;

    if (seed == 0)
        seed = cm_fn_00946ebb(0);
    cm_fn_009469fa(seed);
    cm_fn_00944c9f(0x00de1f64u, 0x00ad437cu, seed);

    random_value = (s32)cm_fn_00946a04();
    remainder = random_value % 51000;
    CM_GLOBAL(volatile u32, 0x00de1f60) = 0x00aa2454u + (u32)remainder * 4u;

    random_value = (s32)cm_fn_00946a04();
    CM_GLOBAL(volatile u32, 0x00de2798) = 0x00ad4130u;
    remainder = random_value % 0xffff;
    old_pointer = CM_GLOBAL(volatile u32, 0x00de1f60);
    CM_GLOBAL(volatile s32, 0x00de1f5c) = remainder;
    if (old_pointer < 0x00aa2454u || old_pointer > 0x00ad4130u)
        CM_GLOBAL(volatile u32, 0x00de1f60) = 0x00aa2454u;
    return old_pointer;
}
