#include "cmre.h"
extern char *CM_CDECL cm_fn_0066f4e0(s32 value, char *text, void *extra);
static char *const text_009a8348 = (char *)0x009a8348;
static char *const text_009a83bc = (char *)0x009a83bc;
static char *const text_009a8394 = (char *)0x009a8394;
static char *const text_009a8370 = (char *)0x009a8370;
s8 CM_STDCALL cm_fn_004cc3a0(volatile u8 *left, volatile u8 *right,
                             s32 target, u32 unused_arg4)
{
    volatile u32 *left_ref = (volatile u32 *)(left + 0x61);
    volatile u32 *right_ref = (volatile u32 *)(right + 0x61);
    volatile u8 *left_value;
    volatile u8 *right_value;
    u8 left_score;
    u8 right_score;
    u8 difference_bits;
    s8 difference;
    char *text;
    left_value = (volatile u8 *)*left_ref;
    right_value = (volatile u8 *)*right_ref;
    left_score = left_value[0x45];
    right_score = right_value[0x45];
    difference_bits = (u8)(left_score - right_score);
    difference = (s8)difference_bits;
    if (difference < -13) {
        text = text_009a8348;
    } else if (difference < -7) {
        text = text_009a83bc;
    } else if (difference < -3) {
        text = text_009a8394;
    } else if (difference < 3) {
        text = text_009a8370;
    } else if (difference < 7) {
        text = text_009a8394;
    } else if (difference < 13) {
        text = text_009a83bc;
    } else {
        text = text_009a8348;
    }
    (void)cm_fn_0066f4e0(target, text, (void *)0x00ad9c64);
    (void)unused_arg4;
    return difference;
}
