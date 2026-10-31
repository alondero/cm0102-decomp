#include "cmre.h"

/* Original frame: `sub esp, 0x200`, then push esi and push edi (esp = frame - 8
   while the error-path leas run). One 0x200-byte scratch allocation:
   [frame + 0] = scratch + 0x000, [frame + 0x100] = scratch + 0x100. */

extern void CM_THISCALL cm_fn_0062c680(u32 self, u32 arg);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

/* ECX = self (esi); stack argument = arg (edi), a pointer to a 4-byte index. */
u32 CM_THISCALL cm_fn_0062ed70(u32 self, u32 arg)
{
    u8 scratch[0x200];

    if (arg == 0u) {
        u32 ret_0090d130;

        (void)cm_fn_00944cff(0x009e7f28u, arg, arg, scratch + 0x100, scratch);
        /* Pushes 0xe3a, edx and eax before this call are not consumed by
           cm_fn_0090d130 (void); they are the varargs of cm_fn_00944c9f. */
        ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             scratch + 0x100, scratch, 0xe3au);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u,
                       (int)arg);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = arg;
        return 0u;
    }

    {
        u32 idx;
        u32 limit;
        u32 addr;
        u32 value;
        u32 word_0;

        idx = *(volatile u32 *)arg;
        limit = CM_GLOBAL(volatile u32, 0x00ae2364u);
        if ((s32)idx >= (s32)limit) {
            return 0u;
        }
        cm_fn_0062c680(self, arg);

        /* Reloaded after the call: the callee may change *arg. */
        idx = *(volatile u32 *)arg;
        word_0 = *(volatile u32 *)self;
        addr = word_0 + idx * 31u;
        value = *(volatile u32 *)(addr + 0xeu);
        if (value == 0xffffffffu) {
            return 0u;
        }
        word_0 = *(volatile u32 *)(self + 4u);
        return value * 0x30u + word_0;
    }
}
