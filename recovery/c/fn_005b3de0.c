#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))

extern u32 CM_THISCALL cm_fn_005b3560(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005b3de0(u32 self, u32 mode, u32 grid, u32 input)
{
    u8 scratch[0x200];
    u32 name, extension, line, index, value, text;
    name = (u32)scratch;
    extension = (u32)scratch + 0x100;
    if ((s8)mode < 0 || (s8)mode > 1) {
        line = 0x383;
        goto error;
    }
    if ((s16)grid < 0 || (s16)grid > 2 ||
        (s16)(grid >> 16) < 0 || (s16)(grid >> 16) > 3) {
        line = 0x38c;
        goto error;
    }
    if (input == 0) {
        name = (u32)scratch + 0x100;
        extension = (u32)scratch;
        line = 0x394;
        goto error;
    }
    for (index = 0; index != 11; ++index) {
        value = D(input);
        cm_fn_005b3560(self, index, mode, grid, value);
        input += 4;
    }
    D(self + 0x187f) = 0;
    return 1;
error:
    cm_fn_00944cff(0x009cc3dc, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
