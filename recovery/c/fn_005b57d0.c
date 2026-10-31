#include "cmre.h"

extern void CM_CDECL cm_fn_005e8290(const char *message,
                                    const char *buffer, u32 flags);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 message, u32 arg2, u32 arg3,
                                   void *buffer1, void *buffer2);

u32 CM_THISCALL cm_fn_005b57d0(void *self, u32 value, u32 selector)
{
    volatile u8 scratch[0x200];
    u32 self_address = (u32)self;
    s32 first_index = (s32)(s8)(u8)value;
    s32 dispatch_index = (s32)(s8)(u8)selector - 1;

    if ((u32)dispatch_index > 15u) {
        void *buffer0 = (void *)&scratch[0];
        void *buffer100 = (void *)&scratch[0x100];
        u32 formatted;

        (void)cm_fn_00944cff(0x009cc3dcu, 0u, 0u, buffer100, buffer0);
        formatted = (u32)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                             formatted, buffer100, buffer0, 0x7bfu);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0u);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return 0u;
    }

    {
        u8 mode = *(volatile u8 *)(0x005b58c4u + (u32)dispatch_index);
        u32 target = *(volatile u32 *)(0x005b58b8u + (u32)mode * 4u);
        if (target == 0x005b57f5u) {
            u32 field = self_address + 0x5b4u + (u32)first_index;
            u8 old_value = *(volatile u8 *)field;
            u8 new_value = (u8)((old_value & 0xf8u) | (u8)selector);
            *(volatile u8 *)field = new_value;
            *(volatile u32 *)(self_address + 0x187fu) = 0u;
            return 1u;
        }
        if (target == 0x005b582bu) {
            u32 field = self_address + 0x5b4u + (u32)first_index;
            u8 old_value = *(volatile u8 *)field;
            u8 new_value = (u8)((old_value & 0xe7u) | (u8)selector);
            *(volatile u8 *)field = new_value;
            *(volatile u32 *)(self_address + 0x187fu) = 0u;
            return 1u;
        }
    }

    {
        void *buffer0 = (void *)&scratch[0];
        void *buffer100 = (void *)&scratch[0x100];
        u32 formatted;
        (void)cm_fn_00944cff(0x009cc3dcu, 0u, 0u, buffer100, buffer0);
        formatted = (u32)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                             formatted, buffer100, buffer0, 0x7bfu);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0u);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    }
    return 0u;
}
