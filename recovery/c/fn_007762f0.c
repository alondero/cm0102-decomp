#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053b4f0(u32 value);
extern u32 CM_CDECL cm_fn_0053b3d0(u32 value);
extern u32 CM_THISCALL cm_fn_00896c60(u32 *self, u32 first, u32 second,
                                      u32 third, u32 fourth);

u8 CM_STDCALL cm_fn_007762f0(u32 value, u8 mode, u32 object)
{
    u32 converted;
    if (mode != 0u && mode != 1u)
        converted = cm_fn_0053b4f0(value);
    else
        converted = cm_fn_0053b3d0(value);

    u32 object_value = object;
    u32 receiver = CM_GLOBAL(volatile u32, 0x00de1d08u);
    (void)cm_fn_00896c60((u32 *)receiver, converted, object_value, 1u, 1u);

    receiver = CM_GLOBAL(volatile u32, 0x00de1d08u);
    (void)cm_fn_00896c60((u32 *)receiver, converted, object_value, 0u, 1u);

    u8 index = 0;
    while (index < 0x32u) {
        u32 offset = 0xd7u + (u32)index * 4u;
        if (*(volatile u32 *)(converted + offset) == object_value)
            break;
        index++;
    }

    if (index == 0x32u)
        return 0x32u;

    u32 offset = 0xd7u + (u32)index * 4u;
    *(volatile u32 *)(converted + offset) = 0;
    u8 flags = *(volatile u8 *)(object_value + 0x5eu) & 3u;
    *(volatile u32 *)(object_value + 0x24u) = 0;
    *(volatile u8 *)(object_value + 0x28u) = 0;
    *(volatile u8 *)(object_value + 0x5eu) = flags;
    return flags;
}
