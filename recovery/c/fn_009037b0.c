#include "cmre.h"

extern u32 CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 arg2, u32 arg3,
                                  void *output_a, void *output_b);
extern u32 cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 format, u32 message, u32 mode);

void CM_THISCALL cm_fn_009037b0(u32 self, const u32 *value_pointer)
{
    volatile u8 scratch[0x200];
    volatile u8 *self_bytes = (volatile u8 *)self;
    u32 list;
    u32 allocation;
    u32 format_text;
    u8 count;
    u32 value;

    list = *(volatile u32 *)(self + 0x43b);
    if (list == 0) {
        self_bytes[0x43f] = 0;
        allocation = cm_fn_00944e46(0x78);
        *(volatile u32 *)(self + 0x43b) = allocation;
        if (allocation == 0) {
            (void)cm_fn_00944cff(0x00aa1a1c, 0, 0,
                                 (void *)(scratch + 0x100),
                                 (void *)(scratch + 0));
            format_text = cm_fn_0090d130();
            (void)cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                                 format_text, (u32)(scratch + 0x100),
                                 (u32)(scratch + 0), 0xf9);
            cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
            CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
            return;
        }
    }

    count = self_bytes[0x43f];
    if ((s8)count >= 0x1e)
        return;

    list = *(volatile u32 *)(self + 0x43b);
    value = *value_pointer;
    *(volatile u32 *)(list + (u32)(s32)(s8)count * 4u) = value;

    count = self_bytes[0x43f];
    self_bytes[0x43f] = (u8)(count + 1u);
}
