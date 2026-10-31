/* Preferred-image VA 00543440, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_005afde0(u32 self, u32 object);
extern u32 CM_CDECL cm_fn_00543280(u32 object, u32 player, u32 selector);
extern u8 CM_CDECL cm_fn_00543540(u32 player, u32 data, u32 selector);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u8 CM_CDECL cm_fn_00543440(u32 object, u32 player, u32 selector)
{
    u8 scratch[0x200];
    u32 data, table, result, text;
    u8 enabled;
    if (player == 0 || (s8)selector < 1 || (s8)selector > 31) {
        cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, scratch + 0x100, scratch, 0x48ceu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0xffu;
    }
    enabled = B(0x009cc270u);
    data = D(player + 0x61u);
    if (enabled != 0) {
        table = D(0x00ae2a60u);
        result = cm_fn_005afde0(table, object);
        if (result == 0)
            return cm_fn_00543540(player, data, selector);
        result = cm_fn_00543280(object, player, selector);
        if ((u8)result == 0)
            return 0xf9u;
    }
    return cm_fn_00543540(player, data, selector);
}
