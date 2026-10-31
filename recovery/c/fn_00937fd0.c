#include "cmre.h"

/* Preferred-image VA 0x00937fd0, image base 0x00400000, 3.9.68 executable
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   This updates fields 0x14/0x16; 0x00938280 instead updates 0x18/0x1a. */
extern u32 CM_CDECL cm_fn_00944840(s32 a, s32 b);
extern u32 CM_CDECL cm_fn_009448b0(u32 a, u32 b);

#define U32_AT(base, offset) (*(volatile u32 *)((base) + (offset)))
#define U16_AT(base, offset) (*(volatile u16 *)((base) + (offset)))
#define S16_AT(base, offset) (*(volatile s16 *)((base) + (offset)))
#define U8_AT(base, offset) (*(volatile u8 *)((base) + (offset)))

u32 CM_CDECL cm_fn_00937fd0(u32 context)
{
    u32 stack = U32_AT(context, 0x20);
    u32 first_tables = U32_AT(context, 0x0c);
    u32 second_tables = U32_AT(context, 0x10);
    u32 first_index, second_index, first_x, second_x, first_y, second_y;
    u32 x, y, divisor, result;
    s32 normalized_x, normalized_y;
    s32 component_a, component_b, factor_a, factor_b;

    stack -= 4;
    U32_AT(context, 0x20) = stack;
    second_index = U32_AT(stack, 0);
    stack -= 4;
    U32_AT(context, 0x20) = stack;
    first_x = U32_AT(first_tables, 0);
    first_index = U32_AT(stack, 0);
    second_x = U32_AT(second_tables, 0);
    first_y = U32_AT(first_tables, 4);
    x = U32_AT(first_x + first_index * 4u, 0);
    x -= U32_AT(second_x + second_index * 4u, 0);
    second_y = U32_AT(second_tables, 4);
    y = U32_AT(first_y + first_index * 4u, 0);
    y -= U32_AT(second_y + second_index * 4u, 0);
    y <<= 10;
    x <<= 10;

    divisor = cm_fn_009448b0(x, y);
    normalized_x = (s32)cm_fn_00944840((s32)x, (s32)divisor) >> 2;
    normalized_y = (s32)cm_fn_00944840((s32)y, (s32)divisor) >> 2;
    U16_AT(context, 0x14) = (u16)normalized_x;
    U16_AT(context, 0x16) = (u16)normalized_y;
    if (U8_AT(context, 4) & 1) {
        u16 old_y = U16_AT(context, 0x16);
        U16_AT(context, 0x16) = (u16)normalized_x;
        U16_AT(context, 0x14) = (u16)(0u - old_y);
    }

    component_a = S16_AT(context, 0x1a);
    factor_a = S16_AT(context, 0x16);
    factor_b = S16_AT(context, 0x18);
    component_a *= factor_a;
    component_b = S16_AT(context, 0x14);
    factor_b *= component_b;
    result = (u32)((component_a + 0x2000) >> 14);
    result += (u32)((factor_b + 0x2000) >> 14);
    if ((s16)result > -0x400 && (s16)result < 0x400)
        result = (s16)result >= 0 ? 0x4000u : 0xffffc000u;
    U16_AT(context, 0x40) = (u16)result;
    U32_AT(context, 0x42) = 0x00937e50u;
    U32_AT(context, 0x46) = 0x009380c0u;
    U32_AT(context, 0x4a) = 0x009380c0u;
    return 0x009380c0u;
}
