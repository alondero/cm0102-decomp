#include "cmre.h"

extern s32 CM_CDECL cm_fn_0090cfc0(s32 value);

void CM_CDECL cm_fn_00547870(u32 object, u32 value, u32 selector)
{
    if (object == 0 || value == 0)
        return;

    u32 first = CM_GLOBAL(volatile u32, object + 0x9bu);
    if (first == value)
        return;
    u32 second = CM_GLOBAL(volatile u32, object + 0x9fu);
    if (second == value)
        return;
    u32 third = CM_GLOBAL(volatile u32, object + 0xa3u);
    if (third == value)
        return;

    if (CM_GLOBAL(volatile u32, object + 0xa7u) == value) {
        CM_GLOBAL(volatile u32, object + 0xa7u) = 0;
        return;
    }
    if (CM_GLOBAL(volatile u32, object + 0xabu) == value) {
        CM_GLOBAL(volatile u32, object + 0xabu) = 0;
        return;
    }
    if (CM_GLOBAL(volatile u32, object + 0xafu) == value) {
        CM_GLOBAL(volatile u32, object + 0xafu) = 0;
        return;
    }

    if (first == 0) {
        CM_GLOBAL(volatile u32, object + 0x9bu) = value;
        return;
    }
    if (second == 0) {
        CM_GLOBAL(volatile u32, object + 0x9fu) = value;
        return;
    }
    if (third == 0) {
        CM_GLOBAL(volatile u32, object + 0xa3u) = value;
        return;
    }
    if (selector == 0)
        return;

    s32 choice = cm_fn_0090cfc0(3);
    if (choice == 0)
        CM_GLOBAL(volatile u32, object + 0x9bu) = value;
    else if (choice == 1)
        CM_GLOBAL(volatile u32, object + 0x9fu) = value;
    else if (choice == 2)
        CM_GLOBAL(volatile u32, object + 0xa3u) = value;
}
