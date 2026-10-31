/* Preferred-image VA 00449590, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0053b900(u32 input);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_STDCALL cm_fn_00449590(s8 selector, u32 player, u8 flag)
{
    u8 scratch[0x200];
    u32 index, target, text, result;
    if (player == 0) {
        cm_fn_00944cff(0x0098b6a0u, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, scratch + 0x100, scratch, 0x892u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    index = (u32)(s32)selector - 1u;
    if (index > 31u)
        return 1;
    index = B(0x004496a8u + index);
    target = D(0x00449694u + index * 4u);
    switch (target) {
    case 0x00449629u:
    case 0x0044966cu:
        result = cm_fn_0053b900(player);
        return result == 0;
    case 0x00449641u:
        result = cm_fn_0053b900(player);
        return result != 0;
    case 0x00449657u:
        if (flag == 1)
            return 0;
        result = cm_fn_0053b900(player);
        return result == 0;
    case 0x00449684u:
        return 1;
    }
    /* Every target of the bounded pinned image table is represented. */
    __builtin_unreachable();
}
