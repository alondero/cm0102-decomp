#include "cmre.h"

u32 CM_CDECL cm_fn_005e4890(u32 mask_arg, u32 alpha_arg, u32 config_arg)
{
    u32 disabled = CM_GLOBAL(volatile u32, 0x00aebe0cu);
    if (disabled != 0) return disabled & 0xffff0000u;

    u32 config = config_arg != 0 ? config_arg : 0x00ae3158u;
    u32 mask = mask_arg & 0xffffu;
    volatile u32 red_component = *(volatile u32 *)(config + 0x10);
    volatile u32 red_denominator = red_component + 1u;
    volatile u32 red_numerator = (red_component & mask) << 8;
    u32 red_raw = red_numerator / red_denominator;

    volatile u32 green_component = *(volatile u32 *)(config + 0x14);
    volatile u32 blue_component = *(volatile u32 *)(config + 0x18);
    volatile u32 green_denominator = green_component + 1u;
    volatile u32 green_numerator = (green_component & mask) << 8;
    u32 green_raw = green_numerator / green_denominator;

    volatile u32 alpha = alpha_arg & 0xffu;
    volatile u32 blue_denominator = blue_component + 1u;
    volatile u32 blue_numerator = (blue_component & mask) << 8;
    u32 blue_raw = blue_numerator / blue_denominator;

    u32 red = ((red_raw & 0xffu) * alpha) / 100u;
    u32 green = ((green_raw & 0xffu) * alpha) / 100u;
    u32 blue = ((blue_raw & 0xffu) * alpha) / 100u;
    if ((s32)red >= 0xff) red = 0xff;
    if ((s32)green >= 0xff) green = 0xff;
    if ((s32)blue >= 0xff) blue = 0xff;

    u32 final_config = config_arg != 0 ? config_arg : 0x00ae3158u;
    u32 format = *(volatile u32 *)(final_config + 0x14);
    if (format == 0x7e0u) {
        red &= 0xf8u;
        green &= 0xfcu;
        blue >>= 3;
        return ((red << 5) | green) << 3 | blue;
    }
    red &= 0xf8u;
    green &= 0xf8u;
    blue >>= 3;
    return ((red << 5) | green) << 2 | blue;
}
