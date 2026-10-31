#include "cmre.h"

extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_007fe4a0(void *self, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6);

/* Cdecl, one stack argument (arg_04 = [entry+4]), `ret 0`.
   The 0x200-byte frame block is one scratch array: `scratch` is its base (L),
   with the 0x100-byte buffer at L+0 and the one at L+0x100 used as separate
   output areas. Both are passed by address and left uninitialized.
   Path arg_04 == 0 (cdecl calls, caller-cleaned): 0x944cff fills both buffers,
   then 0x944c9f receives the 0x90d130 result (a no-argument call; its three
   pushes belong to 0x944c9f as varargs: L+0x100, L, 0x2113 in listing order)
   and the 0x5e8290 call's result is returned after the 0xb67a34 store.
   Path arg_04 != 0: the tail call 0x7fe4a0 is thiscall with ECX = 0xb74450 and
   six stack arguments, and its result is returned unchanged. */
u32 CM_CDECL cm_fn_00727f00(u32 arg_04)
{
    u8 scratch[0x200];
    u32 result;

    if (arg_04 != 0)
        return cm_fn_007fe4a0((void *)0x00b74450, arg_04, 0x00727fa0, 0x00727fb0, 0, 0, 0);

    cm_fn_00944cff(0x00a161d4, 0, 0, (void *)(scratch + 0x100), (void *)scratch);
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, cm_fn_0090d130(),
                   (u32)(scratch + 0x100), (u32)scratch, 0x2113);
    result = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(u32, 0x00b67a34) = 0;
    return result;
}
