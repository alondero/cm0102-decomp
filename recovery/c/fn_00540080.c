#include "cmre.h"

extern s32 CM_CDECL cm_fn_0090c1d0(volatile u8 *left, volatile u8 *right, s32 compare_mode, u32 max_count);

/* cdecl with two stack arguments (ret 0, the caller pops them). Each argument points at a
   char pointer. For each non-null string at +0x38, the function copies the string and its
   NUL terminator (strlen + 1 bytes: dwords first, then bytes) into the 0xfa0-byte frame.
   Buffer B is at frame offset 0x000, buffer A at 0x7d0, and each buffer's first byte is
   cleared before its copy. The callee then compares A with B and its result is returned in
   EAX. */
s32 CM_CDECL cm_fn_00540080(u32 *param_1, u32 *param_2)
{
    u32 frame[0x3e8];   /* one 0xfa0-byte scratch area; the frame is not otherwise initialized */
    u8 *fb;
    u32 a;
    u32 b;
    u32 src;
    u32 n;
    u32 i;
    s32 result;

    fb = (u8 *)frame;
    a = *param_1;
    b = *param_2;

    fb[0x7d0] = 0;
    if (a != 0 && a + 0x38U != 0) {
        src = a + 0x38U;
        n = 0;
        while (*(u8 *)(src + n) != 0) {
            n++;
        }
        n++;
        for (i = 0; i < (n & ~3U); i += 4) {
            *(u32 *)(fb + 0x7d0 + i) = *(u32 *)(src + i);
        }
        for (i = n & ~3U; i < n; i++) {
            fb[0x7d0 + i] = *(u8 *)(src + i);
        }
    }

    fb[0] = 0;
    if (b != 0 && b + 0x38U != 0) {
        src = b + 0x38U;
        n = 0;
        while (*(u8 *)(src + n) != 0) {
            n++;
        }
        n++;
        for (i = 0; i < (n & ~3U); i += 4) {
            *(u32 *)(fb + i) = *(u32 *)(src + i);
        }
        for (i = n & ~3U; i < n; i++) {
            fb[i] = *(u8 *)(src + i);
        }
    }

    result = cm_fn_0090c1d0((volatile u8 *)(fb + 0x7d0), (volatile u8 *)fb, 1, 0U);
    return result;
}
