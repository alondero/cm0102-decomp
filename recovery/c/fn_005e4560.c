#include "cmre.h"

extern u32 CM_CDECL cm_fn_005e4800(u32 arg1, u32 arg2, u32 arg3,
                                   u32 *arg4);

void CM_CDECL cm_fn_005e4560(void)
{
    u32 year;
    u32 mask;

    if (CM_GLOBAL(volatile u32, 0x00aebe0cu) != 0)
        return;

    year = CM_GLOBAL(volatile u32, 0x00ae316cu);
    mask = 0u - (u32)(year != 0x7e0u);
    CM_GLOBAL(volatile u16, 0x00aebdeau) = 0;
    CM_GLOBAL(volatile u16, 0x00ae31a2u) =
        (u16)(((mask & 0xffff8000u) + 0xffffffffu));
    CM_GLOBAL(volatile u16, 0x00ae3184u) =
        (u16)((mask & 0xffff8c80u) + 0x0000e71cu);
    CM_GLOBAL(volatile u16, 0x00ae31a8u) =
        (u16)((mask & 0xffff8400u) + 0x0000f800u);
    CM_GLOBAL(volatile u16, 0x00aebdccu) =
        (u16)((mask & 0xffffc000u) + 0x00008000u);
    CM_GLOBAL(volatile u16, 0x00aebdd4u) =
        (u16)((mask & 0xffff8200u) + 0x0000fc00u);
    CM_GLOBAL(volatile u16, 0x00ae3190u) =
        (u16)((mask & 0xffffbf00u) + 0x00008200u);
    CM_GLOBAL(volatile u16, 0x00aebdecu) =
        (u16)((mask & 0xffff8000u) + 0x0000ffe0u);
    CM_GLOBAL(volatile u16, 0x00aebdeeu) =
        (u16)((mask & 0xffff8000u) + 0x0000fff0u);

    CM_GLOBAL(volatile u16, 0x00ae31aau) =
        (u16)cm_fn_005e4800(0, 0x80, 0, 0);
    CM_GLOBAL(volatile u16, 0x00ae3150u) =
        (u16)cm_fn_005e4800(0, 0x40, 0, 0);
    CM_GLOBAL(volatile u16, 0x00ae3152u) =
        (u16)cm_fn_005e4800(0, 0xff, 0xff, 0);
    CM_GLOBAL(volatile u16, 0x00aebde8u) =
        (u16)cm_fn_005e4800(0x80, 0xff, 0xff, 0);
    CM_GLOBAL(volatile u16, 0x00aebdf0u) =
        (u16)cm_fn_005e4800(0, 0, 0xff, 0);
    CM_GLOBAL(volatile u16, 0x00aebddeu) =
        (u16)cm_fn_005e4800(0, 0x80, 0xff, 0);
    CM_GLOBAL(volatile u16, 0x00aebe04u) =
        (u16)cm_fn_005e4800(0, 0, 0x80, 0);
    CM_GLOBAL(volatile u16, 0x00aebdd6u) =
        (u16)cm_fn_005e4800(0, 0, 0x60, 0);
    CM_GLOBAL(volatile u16, 0x00ae3148u) =
        (u16)cm_fn_005e4800(0x80, 0, 0x80, 0);
    CM_GLOBAL(volatile u16, 0x00ae3192u) =
        (u16)cm_fn_005e4800(0x40, 0, 0x40, 0);
    CM_GLOBAL(volatile u16, 0x00ae31a0u) =
        (u16)cm_fn_005e4800(0x80, 0x40, 0x40, 0);
    CM_GLOBAL(volatile u16, 0x00ae317eu) =
        (u16)cm_fn_005e4800(0x80, 0x80, 0x80, 0);
    CM_GLOBAL(volatile u16, 0x00aebddcu) =
        (u16)cm_fn_005e4800(0xe0, 0xe0, 0xe0, 0);
    CM_GLOBAL(volatile u16, 0x00ae3154u) =
        (u16)cm_fn_005e4800(0xc0, 0xc0, 0xc0, 0);
    CM_GLOBAL(volatile u16, 0x00ae317cu) =
        (u16)cm_fn_005e4800(0x40, 0x40, 0x40, 0);
    CM_GLOBAL(volatile u16, 0x00ae3140u) =
        (u16)cm_fn_005e4800(0, 0x80, 0x80, 0);
}
