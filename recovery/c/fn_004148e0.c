#include "cmre.h"

/* Original frame: sub esp, 0x200 with esi saved. Scratch buffer A is scratch + 0
   and scratch buffer B is scratch + 0x100 (adjacent in one 0x200-byte frame).
   The routine returns void: its in-range path leaves 3 * index + 9 in EAX and
   its out-of-range path leaves the residual EAX of cm_fn_005e8290. Both caller
   sites in the Ghidra drafts (0x00417398, 0x00417414) return a constant after
   the call and do not read EAX. */

extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

void CM_THISCALL cm_fn_004148e0(u32 self, u32 key_ptr, u32 value_b, u32 value_c,
                                u32 value_d)
{
    u8 scratch[0x200];
    u32 key;
    s32 index;

    /* No null check here: the original dereferences key_ptr directly. */
    key = *(volatile u32 *)key_ptr;
    index = (s32)(key - CM_GLOBAL(u32, 0x00ae2364u) + 0x10u);
    if (index >= 0 && index <= 0x10) {
        /* Record stride 12 bytes from self + 0x1c: store value_b, value_c
           and value_d in listing order. */
        u32 record = self + (u32)index * 12u;
        *(volatile u32 *)(record + 0x1cu) = value_b;
        *(volatile u32 *)(record + 0x20u) = value_c;
        *(volatile u32 *)(record + 0x24u) = value_d;
        return;
    }

    /* Out-of-range path: format into scratch buffers A and B. */
    (void)cm_fn_00944cff(0x00987e24u, 0u, 0u, scratch + 0x100, scratch);
    {
        /* Pushes 0x3bc, A and B are not consumed by cm_fn_0090d130 (void).
           They are the varargs of the following cm_fn_00944c9f call. */
        u32 ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             scratch + 0x100, scratch, 0x3bcu);
    }
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
}
