#include "cmre.h"

void CM_CDECL cm_fn_00938a10(volatile u8 *object, u32 mode, u32 enabled)
{
    u32 base = *(volatile u32 *)(object + 0x28) + 0x58u;
    u32 shape = mode & 0xc0u;
    s16 value;
    s32 word_value;
    s32 temp;
    u32 curve;

    if (enabled != 0) {
        if (shape == 0) value = 0x20;
        else if (shape == 0x40) value = 0x40;
        else if (shape == 0x80) value = 0x80;
        else value = 0x3e7;
        *(volatile u16 *)(base + 0x24) = (u16)value;
        word_value = *(volatile s16 *)(base + 0x24);
        temp = word_value - 1;
        *(volatile u32 *)(base + 0x1c) = (u32)~temp;
    } else {
        *(volatile u32 *)(base + 0x20) = 0x2d413ccdu;
        if (shape == 0)
            *(volatile u32 *)(base + 0x20) = 0x16a09e66u;
        else if (shape == 0x80)
            *(volatile u32 *)(base + 0x20) = 0x5a82799au;
        else if (shape == 0xc0)
            *(volatile u32 *)(base + 0x20) = 0x3e7u;
        curve = *(volatile u32 *)(base + 0x20);
        temp = (s32)(curve + 0x800000u);
        value = (s16)(temp >> 24);
        *(volatile u16 *)(base + 0x24) = (u16)value;
    }

    switch (mode & 0x30u) {
    case 0:
        *(volatile u16 *)(base + 0x26) = 0;
        break;
    case 0x10:
        word_value = *(volatile s16 *)(base + 0x24);
        temp = word_value + 2;
        temp >>= 2;
        *(volatile u16 *)(base + 0x26) = (u16)temp;
        break;
    case 0x20:
        word_value = *(volatile s16 *)(base + 0x24);
        temp = word_value + 1;
        temp >>= 1;
        *(volatile u16 *)(base + 0x26) = (u16)temp;
        break;
    default:
        word_value = *(volatile s16 *)(base + 0x24);
        temp = word_value * 3 + 2;
        temp >>= 2;
        *(volatile u16 *)(base + 0x26) = (u16)temp;
        break;
    }

    shape = mode & 0x0fu;
    if (shape == 0) {
        value = *(volatile s16 *)(base + 0x24);
        value = (s16)(value - 1);
    } else {
        word_value = *(volatile s16 *)(base + 0x24);
        temp = ((s32)shape - 4) * word_value + 4;
        temp >>= 3;
        value = (s16)temp;
    }
    *(volatile u16 *)(base + 0x28) = (u16)value;
}
