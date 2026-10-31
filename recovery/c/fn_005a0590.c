#include "cmre.h"

/* Original frame: sub esp, 0x204 with esi and edi saved. The scratch base L is
   the esp right after the sub. Scratch + 0 is the 4-byte out value written by
   cm_fn_00540a50 ([esp+8] at L). Scratch + 4 is buffer A ([esp+0xc] before the
   pushes) and scratch + 0x104 is buffer B ([esp+0x10c] before the pushes). */

extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 arg1)
{
    u8 scratch[0x204];
    u32 result;
    u32 out_value;
    u32 record;
    u32 current;

    if (arg1 == 0) {
        (void)cm_fn_00944cff(0x009c8784u, arg1, arg1, scratch + 0x104,
                             scratch + 4);
        {
            /* Pushes 0x20b5, A and B are not consumed by cm_fn_0090d130 (void).
               They are the varargs of the following cm_fn_00944c9f call. */
            u32 ret_0090d130 = cm_fn_0090d130();
            (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                                 scratch + 0x104, scratch + 4, 0x20b5u);
        }
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u,
                       (int)arg1);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = arg1;
        return 0;
    }

    result = cm_fn_00540a50(arg1, (u32)scratch, 1u);
    if (result != 0) {
        out_value = *(volatile u32 *)scratch;
        if (out_value == 0) {
            arg1 = result;
        }
    }

    /* Original reads [esi] before [edi]; both are volatile so the order holds. */
    record = *(volatile u32 *)arg1;
    current = *(volatile u32 *)self;
    return record * 359u + current;
}
