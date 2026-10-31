#include "cmre.h"

extern u32 CM_CDECL cm_fn_00944250(u32, u32, u32);

u32 *CM_CDECL cm_fn_0093bd70(u32 object_arg, u32 unused_arg,
                              s16 first_value, s16 second_value,
                              s16 third_value)
{
    (void)unused_arg;
    u32 object = cm_fn_00944250(object_arg, 0x73u, 0u);
    u32 block;
    u32 first_record;
    u32 second_record;

    *(volatile u32 *)(object + 0x00u) = object_arg;
    *(volatile u32 *)(object + 0x3cu) = 0u;
    *(volatile u32 *)(object + 0x38u) = 0u;
    *(volatile u32 *)(object + 0x54u) = 0u;
    *(volatile u32 *)(object + 0x50u) = 0u;
    *(volatile u32 *)(object + 0x4cu) = 0u;
    *(volatile u32 *)(object + 0x0au) = 0u;
    *(volatile u32 *)(object + 0x0eu) = 0u;
    *(volatile u16 *)(object + 0x32u) = 2u;
    *(volatile u16 *)(object + 0x04u) = 0u;
    *(volatile u32 *)(object + 0x06u) = 0u;
    *(volatile u32 *)(object + 0x5cu) = 0u;
    *(volatile u16 *)(object + 0x34u) = 0u;
    *(volatile u16 *)(object + 0x68u) = 0u;
    *(volatile u16 *)(object + 0x6au) = 0u;
    *(volatile u16 *)(object + 0x6cu) = 0u;
    *(volatile u16 *)(object + 0x6eu) = 0u;
    *(volatile u32 *)(object + 0x58u) = 0u;
    *(volatile u32 *)(object + 0x64u) = 0u;
    *(volatile u32 *)(object + 0x60u) = 0u;

    block = cm_fn_00944250(object_arg, 0x34u, 1u);
    *(volatile u32 *)(object + 0x4cu) = block;
    *(volatile u32 *)(object + 0x50u) = block + 0x10u;
    *(volatile u32 *)(object + 0x40u) = block + 0x20u;
    *(volatile u32 *)(object + 0x06u) = 0u;
    first_record = block + 0x28u;
    *(volatile u32 *)(object + 0x44u) = first_record;
    second_record = first_record + 8u;
    *(volatile u32 *)(object + 0x48u) = second_record;

    *(volatile u16 *)first_record = 0u;
    *(volatile u16 *)(block + 0x20u) = 0u;
    *(volatile u16 *)(first_record + 2u) = 0u;
    *(volatile u16 *)(block + 0x22u) =
        (u16)(*(volatile u16 *)(block + 0x20u) + (u16)first_value);
    *(volatile u16 *)(first_record + 4u) =
        (u16)(*(volatile u16 *)(object + 0x6eu) + (u16)second_value);
    *(volatile u16 *)(block + 0x24u) = 0u;
    *(volatile u16 *)(first_record + 6u) =
        (u16)(*(volatile u16 *)(first_record + 4u) - (u16)third_value);
    *(volatile u16 *)(block + 0x26u) = 0u;
    *(volatile u16 *)(object + 0x36u) = 0u;
    return (u32 *)object;
}
