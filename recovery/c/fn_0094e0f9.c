#include "cmre.h"

extern u32 CM_CDECL cm_fn_00951ce0(u32 string, u32 character);

u32 CM_CDECL cm_fn_0094e0f9(u32 string, u32 character)
{
    u32 result = 0;
    u32 cursor = string;
    u8 current;
    if (CM_GLOBAL(volatile u32, 0x00de441c) == 0)
        return cm_fn_00951ce0(string, character);
    do {
        u32 code;
        current = *(volatile u8 *)cursor;
        code = current;
        if (CM_GLOBAL(volatile u8, 0x00de4521 + code) & 4) {
            current = *(volatile u8 *)(cursor + 1);
            cursor++;
            if (current) {
                code = (code << 8) | current;
                if (character == code)
                    result = cursor - 1;
            } else if (result == 0) {
                result = cursor;
            }
        } else if (character == code) {
            result = cursor;
        }
        cursor++;
    } while (current);
    return result;
}
