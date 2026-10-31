#include "cmre.h"

/* Original frame: sub esp, 0x200. Scratch A is [esp] at 0x008aa192 (scratch + 0).
   Scratch B is [esp + 0x100] at 0x008aa196 (scratch + 0x100).
   The push 0x1a49 (0x008aa1bb) and the pushes of A and B before
   cm_fn_0090d130 are the varargs of the following cm_fn_00944c9f call. */

#define OBJ_U32(off) (*(volatile u32 *)(self + (u32)(off)))

extern u32 CM_THISCALL cm_fn_005b5910(u32 self, u32 arg1, u32 arg2);
extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

/* ECX is the receiver: it is read at 0x008aa162 (self + 0xe40). */
u32 CM_THISCALL cm_fn_008aa150(u32 self, u32 arg1, u32 arg2)
{
    u8 scratch[0x200];
    s8 index = (s8)(u8)(arg1 & 0xffu);

    /* test al, al; jl and cmp al, 0xa; jg: signed low byte in [0, 10]. */
    if (index >= 0 && index <= 10) {
        if (OBJ_U32(0xe40) == 0u) {
            (void)cm_fn_005b5910(self + 0xe44u, arg1, arg2);
            return 1u;
        }
        return 0u;
    }

    (void)cm_fn_00944cff(0x00a8b94cu, 0u, 0u, scratch + 0x100, scratch);
    u32 ret_0090d130 = cm_fn_0090d130();
    (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                         scratch + 0x100, scratch, 0x1a49u);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    return 0u;
}
