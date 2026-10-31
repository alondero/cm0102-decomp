#include "cmre.h"

/* Original routine 0x0071b4a0 (preferred-image VA, image base 0x00400000).
   Convention: ret 0 (caller cleans up), so plain cdecl. The listing reads
   [entry+4] at 0x0071b5b5, the first stack slot above the return address; it
   is kept as the parameter arg1 and pushed again as the middle argument of the
   call at 0x0071b5c3.
   Each thiscall uses ECX = 0x00b74450 (the object), set before every call.
   Argument order: the last pushed value is the first C argument (MSVC pushes
   right to left), so "push A; push B; push C; call" is f(this, C, B, A). */

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 param_2, u32 param_3, u32 param_4,
                                      u32 param_5, u32 param_6);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 param_2, u32 param_3, u32 param_4);

CM_CDECL void cm_fn_0071b4a0(u32 arg1)
{
    if (cm_fn_007fe600(0x00b74450u, 0x0071b630u, 0x0071bdc0u, 0u, 0x0071c170u, 0u) == 0) {
        return;
    }
    cm_fn_007ff270(0x00b74450u, 0u, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 2u, 6u, 0u);
    cm_fn_007ff270(0x00b74450u, 9u, 0xffffffffu, 0u);
    cm_fn_007ff270(0x00b74450u, 0x0au, 3u, 0u);
    cm_fn_007ff270(0x00b74450u, 0x0bu, 0xffffffffu, 0u);
    cm_fn_007ff270(0x00b74450u, 0x0cu, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 0x0du, 0xffffffffu, 0u);
    cm_fn_007ff270(0x00b74450u, 0x16u, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 0x17u, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 0x18u, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 0x19u, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 0x1au, 0xffffffffu, 0u);
    cm_fn_007ff270(0x00b74450u, 0x1bu, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 0x1cu, 1u, 0u);
    cm_fn_007ff270(0x00b74450u, 0x1du, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 0x1eu, arg1, 0u);
    cm_fn_007ff270(0x00b74450u, 0x1fu, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 0x20u, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 0x21u, 0xffffffffu, 0u);
    cm_fn_007ff270(0x00b74450u, 0x22u, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 0x23u, 0xffffffffu, 0u);
    cm_fn_007ff270(0x00b74450u, 0x2du, 0u, 0u);
}
