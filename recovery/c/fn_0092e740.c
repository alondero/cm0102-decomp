#include "cmre.h"
extern void CM_CDECL cm_fn_005e8290(const char *message, const char *body, int type);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

void CM_THISCALL cm_fn_0092e740(u8 *self, u32 parameter)
{
    s16 low = *(volatile s16 *)(self + 0x36u);
    s16 high = *(volatile s16 *)(self + 0x38u);
    if (low >= high) {
        u8 scratch[0x200];
        (void)cm_fn_00944cff(0x00ad5888u, 0, 0,
            (void *)(scratch + 0x100), scratch);
        char *label = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
            (u32)label, (u32)(scratch + 0x100), (u32)scratch, 0x456u);
        cm_fn_005e8290((const char *)0x009870e0u,
            (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return;
    }
    u32 index = (u32)(s32)low;
    u32 array = *(volatile u32 *)(self + 0x14u);
    *(volatile u8 *)(array + index * 6u + 4u) = 6;
    index = (u32)(s32)*(volatile s16 *)(self + 0x36u);
    array = *(volatile u32 *)(self + 0x14u);
    *(volatile u32 *)(array + index * 6u) = parameter;
    low = *(volatile s16 *)(self + 0x36u);
    *(volatile s16 *)(self + 0x36u) = (s16)((u16)low + 1u);
}
