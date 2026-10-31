#include "cmre.h"

extern void CM_CDECL cm_fn_005a8f60(void);
extern void CM_CDECL cm_fn_005cc7d0(void);
extern void CM_CDECL cm_fn_005e8290(u32 string, u32 destination, u32 flag);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 arg1, u32 arg2,
                                   void *arg3, void *arg4);
extern void CM_CDECL cm_fn_00945934(u32 param);

u32 CM_THISCALL cm_fn_007fec60(u32 self, u32 optional)
{
    s32 index = *(volatile s16 *)(self + 0x382au);
    u32 offset = (u32)index * 3u;
    offset <<= 8;
    u32 target = *(volatile u32 *)(self + offset + 0x7e2u);

    if (target == 0) {
        volatile u8 scratch[0x200];
        u32 scratch_base = (u32)scratch;
        (void)cm_fn_00944cff(0x00a7ddbcu, 0, 0,
                             (void *)(scratch_base + 0x100u),
                             (void *)scratch_base);

        u32 diagnostic = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, diagnostic,
                             scratch_base + 0x100u, scratch_base, 0x5e1u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);

        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        cm_fn_005cc7d0();
        cm_fn_005a8f60();
        cm_fn_00945934(0xffffffffu);
    }

    u32 active = *(volatile u32 *)(target + 0x1f4u);
    if (active == 0)
        return 1;

    if (optional != 0)
        return 0;

    u32 nested = *(volatile u32 *)(target + 0x10u);
    if (nested != 0)
        return 0;
    return 1;
}
