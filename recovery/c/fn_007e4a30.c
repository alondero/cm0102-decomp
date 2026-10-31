#include "cmre.h"

/* Original frame: sub esp, 0x200 at 0x007e4a34, then push ebx at 0x007e4a3c.
   Scratch A is the buffer at the listing's [esp+4] after the push (scratch + 0)
   and scratch B is [esp+0x104] (scratch + 0x100); both are adjacent in one
   0x200-byte frame.

   The call to cm_fn_0090d130 at 0x007e4ac2 takes no arguments and returns
   0xad4394. The three values pushed before it (0xa9 and the two scratch
   pointers) remain on the stack as the varargs of the following
   cm_fn_00944c9f call. */

extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

/* cdecl (ret 0) with two stack arguments: arg1 is [esp+4] at entry and arg2 is
   [esp+8] at entry. Both are pointers to records; the comparison reads the
   signed bytes at offsets 0x19 and 0x1a. */
u32 CM_CDECL cm_fn_007e4a30(u32 arg1, u32 arg2)
{
    u8 scratch[0x200];

    if (arg1 == 0 || arg2 == 0) {
        (void)cm_fn_00944cff(0x00a7a088u, 0u, 0u, scratch + 0x100, scratch);
        {
            u32 ret_0090d130 = cm_fn_0090d130();
            (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                                 scratch + 0x100, scratch, 0xa9u);
        }
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return 0u;
    }

    {
        /* Reads arg1 offset 0x19 (bl) before offset 0x1a (al). */
        s8 a19 = *(volatile s8 *)(arg1 + 0x19u);
        s8 a1a = *(volatile s8 *)(arg1 + 0x1au);
        s32 d1;
        s8 b19;
        s8 b1a;
        s32 d2;

        /* jle 0x7e4af5: signed compare of the two bytes; no match. */
        if (a19 <= a1a) {
            return 0u;
        }
        d1 = (s32)a19 - (s32)a1a;

        /* Reads arg2 offset 0x19 (dl) before offset 0x1a (esi). */
        b19 = *(volatile s8 *)(arg2 + 0x19u);
        b1a = *(volatile s8 *)(arg2 + 0x1au);
        d2 = (s32)b19 - (s32)b1a;

        if (d1 > d2) {
            return 1u;
        }
        /* jne 0x7e4af5: flags from the d1 vs d2 compare (not equal). */
        if (d1 != d2) {
            return 0u;
        }
        /* Equal differences: signed compare of the offset 0x19 bytes. */
        if (a19 <= b19) {
            return 0u;
        }
        return 1u;
    }
}
