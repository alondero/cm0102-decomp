#include "cmre.h"

/* Original frame: sub esp, 0x200. Scratch buffer A is [esp] after the sub
   (scratch + 0). Scratch buffer B is [esp + 0x100] (scratch + 0x100). Both are
   adjacent in one 0x200-byte frame. */

extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

u32 CM_THISCALL cm_fn_004149a0(u32 self, u32 arg1, u32 arg2)
{
    u8 scratch[0x200];
    u32 key;
    s32 index;
    u32 slot;
    u32 slot_value;

    if (arg1 == 0) {
        return 0;
    }

    key = *(volatile u32 *)arg1;
    index = (s32)(key - CM_GLOBAL(u32, 0x00ae2364u) + 0x10u);
    if (index >= 0 && index <= 0x10) {
        /* Record stride 12 bytes at self + 0x1c. Returns the record address
           when its field equals arg2, otherwise 0. */
        slot = self + (u32)index * 12u + 0x1cu;
        slot_value = *(volatile u32 *)slot;
        return (slot_value == arg2) ? slot : 0u;
    }

    /* Out-of-range path: format into scratch buffers A and B. */
    (void)cm_fn_00944cff(0x00987e24u, 0u, 0u, scratch + 0x100, scratch);
    {
        /* Pushes 0x3e1, A and B are not consumed by cm_fn_0090d130 (void).
           They are the varargs of the following cm_fn_00944c9f call. */
        u32 ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             scratch + 0x100, scratch, 0x3e1u);
    }
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    return 0;
}
