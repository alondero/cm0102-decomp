#include "cmre.h"

/* Original frame: sub esp, 0x200. Scratch A is [esp] at 0x005ef27e (scratch + 0).
   Scratch B is [esp + 0x100] at 0x005ef282 (scratch + 0x100). The push 0xff
   (0x005ef2a7) and the pushes of A and B before cm_fn_0090d130 are the varargs
   of the following cm_fn_00944c9f call. */

extern u32 CM_THISCALL cm_fn_005ef5e0(u32 self, u32 arg1, u32 arg2, u32 arg3);
extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

/* ECX is not read here; on the nonzero branch it is forwarded unchanged to
   cm_fn_005ef5e0, which is a thiscall (ECX read before written). */
u32 CM_THISCALL cm_fn_005ef270(u32 self, u32 arg1, u32 arg2)
{
    u8 scratch[0x200];

    if (arg1 == 0u) {
        (void)cm_fn_00944cff(0x009ce408u, 0u, 0u, scratch + 0x100, scratch);
        u32 ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             scratch + 0x100, scratch, 0xffu);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return 0u;
    }

    /* mov edx, [0xae2364] is read before the dereference of arg1 (mov esi, [eax]). */
    {
        u32 global_value = CM_GLOBAL(volatile u32, 0x00ae2364u);
        u32 pointee = *(volatile u32 *)(arg1);
        u32 limit = global_value + 0xfffffff0u;  /* add edx, -0x10 */

        if ((s32)pointee >= (s32)limit) {
            /* The callee's EAX is returned unchanged (add esp; ret 8). */
            return cm_fn_005ef5e0(self, 0u, arg1, arg2);
        }
        return 0u;
    }
}
