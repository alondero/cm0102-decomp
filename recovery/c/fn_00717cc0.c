#include "cmre.h"

extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, s32 param_3);
extern u32 CM_THISCALL cm_fn_007fe4a0(u8 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6);
extern void CM_THISCALL cm_fn_007ff130(u8 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

/* Cdecl with two stack arguments (ret 0). The second argument is zero in the first
   branch; in the second branch the first argument is passed to the object at 0x00b74450.
   Scratch offsets: the 0x200-byte frame buffer at offset 0 (arg5 of 0x944cff) and
   offset 0x100 (arg4 of 0x944cff). All four direct callers discard EAX. */
void CM_CDECL cm_fn_00717cc0(u32 arg_a, u32 arg_b)
{
    u8 scratch[0x200];
    char *text;
    u32 ret;

    if (arg_b == 0) {
        cm_fn_00944cff(0x00a161d4U, arg_b, arg_b, (void *)(scratch + 0x100), (void *)scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0U, 0x009870e8U, (u32)text,
                       (u32)(scratch + 0x100), (u32)scratch, 0x00ceU);
        cm_fn_005e8290((const char *)0x009870e0U,
                             (const char *)&CM_GLOBAL(u8, 0x00ae24d0), (s32)arg_b);
        CM_GLOBAL(volatile u32, 0x00b67a34) = arg_b;
        return;
    }

    ret = cm_fn_007fe4a0(&CM_GLOBAL(u8, 0x00b74450), arg_a, 0x00717db0U, 0x00718930U,
                         0U, 0U, 0U);
    if (ret == 0) {
        return;
    }
    cm_fn_007ff130(&CM_GLOBAL(u8, 0x00b74450), arg_a, 1U, arg_b, 0U);
    cm_fn_007ff130(&CM_GLOBAL(u8, 0x00b74450), arg_a, 2U, 0U, 0U);
    cm_fn_007ff130(&CM_GLOBAL(u8, 0x00b74450), arg_a, 3U, 0U, 0U);
    cm_fn_007ff130(&CM_GLOBAL(u8, 0x00b74450), arg_a, 4U, 1U, 0U);
}
