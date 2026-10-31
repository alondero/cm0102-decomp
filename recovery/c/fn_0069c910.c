#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern void CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 value);
extern u32 CM_THISCALL cm_fn_004ae8a0(void *self, const u8 *text, u32 mode, u32 value);

u32 CM_STDCALL cm_fn_0069c910(void *object, u32 use_row, void *receiver)
{
    u32 result0 = (u32)cm_fn_00785830(object, 0u);
    u32 result1 = (u32)cm_fn_00785830(object, 1u);
    u32 result2 = (u32)cm_fn_00785830(object, 2u);
    u32 map_base = CM_GLOBAL(volatile u32, 0x00ae23b4);
    u32 row_address = map_base + result1 * 581u;

    if (use_row != 0u) {
        s32 sum = (s32)(result2 + result0);
        s32 remainder = sum % 3;
        s32 value = (s32)*(volatile s8 *)(row_address + 0x52u);
        u32 format;

        if (remainder == 0)
            format = 0x00a04fa4u;
        else if (remainder == 1)
            format = 0x00a050b0u;
        else
            format = 0x00a051a0u;

        cm_fn_0066f4e0(0x00de1f64u, format, (u32)value, (u32)value,
                       row_address + 0x38u);
    } else {
        cm_fn_0066f4e0(0x00de1f64u, 0x00a04f88u);
    }

    (void)cm_fn_004ae660(receiver, 0x00de1f64u);
    return cm_fn_004ae8a0(receiver, (const u8 *)(row_address + 0x38u),
                          0x7d5u, row_address);
}
