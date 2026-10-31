/* Preferred-image VA 0x00550230, image base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ret 0 with no ECX read: cdecl, four stack arguments, caller cleans up.
   Scratch: the listing's [esp+4] buffer is scratch+0x000 and its [esp+0x104]
   buffer is scratch+0x100 (one 0x200-byte allocation). 0x0090d130 takes no
   arguments; the pushes before its call are the trailing varargs of 0x00944c9f.
   The 0x00944c9f/0x005e8290 cleanup is 0x24 bytes, handled by the caller ABI.
   0x00945934 is entered with -1 and is not assumed to return; the fall-through
   reaches the common tail either way, as in the original. */
#include "cmre.h"

extern void cm_fn_005a8f60(void);
extern void cm_fn_005cc7d0(void);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00932c30(u32 destination, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern void cm_fn_00945934(u32 param_1);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 count, u32 element_size, u32 state_address);

u32 cm_fn_00550230(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    u32 iVar1;
    u32 iVar3;
    u32 r;
    u8 scratch[0x200];

    iVar1 = *(volatile u32 *)(param_4 + 0x37);
    if (*(volatile u32 *)(param_4 + 0x3f) != 1) {
        cm_fn_00944cff(0x009b8dc0u, 0, 0, scratch + 0x100, scratch);
        r = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, r, scratch + 0x100, scratch, 0x605u);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        cm_fn_005cc7d0();
        cm_fn_005a8f60();
        cm_fn_00945934(0xffffffffu);
    }
    if (param_2 != 0) {
        iVar3 = cm_fn_00932c30(param_3, 0x1a, iVar1, param_1);
        return (u32)(iVar3 == iVar1);
    }
    iVar3 = cm_fn_00945bfe(param_3, 0x1a, iVar1, param_1);
    return (u32)(iVar3 == iVar1);
}
