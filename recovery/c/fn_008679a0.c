#include "cmre.h"

/* Variadic formatter at 0x0066f4e0: this routine pushes only the destination and
   the format string (add esp, 8 after the call), so no variadic arguments follow. */
extern u32 CM_CDECL cm_fn_0066f4e0(u32 dst, u32 fmt, ...);

/* Format-string addresses are the original image immediates pushed by the listing
   (0x00a83744, 0x00a83750, 0x00a83760, 0x00a83774). The callee's EAX result is
   returned unchanged: every path ends with `call 0x66f4e0; add esp, 8; ret`, so
   the caller reads the full 32-bit EAX (including the upper 16 bits). */
u32 cm_fn_008679a0(s8 kind, u32 dst)
{
    if (kind == 0) {
        return (u32)cm_fn_0066f4e0(dst, 0x00a83744u);
    }
    if (kind == 1) {
        return (u32)cm_fn_0066f4e0(dst, 0x00a83750u);
    }
    if (kind == 2) {
        return (u32)cm_fn_0066f4e0(dst, 0x00a83760u);
    }
    return (u32)cm_fn_0066f4e0(dst, 0x00a83774u);
}
