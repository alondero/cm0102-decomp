#include "cmre.h"

extern int CM_CDECL cm_fn_0090cfc0(int count);

void CM_CDECL cm_fn_00547e70(u32 object, u32 value, u32 selector)
{
    u32 base;
    u32 first;
    u32 second;
    u32 third;
    u32 choice;

    if (object == 0u)
        return;
    base = *(volatile u32 *)(object + 0x65u);
    if (base == 0u)
        return;
    if (value == 0u)
        return;

    if (*(volatile u32 *)(value + 0xa7u) == object)
        return;
    if (*(volatile u32 *)(value + 0xabu) == object)
        return;
    if (*(volatile u32 *)(value + 0xafu) == object)
        return;

    first = *(volatile u32 *)(base + 4u);
    if (first == value) {
        *(volatile u32 *)(base + 4u) = 0u;
        return;
    }
    second = *(volatile u32 *)(base + 8u);
    if (second == value) {
        *(volatile u32 *)(base + 8u) = 0u;
        return;
    }
    third = *(volatile u32 *)(base + 0xcu);
    if (third == value) {
        *(volatile u32 *)(base + 0xcu) = 0u;
        return;
    }

    if (*(volatile u32 *)(base + 0x10u) == 0u) {
        *(volatile u32 *)(base + 0x10u) = value;
        return;
    }
    if (*(volatile u32 *)(base + 0x14u) == 0u) {
        *(volatile u32 *)(base + 0x14u) = value;
        return;
    }
    if (*(volatile u32 *)(base + 0x18u) == 0u) {
        *(volatile u32 *)(base + 0x18u) = value;
        return;
    }
    if (selector == 0u)
        return;

    choice = (u32)cm_fn_0090cfc0(3);
    if (choice == 0u) {
        base = *(volatile u32 *)(object + 0x65u);
        *(volatile u32 *)(base + 0x10u) = value;
    } else if (choice == 1u) {
        base = *(volatile u32 *)(object + 0x65u);
        *(volatile u32 *)(base + 0x14u) = value;
    } else if (choice == 2u) {
        base = *(volatile u32 *)(object + 0x65u);
        *(volatile u32 *)(base + 0x18u) = value;
    }
}
