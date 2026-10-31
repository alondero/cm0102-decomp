#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_CDECL cm_fn_005e42b0(u32);
extern u32 CM_CDECL cm_fn_005e3fd0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_005e2eb0(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_005ee590(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

static void cm_copy_string(u32 destination, u32 source)
{
    u32 cursor = source, remaining = 0xffffffffu, value, count, i;
    do {
        value = B(cursor);
        ++cursor;
        --remaining;
    } while (remaining != 0 && value != 0);
    count = ~remaining;
    source = cursor - count;
    for (i = count >> 2; i != 0; --i) {
        value = D(source);
        D(destination) = value;
        source += 4;
        destination += 4;
    }
    for (i = count & 3u; i != 0; --i) {
        value = B(source);
        B(destination) = (u8)value;
        ++source;
        ++destination;
    }
}

u32 CM_THISCALL cm_fn_005eed50(u32 self, u32 input, u32 update)
{
    u8 scratch[0x200];
    u32 flags, resource, index, first, second, equal, destination, offset, context, result, x, y, width, height, text;
    if (input == 0) {
        cm_fn_00944cff(0x009cdc60, 0, 0, (u32)scratch + 0x100, (u32)scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x100, (u32)scratch, 0x22f);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result;
    }
    flags = D(self + 0xc);
    if ((flags & 0x400u) != 0) {
        resource = D(self + 0x5c);
        if (resource != 0) {
            index = 0;
            equal = 0;
            for (;;) {
                first = B(0x00ae2920 + index);
                second = B(self + 0x80 + index);
                if (first != second)
                    break;
                if (first == 0) {
                    equal = 1;
                    break;
                }
                ++index;
            }
            if (equal)
                W(0x00ae291c) = (u16)(W(0x00ae291c) - 1u);
            else
                cm_fn_005e42b0(resource);
            D(self + 0x5c) = 0;
        }
    }
    cm_copy_string(self + 0x80, input);
    if ((B(self + 0xc) & 8u) != 0) {
        destination = D(self + 0x54);
        if (destination != 0)
            cm_copy_string(destination, self + 0x80);
    }
    offset = (u32)(s32)(s16)W(0x00b77c7a) * 3u * 256u;
    context = D(0x00b74c4c + offset);
    result = offset;
    if (D(context + 0x12f545) != 0 && update != 0) {
        resource = D(self + 0x4c);
        if (resource != 0) {
            y = D(self + 0x14);
            x = D(self + 0x10);
            cm_fn_005e3fd0(x, y, resource);
            resource = D(self + 0x4c);
            cm_fn_005e42b0(resource);
            D(self + 0x4c) = 0;
        }
        cm_fn_005ee590(self, 0);
        height = D(self + 0x1c);
        width = D(self + 0x18);
        y = D(self + 0x14);
        x = D(self + 0x10);
        result = cm_fn_005e2eb0(x, y, width, height);
    }
    return result;
}
