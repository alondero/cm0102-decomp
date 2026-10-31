/* Preferred-image VA 0x00867A00, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (ret 0, caller pops). The first argument is a byte at [esp+4],
   sign-extended with movsx, and compared unsigned against 4 (ja), so negative values
   take the default case. A jump table at 0x00867A84 selects one of five cases. Every
   path loads the second argument from [esp+8], pushes a fixed global string address
   from the table of six string globals as the second argument, and calls
   0x0066F4E0 with (value, string). The callee's result is returned unchanged, so the
   return is 32-bit (char pointer). Global strings are passed by address only; their
   contents are never read here. */
#include "cmre.h"

extern char *CM_CDECL cm_fn_0066f4e0(s32 param_1, char *param_2);

char *CM_CDECL cm_fn_00867a00(s8 selector, s32 value)
{
    u32 index = (u32)(s32)selector;

    if (index > 4u) {
        return cm_fn_0066f4e0(value, &CM_GLOBAL(char, 0xa83788));
    }
    switch (index) {
    case 0:
        return cm_fn_0066f4e0(value, &CM_GLOBAL(char, 0xa8380c));
    case 1:
        return cm_fn_0066f4e0(value, &CM_GLOBAL(char, 0xa837f0));
    case 2:
        return cm_fn_0066f4e0(value, &CM_GLOBAL(char, 0xa837d4));
    case 3:
        return cm_fn_0066f4e0(value, &CM_GLOBAL(char, 0xa837b8));
    case 4:
        return cm_fn_0066f4e0(value, &CM_GLOBAL(char, 0xa8379c));
    default:
        return cm_fn_0066f4e0(value, &CM_GLOBAL(char, 0xa83788));
    }
}
