#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

extern u32 CM_THISCALL cm_fn_005b3c70(u32, s8, s8, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005b4410(u32 self, u32 mode, u32 grid, u32 target)
{
    u8 scratch[0x204];
    u32 index, result, name, extension, line, text;
    name = (u32)scratch + 4;
    extension = (u32)scratch + 0x104;
    if ((s8)mode < 0 || (s8)mode > 1) {
        line = 0x453;
        goto error;
    }
    if ((s16)grid < 0 || (s16)grid > 2 ||
        (s16)(grid >> 16) < 0 || (s16)(grid >> 16) > 3) {
        line = 0x45c;
        goto error;
    }
    if ((s16)target < 0 || (s16)target > 0x1a ||
        (s16)(target >> 16) < 0 || (s16)(target >> 16) > 0x23) {
        name = (u32)scratch + 0x104;
        extension = (u32)scratch + 4;
        line = 0x465;
        goto error;
    }
    for (index = 0; index != 11; ++index) {
        result = cm_fn_005b3c70(self, (s8)index, (s8)mode, grid, (u32)scratch);
        if (W((u32)scratch) == (u16)target &&
            W((u32)scratch + 2) == (u16)(target >> 16))
            return (result & 0xffffff00u) | index;
    }
    return result | 0xffu;
error:
    cm_fn_00944cff(0x009cc3dc, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return result & 0xffffff00u;
}
