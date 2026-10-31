#include "cmre.h"

extern u32 CM_CDECL cm_fn_005481a0(volatile u8 *object);

u32 CM_CDECL cm_fn_00548150(u32 object, u32 expected)
{
    u32 slot = object + 0xd7;
    u32 count = 50;
    u32 result;
    u8 matches = 0;
    do {
        result = *(volatile u32 *)slot;
        if (result != 0) {
            u32 field = *(volatile u32 *)(result + 0x61);
            if (field != 0) {
                result = cm_fn_005481a0((volatile u8 *)result);
                if (result == expected)
                    ++matches;
            }
        }
        slot += 4;
        --count;
    } while (count != 0);
    return (result & 0xffffff00u) | matches;
}
