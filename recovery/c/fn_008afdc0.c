#include "cmre.h"

void CM_FASTCALL cm_fn_008afdc0(void *self)
{
    volatile u8 *object = (volatile u8 *)self;
    u8 mode = object[0x25c];

    if (mode == 0) {
        *(volatile u16 *)(object + 0x2c) = 0x204;
        *(volatile u16 *)(object + 0x2e) = 0x204;
        *(volatile u16 *)(object + 0x26) = 1;
        *(volatile u16 *)(object + 0x28) = 0x804;
        *(volatile u16 *)(object + 0x2a) = 0x84;
        *(volatile u16 *)(object + 0x30) = 0x210;
        *(volatile u16 *)(object + 0x32) = 0x810;
        *(volatile u16 *)(object + 0x34) = 0x210;
        *(volatile u16 *)(object + 0x36) = 0x240;
        *(volatile u16 *)(object + 0x38) = 0x240;
        *(volatile u16 *)(object + 0x3a) = 0x90;
    } else if (mode == 2) {
        *(volatile u16 *)(object + 0x30) = 2;
        *(volatile u16 *)(object + 0x2c) = 0x204;
        *(volatile u16 *)(object + 0x2e) = 0x204;
        *(volatile u16 *)(object + 0x26) = 1;
        *(volatile u16 *)(object + 0x28) = 0x808;
        *(volatile u16 *)(object + 0x2a) = 0x88;
        *(volatile u16 *)(object + 0x32) = 0x210;
        *(volatile u16 *)(object + 0x34) = 0x210;
        *(volatile u16 *)(object + 0x36) = 0x240;
        *(volatile u16 *)(object + 0x38) = 0x240;
        *(volatile u16 *)(object + 0x3a) = 0x210;
    } else {
        *(volatile u16 *)(object + 0x26) = 1;
        *(volatile u16 *)(object + 0x28) = 0x804;
        *(volatile u16 *)(object + 0x2a) = 0x84;
        if (mode == 1) {
            *(volatile u16 *)(object + 0x30) = 0x90;
            *(volatile u16 *)(object + 0x2c) = 0x204;
            *(volatile u16 *)(object + 0x2e) = 0x204;
            *(volatile u16 *)(object + 0x32) = 0x210;
            *(volatile u16 *)(object + 0x34) = 0x810;
            *(volatile u16 *)(object + 0x36) = 0xc0;
        } else {
            *(volatile u16 *)(object + 0x2c) = 2;
            *(volatile u16 *)(object + 0x2e) = 0x204;
            *(volatile u16 *)(object + 0x30) = 0x208;
            *(volatile u16 *)(object + 0x32) = 0x90;
            *(volatile u16 *)(object + 0x34) = 0x810;
            *(volatile u16 *)(object + 0x36) = 0x220;
        }
        *(volatile u16 *)(object + 0x38) = 0x240;
        *(volatile u16 *)(object + 0x3a) = 0x840;
    }

    *(volatile u16 *)(object + 0x3c) = 1;
    *(volatile u16 *)(object + 0x3e) = 0x40;
    *(volatile u16 *)(object + 0x40) = 4;
    *(volatile u16 *)(object + 0x42) = 0x10;
    *(volatile u16 *)(object + 0x44) = 0x40;
    *(volatile u16 *)(object + 0x46) = 4;
    *(volatile u16 *)(object + 0x48) = 0x10;
    *(volatile u16 *)(object + 0x4a) = 0x40;
    *(volatile u16 *)(object + 0x4c) = 4;
}
