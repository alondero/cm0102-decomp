#include "cmre.h"

u32 CM_CDECL cm_fn_0093bbd0(volatile u32 *cursor, volatile u16 *first_out,
                           volatile u16 *second_out)
{
    u32 start = cursor[1];
    volatile u8 *bytes = (volatile u8 *)cursor[0];
    u8 high_byte = bytes[start];
    u32 offset = start + 1;
    u8 low_byte;
    u32 packed;
    u32 value;
    u32 kind;
    u32 first_component = 0;
    u32 second_component = 0;

    cursor[1] = offset;
    low_byte = bytes[offset];
    packed = ((u32)high_byte << 8) | low_byte;
    offset++;
    cursor[1] = offset;
    value = packed & 0x3fffu;
    kind = packed >> 14;

    if (value < 0x898u) {
        if (value == 0 && kind <= 1) {
            u8 a = bytes[offset];
            offset++;
            cursor[1] = offset;
            {
                u8 b = bytes[offset];
                u16 first = (u16)(((u16)a << 8) | b);
                offset++;
                cursor[1] = offset;
                {
                    u8 c = bytes[offset];
                    offset++;
                    cursor[1] = offset;
                    {
                        u8 d = bytes[offset];
                        offset++;
                        cursor[1] = offset;
                        *first_out = first;
                        *second_out = (u16)(((u16)c << 8) | d);
                    }
                }
            }
            return kind == 0 ? 1u : 0u;
        }
        first_component = value;
        second_component = 0;
    } else if (value < 0x313cu) {
        u32 adjusted = value - 0x898u;
        first_component = adjusted / 0x66u + 1u;
        second_component = adjusted % 0x66u + 1u;
    } else if (value < 0x393cu) {
        u32 adjusted = value - 0x313cu;
        u32 extra = bytes[offset];
        offset++;
        cursor[1] = offset;
        adjusted = (adjusted << 8) | extra;
        first_component = adjusted / 0x2d4u + 1u;
        second_component = adjusted % 0x2d4u + 1u;
    } else {
        u32 adjusted = value - 0x393cu;
        u32 high = bytes[offset];
        offset++;
        cursor[1] = offset;
        {
            u32 middle = bytes[offset];
            offset++;
            cursor[1] = offset;
            adjusted = (adjusted << 16) | (high << 8) | middle;
        }
        first_component = adjusted / 0x299au;
        second_component = adjusted % 0x299au;
    }

    u32 target = CM_GLOBAL(volatile u32, 0x0093bd54u + kind * 4u);
    (void)target;
    switch (kind) {
    case 0:
        break;
    case 1:
        second_component = 0u - second_component;
        {
            u32 temp = first_component;
            first_component = second_component;
            second_component = temp;
        }
        break;
    case 2:
        first_component = 0u - first_component;
        second_component = 0u - second_component;
        break;
    case 3:
        {
            u32 temp = first_component;
            first_component = second_component;
            second_component = 0u - temp;
        }
        break;
    default:
        first_component = 0;
        second_component = 0;
        break;
    }

    *first_out = (u16)((s32)first_component >> 1);
    *second_out = (u16)second_component;
    return 1u - (first_component & 1u);
}
