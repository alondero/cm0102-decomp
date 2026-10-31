/* Preferred-image VA 0x004CC2E0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: stdcall with three stack arguments, ret 0xc. The first argument
   is an object pointer; the byte at [object+0x61]+0x45 (read through a pointer
   stored at +0x61) is compared as a signed value against 4, 8, 0x0c, 0x0f and
   0x12. The second argument is passed as the first argument of 0066F4E0 with
   one of six constant string addresses and the address 0x00AD9C64; the third
   argument is never read. Returns nothing. 0066F4E0 is an explicit boundary
   called with three cdecl arguments (the caller pops 0xc). */
#include "cmre.h"

extern char *CM_CDECL cm_fn_0066f4e0(u32, char *, u8 *);

void CM_STDCALL cm_fn_004cc2e0(u8 *object, u32 arg2, u32 arg3)
{
    s8 level;

    (void)arg3;
    level = *(s8 *)(*(u8 **)(object + 0x61) + 0x45);
    if (level < 4) {
        cm_fn_0066f4e0(arg2, (char *)0x009a8320, (u8 *)0x00ad9c64);
        return;
    }
    if (level < 8) {
        cm_fn_0066f4e0(arg2, (char *)0x009a82fc, (u8 *)0x00ad9c64);
        return;
    }
    if (level < 0x0c) {
        cm_fn_0066f4e0(arg2, (char *)0x009a82dc, (u8 *)0x00ad9c64);
        return;
    }
    if (level < 0x0f) {
        cm_fn_0066f4e0(arg2, (char *)0x009a82b8, (u8 *)0x00ad9c64);
        return;
    }
    if (level < 0x12) {
        cm_fn_0066f4e0(arg2, (char *)0x009a8290, (u8 *)0x00ad9c64);
        return;
    }
    cm_fn_0066f4e0(arg2, (char *)0x009a826c, (u8 *)0x00ad9c64);
}
