#include "cmre.h"

extern void CM_CDECL cm_fn_00946da0(u32 destination, u32 size);
extern void CM_CDECL cm_fn_00945b13(u32 output, u32 drive, u32 directory,
                                   u32 name, u32 extension);
extern u32 CM_CDECL cm_fn_0090bf90(u32 output);
extern u32 CM_CDECL cm_fn_00945bd2(u32 name);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                  u32 buffer1, u32 buffer2);
extern char *CM_CDECL cm_fn_0090d130(void);
extern void CM_CDECL cm_fn_005e8290(const char *message, const char *buffer,
                                   int mode);

u32 CM_CDECL cm_fn_0090bea0(u32 name, u32 output)
{
    volatile u8 scratch[0x304];
    u32 result;

    cm_fn_00946da0((u32)(unsigned long)&scratch[0], 0x104u);
    cm_fn_00945b13(output, 0u, (u32)(unsigned long)&scratch[0], name, 0u);
    result = cm_fn_0090bf90(output);
    if (result != 0u)
        return 1u;
    result = cm_fn_00945bd2(output);
    if (result == 0u)
        return 1u;

    (void)cm_fn_00944c9f((u32)(unsigned long)&scratch[0], 0x00ad419cu, name);
    (void)cm_fn_00944cff(0x00ad4134u, 0u, 0u,
                         (u32)(unsigned long)&scratch[0x104],
                         (u32)(unsigned long)&scratch[0x204]);
    result = (u32)(unsigned long)cm_fn_0090d130();
    (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, result,
                         (u32)(unsigned long)&scratch[0x104],
                         (u32)(unsigned long)&scratch[0x204], 0x28cu);
    cm_fn_005e8290((const char *)0x009870e0u,
                   (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    return 0u;
}
