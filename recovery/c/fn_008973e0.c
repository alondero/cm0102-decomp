#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_STDCALL cm_fn_008982b0(u32, u32);
extern void CM_CDECL cm_fn_0090ba90(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_008973e0(u32 self, u32 record, u32 first, u32 second, u32 selector)
{
    u8 scratch[0x200];
    u32 name, extension, line, text, row, table, left, right, cursor, index, value;
    name = (u32)scratch;
    extension = (u32)scratch + 0x100;
    if (record == 0) {
        name = (u32)scratch + 0x100;
        extension = (u32)scratch;
        line = 0x4bf;
        goto error;
    }
    if ((s8)first < 0 || (s8)first > 0x13) {
        line = 0x4c7;
        goto error;
    }
    if ((s8)second < 0 || (s8)second > 0x13) {
        line = 0x4cf;
        goto error;
    }
    /* 008982b0 takes two stack arguments and ignores incoming ECX. */
    row = cm_fn_008982b0(record, selector);
    if (row == 0xffffffffu)
        return 0;
    table = D(self + 0x601);
    row *= 145u;
    right = table + row + (u32)(s32)(s8)second * 4u + 2;
    left = table + row + (u32)(s32)(s8)first * 4u + 2;
    cm_fn_0090ba90(left, right, 4);
    if (B(self + 0x5f0) != 0) {
        cursor = self + 0x38;
        for (index = 0; index != 16; ++index, cursor += 0x5c) {
            if (D(cursor) != 0 && D(cursor - 8) == record && D(cursor - 4) == selector) {
                D(cursor) = 0;
                value = B(self + 0x5f0);
                B(self + 0x5f0) = (u8)(value - 1u);
            }
        }
    }
    return 1;
error:
    cm_fn_00944cff(0x00a8b908, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
