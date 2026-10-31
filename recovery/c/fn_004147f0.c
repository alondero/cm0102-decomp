#include "cmre.h"

extern void *CM_CDECL cm_fn_00944e46(u32 size);
extern void *CM_CDECL cm_fn_00945667(void *old_pointer, u32 new_size);
extern u32 CM_CDECL cm_fn_005e8290(const char *message,
                                  const char *buffer, u32 flags);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 message, u32 arg2, u32 arg3,
                                   void *buffer1, void *buffer2);

u32 CM_THISCALL cm_fn_004147f0(void *self, u32 item)
{
    u32 self_address = (u32)self;
    volatile u8 scratch[0x200];
    s16 capacity = *(volatile s16 *)(self_address + 0x1au);
    s16 count = *(volatile s16 *)(self_address + 0x18u);

    if (count >= capacity) {
        u16 new_capacity = (u16)((u16)capacity + 5u);
        s32 signed_capacity = (s32)(s16)new_capacity;
        u32 old_pointer = *(volatile u32 *)(self_address + 0x14u);
        u32 new_pointer;

        *(volatile s16 *)(self_address + 0x1au) = (s16)new_capacity;
        if (old_pointer == 0u)
            new_pointer = (u32)cm_fn_00944e46((u32)signed_capacity * 4u);
        else
            new_pointer = (u32)cm_fn_00945667(
                (void *)old_pointer, (u32)signed_capacity * 4u);
        *(volatile u32 *)(self_address + 0x14u) = new_pointer;
        if (new_pointer == 0u) {
            void *buffer0 = (void *)&scratch[0];
            void *buffer100 = (void *)&scratch[0x100];
            u32 formatted;
            u32 diagnostic_result;

            (void)cm_fn_00944cff(0x00987e24u, 0u, 0u,
                                 buffer100, buffer0);
            formatted = (u32)cm_fn_0090d130();
            (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                                 formatted, buffer100, buffer0, 0x39au);
            diagnostic_result = (u32)cm_fn_005e8290(
                (const char *)0x009870e0u,
                (const char *)0x00ae24d0u, 0u);
            CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
            return (diagnostic_result & 0xffff0000u) | 0xffffu;
        }
    }

    {
        s32 append_index = (s32)*(volatile s16 *)(self_address + 0x18u);
        u32 array = *(volatile u32 *)(self_address + 0x14u);
        *(volatile u32 *)(array + (u32)append_index * 4u) = item;
        {
            u16 current_count = (u16)*(volatile s16 *)(self_address + 0x18u);
            *(volatile s16 *)(self_address + 0x18u) =
                (s16)(u16)(current_count + 1u);
        }
        {
            u16 new_count = (u16)*(volatile s16 *)(self_address + 0x18u);
            u32 combined = (self_address & 0xffff0000u) | (u32)new_count;
            return combined - 1u;
        }
    }
}
