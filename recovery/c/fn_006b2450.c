/* Preferred-image VA 006b2450, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00786b70(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_006b2450(u32 person, u32 type)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x2e0];
    u32 previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095d16b;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0077bbe0((u32)scratch + 0x200, 0, 0);
    frame.state = 0;
    u32 result;
    u32 line;
    u8 *name = scratch + 0x100;
    u8 *extension = scratch;
    if (!person) {
        line = 0x68e;
        goto error;
    }
    u32 manager = D(0x00b7431c);
    result = cm_fn_00786b70(manager, person, type, (u32)scratch + 0x200);
    if (!result) {
        line = 0x695;
        name = scratch;
        extension = scratch + 0x100;
        goto error;
    }
    result = cm_fn_007fe600(0x00b74450, 0x006b25e0, 0x006b2790, 1, 0, 0);
    if (result) {
        u32 index = (u32)cm_fn_00785830((u32)scratch + 0x200, 0);
        u32 base = D(0x00ae23b4);
        u32 club = base + index * 581u;
        cm_fn_007ff270(0x00b74450, 0, type, 0);
        result = cm_fn_007ff270(0x00b74450, 1, club, 0);
    }
    goto done;
error:
    cm_fn_00944cff(0x00a15138, 0, 0, name, extension);
    u32 text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
