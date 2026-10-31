#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005b4ee0(u32 self, s8 selected, s8 side,
                             u32 coordinates, u32 output)
{
    u8 scratch[0x204];
    u32 local = (u32)scratch;
    u32 name, extension, line, text, index, chosen, candidate, value;
    s16 row = (s16)coordinates;
    s16 column = (s16)(coordinates >> 16);
    if (selected < 0 || selected > 10) {
        name = local + 4;
        extension = local + 0x104;
        line = 0x5ce;
        goto error;
    }
    if (side < 0 || side > 1) {
        name = local + 4;
        extension = local + 0x104;
        line = 0x5d6;
        goto error;
    }
    if (row < 0 || row > 8 || column < 0 || column > 11) {
        name = local + 0x104;
        extension = local + 4;
        line = 0x5df;
        goto error;
    }
    D(output) = 0;
    D(local) = (u32)(s32)selected;
    index = 0;
    do {
        value = D(local);
        if (index != value) {
            chosen = (u32)(s32)side + value * 2;
            chosen = (u32)(s32)column + ((u32)(s32)row + chosen * 3) * 4;
            candidate = (u32)(s32)side + index * 2;
            candidate = (u32)(s32)column + ((u32)(s32)row + candidate * 3) * 4;
            candidate = D(self + candidate * 4 + 0x164);
            chosen = D(self + chosen * 4 + 0x164);
            value = D(local);
            if (candidate == chosen) {
                if ((s32)index <= (s32)value)
                    return 0;
                D(output) = 1;
                return 1;
            }
        }
        ++index;
    } while ((s32)index < 11);
    return 1;
error:
    cm_fn_00944cff(0x009cc3dc, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
