/* WIP: return-value mismatch on cases 217/406/458 after five failed iterations. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_0066f4d0(void);

s32 CM_CDECL cm_fn_0090c1d0(volatile u8 *left, volatile u8 *right,
                             s32 compare_mode, u32 max_count)
{
    u32 index = 0;
    u32 locale;
    u8 fold_case = 1;
    u8 left_byte;
    u8 right_byte;
    u8 left_key;
    u8 right_key;

    locale = cm_fn_0066f4d0();
    if (locale == 10)
        fold_case = 0;
    if (left == (volatile u8 *)0)
        return right != (volatile u8 *)0 ? -1 : 0;
    if (right == (volatile u8 *)0)
        return 1;

    if (*left != 0) {
        for (;;) {
            right_byte = right[index];
            if (right_byte == 0)
                break;
            /* The listing tests its stack-local fold flag before loading left[i]. */
            if (fold_case != 0) {
                left_byte = left[index];
                right_key = CM_GLOBAL(volatile u8,
                    0x00aa2154u + (u32)right_byte * 3u);
                left_key = CM_GLOBAL(volatile u8,
                    0x00aa2154u + (u32)left_byte * 3u);
            } else {
                left_byte = left[index];
                left_key = left_byte;
                right_key = right_byte;
            }
            if (compare_mode == 0) {
                left_key = CM_GLOBAL(volatile u8,
                    0x00aa2156u + (u32)left_key * 3u);
                right_key = CM_GLOBAL(volatile u8,
                    0x00aa2156u + (u32)right_key * 3u);
            }
            if (left_key != right_key)
                return left_key < right_key ? -1 : 1;
            if (max_count != 0 && index == max_count - 1u)
                return 0;
            if (left[index + 1u] == 0)
                break;
            ++index;
        }
    }

    left_byte = left[index];
    if (left_byte != 0)
        return 1;
    right_byte = right[index];
    if (right_byte != 0)
        return -1;
    return 0;
}
