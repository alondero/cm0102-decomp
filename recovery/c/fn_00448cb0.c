/* Preferred-image VA 00448cb0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_00452710(u32 self, u32 player, u32 option);
extern u32 CM_STDCALL cm_fn_00449590(s8 selector, u32 player, u8 flag);
extern u32 CM_CDECL cm_fn_007e4920(u32 source, u32 destination);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00448cb0(u32 self, volatile u32 text, volatile u32 player_arg,
                              volatile u16 score1, volatile u16 score2)
{
    u8 work[0x200];
    u32 source, player, record, line, version;
    u8 *name, *extension;
    u16 old, incoming;
    source = text;
    if (source == 0) goto invalid;
    player = player_arg;
    if (player == 0) goto invalid;
    record = cm_fn_00452710(self, player, 1);
    if (record == 0) {
        line = 0x702u;
        name = work + 0x100;
        extension = work;
        goto diagnostic;
    }
    if (cm_fn_00449590(0x17, player, 1) != 0) {
        old = W(record + 0x247u);
        incoming = score1;
        if (old == 0xffffu || (s16)incoming > (s16)old) {
            source = text;
            W(record + 0x247u) = incoming;
            cm_fn_007e4920(source, record + 0x249u);
        }
    }
    if (cm_fn_00449590(0x18, player, 1) != 0) {
        old = W(record + 0x261u);
        incoming = score2;
        if (old == 0xffffu || (s16)incoming > (s16)old) {
            source = text;
            W(record + 0x261u) = incoming;
            cm_fn_007e4920(source, record + 0x263u);
        }
    }
    return 1;
invalid:
    line = 0x6fau;
    name = work;
    extension = work + 0x100;
diagnostic:
    cm_fn_00944cff(0x0098b6a0u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
