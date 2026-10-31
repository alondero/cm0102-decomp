#include "cmre.h"

extern int CM_CDECL cm_fn_0090cfc0(int count);

void CM_CDECL cm_fn_00547a10(u32 object, u32 value, u32 selector)
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

    first = *(volatile u32 *)(base + 0x1cu);
    if (first == value)
        return;
    second = *(volatile u32 *)(base + 0x20u);
    if (second == value)
        return;
    third = *(volatile u32 *)(base + 0x24u);
    if (third == value)
        return;

    if (*(volatile u32 *)(base + 0x28u) == value) {
        *(volatile u32 *)(base + 0x28u) = 0u;
        return;
    }
    if (*(volatile u32 *)(base + 0x2cu) == value) {
        *(volatile u32 *)(base + 0x2cu) = 0u;
        return;
    }
    if (*(volatile u32 *)(base + 0x30u) == value) {
        *(volatile u32 *)(base + 0x30u) = 0u;
        return;
    }

    if (first == 0u) {
        *(volatile u32 *)(base + 0x1cu) = value;
        return;
    }
    if (second == 0u) {
        *(volatile u32 *)(base + 0x20u) = value;
        return;
    }
    if (third == 0u) {
        *(volatile u32 *)(base + 0x24u) = value;
        return;
    }
    if (selector == 0u)
        return;

    choice = (u32)cm_fn_0090cfc0(3);
    if (choice == 0u) {
        base = *(volatile u32 *)(object + 0x65u);
        *(volatile u32 *)(base + 0x1cu) = value;
    } else if (choice == 1u) {
        base = *(volatile u32 *)(object + 0x65u);
        *(volatile u32 *)(base + 0x20u) = value;
    } else if (choice == 2u) {
        base = *(volatile u32 *)(object + 0x65u);
        *(volatile u32 *)(base + 0x24u) = value;
    }
}
