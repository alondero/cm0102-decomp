/* Preferred-image VA 0x0093A400, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (ECX is not read before it is written; ret 0 so the caller pops
   the one stack argument), argument = context pointer ctx (ebp). Reads
   selector = dword ctx+0x04, then top = dword ctx+0x20 - 4 and v = dword [top].
   Selector dispatch (esi = selector - 0x56, jump table through the byte table at
   0x0093A4BC, entries at 0x0093A4A0): 0x56 and 0x57 take the rounding path; 0x5D,
   0x64, 0x65, 0x66 and 0x67 each take their own path; every other selector,
   including any selector outside 0x56..0x67, pops only (no store).
   - 0x56 / 0x57: if v < 0 then w = -((0x20 - v) & ~0x3F), else w = (v + 0x20) & ~0x3F.
     If w ^ v is negative and v != 0 then w = 0. w = w >> 6 (arithmetic). If the
     selector is 0x56 then w += 1. Store (~w) & 1 at top.
   - 0x5D: store (v == 0) at top.
   - 0x64: store v if v > 0, else -v at top.
   - 0x65: store -v at top.
   - 0x66: store (v & ~0xFF) | (v & 0xC0) at top.
   - 0x67: t = v + 0x3F; store (t & ~0xFF) | (t & 0xC0) at top.
   Non-pop paths write the value at top and set ctx+0x20 = top + 4 (net no change);
   the no-op path sets ctx+0x20 = top (net pop). EAX returns the final value.
   Field meanings remain unknown; the stack-pointer field is only named by offset.
   Context accesses are volatile so -O2 keeps the listing's access order. */
#include "cmre.h"

u32 CM_CDECL cm_fn_0093a400(u8 *ctx)
{
    u32 selector;
    u32 top;
    u32 v;
    u32 w;
    u32 t;
    u32 result;
    int pushes;

    selector = *(volatile u32 *)(ctx + 0x04);
    top = *(volatile u32 *)(ctx + 0x20) - 4u;
    v = *(volatile u32 *)top;
    result = v;
    pushes = 1;

    switch (selector) {
    case 0x56u:
    case 0x57u:
        if ((s32)v < 0) {
            w = (0x20u - v) & 0xffffffc0u;
            w = 0u - w;
        } else {
            w = (v + 0x20u) & 0xffffffc0u;
        }
        if ((s32)(w ^ v) < 0) {
            if (v != 0u) {
                w = 0u;
            }
        }
        w = (u32)((s32)w >> 6);
        if (selector == 0x56u) {
            w = w + 1u;
        }
        result = v;
        *(volatile u32 *)top = (~w) & 1u;
        break;
    case 0x5du:
        result = v;
        *(volatile u32 *)top = (v == 0u) ? 1u : 0u;
        break;
    case 0x64u:
        if ((s32)v > 0) {
            result = v;
        } else {
            result = 0u - v;
        }
        *(volatile u32 *)top = result;
        break;
    case 0x65u:
        result = 0u - v;
        *(volatile u32 *)top = result;
        break;
    case 0x66u:
        result = (v & 0xffffff00u) | (v & 0xc0u);
        *(volatile u32 *)top = result;
        break;
    case 0x67u:
        t = v + 0x3fu;
        result = (t & 0xffffff00u) | (t & 0xc0u);
        *(volatile u32 *)top = result;
        break;
    default:
        pushes = 0;
        break;
    }

    if (pushes) {
        *(volatile u32 *)(ctx + 0x20) = top + 4u;
    } else {
        *(volatile u32 *)(ctx + 0x20) = top;
    }
    return result;
}
