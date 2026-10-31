#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0068d020(u32);
extern void CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_005acbe0(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00729e30(u32 self, u32 index, u32 output)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x410];
    u32 count, table, scaled, row, first, ax, cx, receiver, node, value, other;
    u32 offset, name, extension, line, text, result;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095d63b;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0068d020((u32)scratch);
    frame.state = 0;
    if ((s32)index < 0)
        goto bad_index;
    count = D(self + 0x1c) - 1u;
    if ((s32)index > (s32)count)
        goto bad_index;
    if (output == 0) {
        name = (u32)scratch + 0x10;
        extension = (u32)scratch + 0x210;
        line = 0x283;
        goto error;
    }
    table = D(self);
    scaled = index * 3u;
    row = table + scaled * 4u;
    first = D(row);
    /* Preserve the original partial EAX/ECX values in all four pushes. */
    ax = (scaled & 0xffff0000u) | W(row + 0xa);
    cx = (table & 0xffff0000u) | W(row + 8);
    receiver = D(0x00ae2a58);
    cm_fn_005acbe0(receiver, cx, ax, cx, ax, first,
                  (u32)scratch, 0xffffffffu, 0xfffffffeu, 0xffffffffu);
    value = D((u32)scratch + 2);
    D((u32)scratch + 0xa) = value;
    node = cm_fn_0068d1b0((u32)scratch);
    while (node != 0) {
        value = D(row + 4);
        other = D(node + 0xc);
        if (value == other || value == D(node + 0x10)) {
            for (offset = 0; offset != 0x4c; offset += 4) {
                value = D(node + offset);
                D(output + offset) = value;
            }
            value = W(node + 0x4c);
            W(output + 0x4c) = (u16)value;
            frame.state = 0xffffffffu;
            value = B(node + 0x4e);
            B(output + 0x4e) = (u8)value;
            cm_fn_0068d040((u32)scratch);
            result = 1;
            goto done;
        }
        node = cm_fn_0068d1b0((u32)scratch);
    }
    goto missing;
bad_index:
    name = (u32)scratch + 0x310;
    extension = (u32)scratch + 0x110;
    line = 0x27b;
error:
    cm_fn_00944cff(0x00a17ba0, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
missing:
    frame.state = 0xffffffffu;
    cm_fn_0068d040((u32)scratch);
    result = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
