#include "cmre.h"

extern u32 CM_STDCALL cm_fn_00944c40(u32 socket, u32 address, u32 address_length);
extern u32 CM_STDCALL cm_fn_00944c3a(u32 socket);

u16 CM_THISCALL cm_fn_008ae840(u32 *self)
{
    s32 found = -1;
    if (*(volatile u32 *)self == 0)
        return 0xffffu;

    s16 outer = 0;
    while (outer < 0x14) {
        u32 socket = *(volatile u32 *)((u32)self + 0x2d8u + (u32)(s32)outer * 4u);
        if (socket != 0xffffffffu) {
            u32 address = cm_fn_00944c40(socket, 0, 0);
            if (address != 0xffffffffu) {
                s16 count = *(volatile s16 *)((u8 *)self + 4u);
                s16 index = 0;
                if (count > 0 && found == -1) {
                    while (index < count && found == -1) {
                        u32 entries = *(volatile u32 *)self;
                        s32 entry = *(volatile s32 *)(entries + (u32)(s32)index * 4u);
                        if (entry == -1)
                            found = index;
                        index++;
                    }
                }
                if (index < count) {
                    u32 entries = *(volatile u32 *)self;
                    *(volatile u32 *)(entries + (u32)(s32)found * 4u) = address;
                    return (u16)found;
                }
                (void)cm_fn_00944c3a(address);
            }
        }
        outer++;
    }
    return 0xffffu;
}
