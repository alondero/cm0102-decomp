#include "cmre.h"

extern s32 CM_CDECL cm_fn_0090cfc0(s32 value);

void CM_CDECL cm_fn_00547940(u32 object, u32 value, u32 selector)
{
    if (object == 0 || value == 0)
        return;

    u32 first = CM_GLOBAL(volatile u32, object + 0xa7u);
    if (first == value)
        return;
    u32 second = CM_GLOBAL(volatile u32, object + 0xabu);
    if (second == value)
        return;
    u32 third = CM_GLOBAL(volatile u32, object + 0xafu);
    if (third == value)
        return;

    if (CM_GLOBAL(volatile u32, object + 0x9bu) == value) {
        CM_GLOBAL(volatile u32, object + 0x9bu) = 0;
        return;
    }
    if (CM_GLOBAL(volatile u32, object + 0x9fu) == value) {
        CM_GLOBAL(volatile u32, object + 0x9fu) = 0;
        return;
    }
    if (CM_GLOBAL(volatile u32, object + 0xa3u) == value) {
        CM_GLOBAL(volatile u32, object + 0xa3u) = 0;
        return;
    }

    if (first == 0) {
        CM_GLOBAL(volatile u32, object + 0xa7u) = value;
        return;
    }
    if (second == 0) {
        CM_GLOBAL(volatile u32, object + 0xabu) = value;
        return;
    }
    if (third == 0) {
        CM_GLOBAL(volatile u32, object + 0xafu) = value;
        return;
    }
    if (selector == 0)
        return;

    s32 choice = cm_fn_0090cfc0(3);
    if (choice == 0)
        CM_GLOBAL(volatile u32, object + 0xa7u) = value;
    else if (choice == 1)
        CM_GLOBAL(volatile u32, object + 0xabu) = value;
    else if (choice == 2)
        CM_GLOBAL(volatile u32, object + 0xafu) = value;
}