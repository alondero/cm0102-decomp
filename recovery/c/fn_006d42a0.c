#include "cmre.h"

extern u32 CM_CDECL cm_fn_00600a50(int first, int second, u32 third,
                                    int *fourth, int fifth);

u32 CM_THISCALL cm_fn_006d42a0(const volatile u8 *self, u8 selector)
{
    u32 object = *(const volatile u32 *)(self + 0x4792);
    u8 state = *(volatile u8 *)(u32)(object + 0x3a);
    if (state == 1 || state == 9) return object & 0xffffff00u;

    u32 selected = selector != 0
        ? *(volatile u32 *)(u32)(object + 0x20)
        : *(volatile u32 *)(u32)(object + 0x1c);
    u8 helper_byte = *(volatile u8 *)(u32)(object + 0x40);
    u32 helper_arg3 = (object & 0xffffff00u) | helper_byte;
    u32 helper_result = cm_fn_00600a50((int)selected, 0, helper_arg3,
                                       0, 0);

    u32 eax = (u32)(s32)(s8)selector;
    if (helper_result != 0) {
        u32 row_offset = (u32)(s32)(s8)selector * 0x18e3u;
        u32 address = (u32)self + row_offset + 0x9766u;
        u32 flags = *(volatile u32 *)(u32)address;
        if ((flags & 0x4000u) != 0) return (flags & 0xffffff00u) | 1u;
        return flags & 0xffffff00u;
    }

    u32 address = (u32)self + (u32)(s32)(s8)selector + 0x1abu;
    u8 code = *(volatile u8 *)(u32)address;
    if (code == 3 || code == 4) return (eax & 0xffffff00u) | 1u;
    return eax & 0xffffff00u;
}
