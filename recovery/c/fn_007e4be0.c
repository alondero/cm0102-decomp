#include "cmre.h"

/* Original frame: `sub esp, 0x200` after push esi. One 0x200-byte scratch
   allocation: [frame + 0] = scratch + 0x000, [frame + 0x100] = scratch + 0x100
   (both lea'd from the same esp base in the error path). */

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

/* cdecl, two stack arguments: param_1 and param_2 are record pointers. Both
   are null-checked before any field is read. The result is EAX as a 0/1 value. */
u32 CM_CDECL cm_fn_007e4be0(u32 param_1, u32 param_2)
{
    u8 scratch[0x200];

    if (param_1 != 0u && param_2 != 0u) {
        s32 a;
        s32 b;
        s32 c;
        s32 d;

        a = *(volatile s8 *)(param_1 + 0x19u);
        b = *(volatile s8 *)(param_2 + 0x1au);
        if (a + b != 0) {
            c = *(volatile s8 *)(param_2 + 0x19u);
            d = *(volatile s8 *)(param_1 + 0x1au);
            return ((d + a) > (c + b)) ? 1u : 0u;
        }
        return 0u;
    }

    {
        u32 ret_0090d130;

        cm_fn_00944cff(0x00a7a088u, 0u, 0u, scratch + 0x100, scratch);
        /* Pushes 0x107, ecx and edx before this call are not consumed by
           cm_fn_0090d130 (void); they are the varargs of cm_fn_00944c9f. */
        ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             scratch + 0x100, scratch, 0x107u);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return 0u;
    }
}
