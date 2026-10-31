#include "cmre.h"

/* Preferred-image VA 0x00938280, image base 0x00400000, 3.9.68 executable
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Retain explicit reads: the context, index stack and tables may alias. */
extern u32 CM_CDECL cm_fn_00944840(s32 a, s32 b);
extern u32 CM_CDECL cm_fn_009448b0(u32 a, u32 b);

#define U32_AT(base, offset) (*(volatile u32 *)((base) + (offset)))
#define U16_AT(base, offset) (*(volatile u16 *)((base) + (offset)))
#define S16_AT(base, offset) (*(volatile s16 *)((base) + (offset)))
#define U8_AT(base, offset) (*(volatile u8 *)((base) + (offset)))

u32 CM_CDECL cm_fn_00938280(u32 context)
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
    U16_AT(context, 0x18) = (u16)normalized_x;
    U16_AT(context, 0x1a) = (u16)normalized_y;
    if (U8_AT(context, 4) & 1) {
        u16 old_y = U16_AT(context, 0x1a);
        U16_AT(context, 0x1a) = (u16)normalized_x;
        U16_AT(context, 0x18) = (u16)(0u - old_y);
    }

    component_a = S16_AT(context, 0x1a);
    factor_a = S16_AT(context, 0x16);
    factor_b = S16_AT(context, 0x14);
    component_a *= factor_a;
    component_b = S16_AT(context, 0x18);
    factor_b *= component_b;
    result = (u32)((component_a + 0x2000) >> 14);
    result += (u32)((factor_b + 0x2000) >> 14);
    if ((s16)result > -0x400 && (s16)result < 0x400)
        result = (s16)result >= 0 ? 0x4000u : 0xffffc000u;
    U16_AT(context, 0x40) = (u16)result;
    U32_AT(context, 0x42) = 0x00937e50u;
    return result;
}
