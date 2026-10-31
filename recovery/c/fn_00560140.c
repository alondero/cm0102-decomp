#include "cmre.h"

extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                  void *local100, void *local0);

void CM_THISCALL cm_fn_00560140(u8 *self, u8 value)
{
    u32 index = *(volatile u32 *)(self + 4);
    u8 scratch[0x200];
    u32 capacity = *(volatile u32 *)(self + 8);

    if (index + 1u > capacity) {
        (void)cm_fn_00944cff(0x009c3734u, 0, 0, scratch + 0x100, scratch);
        u32 message = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, message,
                             scratch + 0x100, scratch, 0x85u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return;
    }

    u32 buffer = *(volatile u32 *)self;
    *(volatile u8 *)(buffer + index) = value;
    u32 next_index = *(volatile u32 *)(self + 4);
    next_index++;
    *(volatile u32 *)(self + 4) = next_index;
}
