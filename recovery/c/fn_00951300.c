#include "cmre.h"

/* Original callee 0x00946540 (lifted prototype: pointer, char). The second
   argument is passed as the full stack dword, as the listing pushes it. */
extern u32 *CM_CDECL cm_fn_00946540(u32 *param_1, u32 param_2);

/* Cdecl, two stack arguments. When global 0x00de441c is zero, forward both
   arguments to 0x00946540 and return its eax. Otherwise scan the string at
   arg_1 for arg_2: a lead byte with bit 0x04 in the table at 0x00de4521 + byte
   takes two bytes (code = lead<<8 | trail); a NUL trail ends the scan with 0.
   Single bytes compare against the full arg_2 dword. A match returns the
   position; reaching the NUL byte returns it only when arg_2 is zero. */
u32 CM_CDECL cm_fn_00951300(u32 arg_1, u32 arg_2)
{
    u32 s;
    u32 c0;
    u32 c1;
    u32 code;

    if (CM_GLOBAL(volatile s32, 0x00de441c) == 0) {
        return (u32)cm_fn_00946540((u32 *)arg_1, arg_2);
    }

    s = arg_1;
    for (;;) {
        c0 = *(volatile u8 *)s;
        if (c0 == 0u) {
            goto L_end;
        }
        if ((CM_GLOBAL(volatile u8, 0x00de4521u + c0) & 0x04u) == 0u) {
            goto L_single;
        }

        /* lead byte: take the trail byte */
        s = s + 1u;
        c1 = *(volatile u8 *)s;
        if (c1 == 0u) {
            return 0u;
        }
        code = (c0 << 8) | c1;
        if (arg_2 == code) {
            return s - 1u;
        }
        s = s + 1u;
        continue;

L_single:
        if (arg_2 == c0) {
            goto L_end;
        }
        s = s + 1u;
    }

L_end:
    return (arg_2 == c0) ? s : 0u;
}
