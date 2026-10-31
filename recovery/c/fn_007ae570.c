#include "cmre.h"

/* Original frame: sub esp, 0x200 (no pushes before the scratch use). Scratch A
   is [esp+0] and scratch B is [esp+0x100] at the listing addresses; both live
   in one 0x200-byte frame, adjacent as in the original.

   The call to cm_fn_0090d130 at 0x007ae5ee takes no arguments: it returns
   0xad4394 in eax. The three values pushed before it (0xb5 or 0xbc, and the
   two scratch pointers) remain on the stack and are the varargs of the
   following cm_fn_00944c9f call (dest, format, ret, scratch B, scratch A, line). */

extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

/* ecx_ptr is the thiscall receiver (ECX). The success path stores through it at
   0x007ae62c, so ECX is read before it is written. arg1 is the stack argument
   at [esp+4] when the routine is entered (ret 4). */
u32 CM_THISCALL cm_fn_007ae570(u32 ecx_ptr, u32 arg1)
{
    u8 scratch[0x200];

    if (arg1 == 0) {
        (void)cm_fn_00944cff(0x00a701b0u, 0u, 0u, scratch + 0x100, scratch);
        {
            u32 ret_0090d130 = cm_fn_0090d130();
            (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                                 scratch + 0x100, scratch, 0xb5u);
        }
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return 0u;
    }

    {
        /* Reads DAT_00ae2364 (edx) before the stack argument's dword (eax). */
        u32 limit_base = CM_GLOBAL(volatile u32, 0x00ae2364u);
        s32 limit = (s32)(limit_base + 0xfffffff0u);
        s32 value = *(volatile s32 *)arg1;

        /* jge 0x7ae62c: signed compare, store through ECX, return 1. */
        if (value >= limit) {
            *(volatile u32 *)ecx_ptr = (u32)value;
            return 1u;
        }
    }

    (void)cm_fn_00944cff(0x00a701b0u, 0u, 0u, scratch, scratch + 0x100);
    {
        u32 ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             scratch, scratch + 0x100, 0xbcu);
    }
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    return 0u;
}
