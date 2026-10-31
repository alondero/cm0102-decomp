#include "cmre.h"

u16 CM_STDCALL cm_fn_007c6930(void *arg1, u32 arg2)
{
    volatile u8 *object = (volatile u8 *)arg1;
    u32 node = *(volatile u32 *)(object + 0x1a);
    u32 code;
    u8 kind;
    u32 global_9cf4a8;
    u32 global_9cf408;
    u32 global_9cf2c0;

    if (node == 0)
        return 0;

    code = *(volatile u32 *)node;
    global_9cf4a8 = CM_GLOBAL(volatile u32, 0x009cf4a8);
    kind = *(volatile u8 *)((volatile u8 *)&arg2);
    global_9cf408 = CM_GLOBAL(volatile u32, 0x009cf408);
    global_9cf2c0 = CM_GLOBAL(volatile u32, 0x009cf2c0);

    if (code == global_9cf4a8) {
        if (kind == 0x16 || kind == 0x20)
            return 500u;
    } else if (code == global_9cf408) {
        if (kind == 0x1e || kind == 0x20)
            return 500u;
    } else if (code == global_9cf2c0) {
        if (kind == 0x16 || kind == 0x1e)
            return 500u;
    } else if (code == CM_GLOBAL(volatile u32, 0x009cf244) && kind == 7) {
        return 1000u;
    }

    if (*(volatile u8 *)(node + 0x78) == kind)
        return 500u;
    if (*(volatile u8 *)(node + 0x79) == kind)
        return 300u;

    node = *(volatile u32 *)(object + 0x1e);
    if (node == 0)
        return 0;

    code = *(volatile u32 *)node;
    if (code == global_9cf4a8) {
        if (kind == 0x16 || kind == 0x20)
            return 300u;
    } else if (code == global_9cf408) {
        if (kind == 0x1e || kind == 0x20)
            return 300u;
    } else if (code == global_9cf2c0) {
        if (kind == 0x16 || kind == 0x1e)
            return 300u;
    } else if (code == CM_GLOBAL(volatile u32, 0x009cf244) && kind == 7) {
        return 500u;
    }

    if (*(volatile u8 *)(node + 0x78) == kind)
        return 300u;
    return (*(volatile u8 *)(node + 0x79) != kind ? 0u : 100u);
}
