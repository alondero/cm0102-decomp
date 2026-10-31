#include "cmre.h"

/* Original frame: sub esp, 0x200. Scratch buffer A is [esp+0] (scratch + 0)
   and buffer B is [esp+0x100] (scratch + 0x100), both in one 0x200-byte frame.
   The signed compare does not read ECX. Both jge sub-branches (param_2 >=
   0x14) read the entry ECX (0066a3e6, 0066a408); the null path overwrites
   it first. Thiscall with three stack arguments (ret 0xc). */

extern u32 CM_CDECL cm_fn_005e8290(const char *message, const char *detail,
                                  int flags);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

u32 CM_THISCALL cm_fn_0066a360(u32 self, u32 param_1, s32 param_2, u32 param_3)
{
    u8 scratch[0x200];

    if (param_2 < 0x14) {
        u32 ret_005e8290;

        (void)cm_fn_00944cff(0x009eaff4u, 0, 0, scratch + 0x100, scratch);
        /* Pushes 0x57f, edx and eax are not consumed by cm_fn_0090d130 (void).
           They are the varargs of the following cm_fn_00944c9f call. */
        u32 ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             scratch + 0x100, scratch, 0x57fu);
        /* EAX left by cm_fn_005e8290 is the value this routine returns. */
        ret_005e8290 = cm_fn_005e8290((const char *)0x009870e0u,
                                      (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return ret_005e8290;
    }

    /* jge path: the byte flag in the low byte of param_3 selects the format. */
    if ((u8)param_3 == 0) {
        s32 value = *(volatile s8 *)self;

        /* cm_fn_0066f4e0 leaves its EAX as this routine's result. */
        return cm_fn_0066f4e0(param_1, 0x009eb0ccu, (u32)value);
    }

    {
        s32 value = *(volatile s8 *)self;

        return cm_fn_0066f4e0(param_1, 0x009eb0b0u, (u32)(value + 1));
    }
}
