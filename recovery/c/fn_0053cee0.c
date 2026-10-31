#include "cmre.h"

extern char *CM_CDECL cm_fn_0066f4e0(char *output, char *format, u8 *extra);

u32 CM_CDECL cm_fn_0053cee0(u8 selector)
{
    s32 index = (s32)(s8)selector - 1;
    u32 format;

    if ((u32)index > 0x3fu) {
        format = 0x009b6b60;
    } else {
        volatile u32 jump_target = *(volatile u32 *)(0x0053d638u + (u32)index * 4u);
        (void)jump_target;
        switch (index) {
        case 0: format = 0x009b73dc; break;
        case 1: format = 0x009b73bc; break;
        case 2: format = 0x009b7398; break;
        case 3: format = 0x009b7374; break;
        case 4: format = 0x009b7350; break;
        case 5: format = 0x009b7330; break;
        case 6: format = 0x009b7310; break;
        case 7: format = 0x009b72ec; break;
        case 8: format = 0x009b72c8; break;
        case 9: format = 0x009b72a8; break;
        case 10: format = 0x009b7288; break;
        case 11: format = 0x009b7268; break;
        case 12: format = 0x009b7248; break;
        case 13: format = 0x009b7224; break;
        case 14: format = 0x009b7200; break;
        case 15: format = 0x009b71dc; break;
        case 16: format = 0x009b71bc; break;
        case 17: format = 0x009b719c; break;
        case 18: format = 0x009b7178; break;
        case 19: format = 0x009b7154; break;
        case 20: format = 0x009b7130; break;
        case 21: format = 0x009b710c; break;
        case 22: format = 0x009b70e8; break;
        case 23: format = 0x009b70c4; break;
        case 24: format = 0x009b70a0; break;
        case 25: format = 0x009b707c; break;
        case 26: format = 0x009b705c; break;
        case 27: format = 0x009b7038; break;
        case 28: format = 0x009b7018; break;
        case 29: format = 0x009b6fb0; break;
        case 30: format = 0x009b6fd4; break;
        case 31: format = 0x009b6ff8; break;
        case 32: format = 0x009b6f8c; break;
        case 33: format = 0x009b73dc; break;
        case 34: format = 0x009b6f68; break;
        case 35: format = 0x009b6f44; break;
        case 36: format = 0x009b6f24; break;
        case 37: format = 0x009b6f04; break;
        case 38: format = 0x009b6ee0; break;
        case 39: format = 0x009b6ebc; break;
        case 40: format = 0x009b6e9c; break;
        case 41: format = 0x009b6e78; break;
        case 42: format = 0x009b6e58; break;
        case 43: format = 0x009b6e38; break;
        case 44: format = 0x009b6e18; break;
        case 45: format = 0x009b6df4; break;
        case 46: format = 0x009b6dcc; break;
        case 47: format = 0x009b6da8; break;
        case 48: format = 0x009b6d84; break;
        case 49: format = 0x009b6d60; break;
        case 50: format = 0x009b6d3c; break;
        case 51: format = 0x009b6d18; break;
        case 52: format = 0x009b6cf4; break;
        case 53: format = 0x009b6cd4; break;
        case 54: format = 0x009b6cb0; break;
        case 55: format = 0x009b6c90; break;
        case 56: format = 0x009b6c70; break;
        case 57: format = 0x009b6c50; break;
        case 58: format = 0x009b6c30; break;
        case 59: format = 0x009b6c0c; break;
        case 60: format = 0x009b6bec; break;
        case 61: format = 0x009b6bc8; break;
        case 62: format = 0x009b6ba4; break;
        case 63: format = 0x009b6b84; break;
        default: format = 0x009b6b60; break;
        }
    }

    (void)cm_fn_0066f4e0((char *)0x00ae21d4, (char *)format, (u8 *)0x00ad9c64);
    return 0x00ae21d4;
}
