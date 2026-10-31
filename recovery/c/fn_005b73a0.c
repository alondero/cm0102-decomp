#include "cmre.h"
#define W(a) CM_GLOBAL(volatile u16, (a))
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_005b73a0(u32 flags, u32 first, u32 second)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 bit, choice, name, extension, line, text;
    for (bit = 1; bit <= 0x40; bit <<= 1) {
        if (flags & bit)
            break;
    }
    if (bit > 0x40) {
        name = local;
        extension = local + 0x100;
        line = 0xc3d;
        goto error;
    }
    W(first) = (u16)bit;
    if (flags & 0x80)
        choice = 0x80;
    else if (flags & 0x100)
        choice = 0x100;
    else if ((flags & 0x200) || (flags & 1))
        choice = 0x200;
    else if (flags & 0x400)
        choice = 0x400;
    else if (flags & 0x800)
        choice = 0x800;
    else {
        name = local + 0x100;
        extension = local;
        line = 0xc66;
        goto error;
    }
    W(second) = (u16)choice;
    return 1;
error:
    cm_fn_00944cff(0x009cc3dc, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
