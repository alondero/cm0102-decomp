/* Preferred-image VA 0x0085F940, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: stdcall with four dword stack arguments (ret 0x10). Only the low byte
   of the first argument is read, as a signed value (MOVSX). Selector values 0..4
   pick one of five jump-table entries at 0x0085F9D0; any other value (including
   negative bytes, via the unsigned compare) takes the default path. Each path
   calls 0x0066F4E0 with cdecl arguments (second argument, the value at [esp+8],
   then a constant address pushed first, which becomes the second argument):
     0 -> 0x00A8380C, 1 -> 0x00A837F0, 2 -> 0x00A837D4, 3 -> 0x00A837B8,
     4 -> 0x00A8379C, default -> 0x00A83788.
   The caller pops the cdecl arguments, so EAX is the callee's return value and
   is returned unchanged. The two trailing stack arguments are never read.
   0x0066F4E0 is an explicit boundary; field and string meanings remain unknown. */
#include "cmre.h"

extern char *CM_CDECL cm_fn_0066f4e0(s32 arg0, char *arg1);

char *CM_STDCALL cm_fn_0085f940(s8 sel, s32 val, u32 unused_a, u32 unused_b)
{
    (void)unused_a;
    (void)unused_b;
    switch (sel) {
    case 0:
        return cm_fn_0066f4e0(val, (char *)(0x00a8380cu));
    case 1:
        return cm_fn_0066f4e0(val, (char *)(0x00a837f0u));
    case 2:
        return cm_fn_0066f4e0(val, (char *)(0x00a837d4u));
    case 3:
        return cm_fn_0066f4e0(val, (char *)(0x00a837b8u));
    case 4:
        return cm_fn_0066f4e0(val, (char *)(0x00a8379cu));
    default:
        return cm_fn_0066f4e0(val, (char *)(0x00a83788u));
    }
}
