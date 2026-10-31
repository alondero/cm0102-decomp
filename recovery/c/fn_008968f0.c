#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))

/* 008982b0 ignores incoming ECX; its callees receive stack arguments. */
extern u32 CM_STDCALL cm_fn_008982b0(u32, u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_008968f0(u32 self, u32 input, u32 index, u32 third)
{
    u8 scratch[0x200];
    u32 name, extension, line, text, row, table, lane, slot, id, offset, base, value, result, club, other;
    if (input == 0) {
        name = (u32)scratch + 0x100;
        extension = (u32)scratch;
        line = 0x2d1;
        goto error;
    }
    if ((s8)index < 0 || (s8)index > 0x13) {
        name = (u32)scratch;
        extension = (u32)scratch + 0x100;
        line = 0x2d9;
        goto error;
    }
    row = cm_fn_008982b0(input, third);
    if (row == 0xffffffffu)
        return 0;
    lane = (u32)(s32)(s8)index * 4u;
    table = D(self + 0x601);
    row *= 145u;
    slot = table + row + lane + 2;
    id = D(slot);
    if ((s32)id <= -1)
        return 0;
    offset = id * 110u;
    base = D(0x00ae23bc);
    value = D(base + offset + 0x61);
    if (value == 0) {
        D(slot) = 0xffffffffu;
        return 0;
    }
    result = cm_fn_0053b900(input);
    base = D(0x00ae23bc);
    if (result != 0) {
        club = D(base + offset + 0x24);
        if (club != 0) {
            value = D(club + 0x53);
            other = D(input + 0x53);
            if (value == other)
                return base + offset;
        }
        table = D(self + 0x601);
        D(table + row + lane + 2) = 0xffffffffu;
        return 0;
    }
    return base + offset;
error:
    cm_fn_00944cff(0x00a8b908, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
