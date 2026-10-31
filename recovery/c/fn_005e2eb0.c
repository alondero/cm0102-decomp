#include "cmre.h"

/* Preferred-image VA 0x005e2eb0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Imports remain original IAT boundaries; their output is not initialized.
   The six-stack-argument graphics calls explicitly retain the ECX value
   present at each original call (vtable or destination-rectangle address).
   This register parameter describes that boundary, not a graphics interface. */
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_005e3680(s32, s32, s32, s32, u32);
extern u32 CM_CDECL cm_fn_005e7f60(u32, u32, u32, u32);

u32 CM_CDECL cm_fn_005e2eb0(volatile u32 first, volatile u32 second,
                           volatile u32 third, volatile u32 fourth)
{
    u8 scratch[0x20];
    u32 local = (u32)scratch, result, dest, source, left_arg, second_arg;
    u32 right_arg, bottom_arg, py, px, hi_y, hi_x, lo_y, lo_x, width, height;
    u32 right, bottom, top, left, xoff, yoff, clip_top, clip_left, vtable, target;
    result = D(0x00aebe0c);
    if (result != 0) return result;
    dest = D(0x00ae31a4);
    if (dest == 0) return result;
    result = D(0x00aebde4);
    if (result == 0 || dest == result) return result;
    bottom_arg = fourth;
    right_arg = third;
    second_arg = second;
    left_arg = first;
    result = cm_fn_005e3680((s32)left_arg, (s32)second_arg,
                           (s32)right_arg, (s32)bottom_arg, local);
    if (result == 0) return result;
    result = D(0x00aebe28);
    if (result != 0) cm_fn_005e7f60(left_arg, second_arg, right_arg, bottom_arg);
    result = D(0x00aebe14);
    if (result != 0) {
        py = D(0x00b67a14);
        px = D(0x00b67a20);
        hi_y = py + 15u;
        hi_x = px + 15u;
        lo_y = py - 5u;
        lo_x = px - 5u;
        if ((s32)hi_x >= (s32)right_arg) hi_x = right_arg;
        left_arg = first;
        if ((s32)lo_x <= (s32)left_arg) lo_x = left_arg;
        width = hi_x - lo_x;
        if ((s32)hi_y >= (s32)bottom_arg) hi_y = bottom_arg;
        second_arg = second;
        if ((s32)lo_y <= (s32)second_arg) lo_y = second_arg;
        height = hi_y - lo_y;
        height = (s32)height > 0 ? height : 0;
        result = (s32)width > 0 ? width : 0;
        if (height * result != 0) {
            target = D(0x009671c0);
            result = ((u32 (CM_STDCALL *)(u32))target)(0);
        }
    }
    result = D(0x00ae319c);
    if (result != 0) {
        right = D(local + 8);
        bottom = D(local + 12);
        source = D(0x00aebde4);
        ++right;
        top = D(local + 4);
        D(local + 8) = right;
        dest = D(0x00ae31a4);
        ++bottom;
        left = D(local);
        D(local + 12) = bottom;
        vtable = D(dest);
        target = D(vtable + 0x1c);
        ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32, u32, u32))target)
            (vtable, dest, left, top, source, local, 0x10);
    } else {
        dest = D(0x00b67a2c);
        target = D(0x009671a8);
        ((u32 (CM_STDCALL *)(u32, u32))target)(dest, local + 0x10);
        top = D(local + 0x14);
        yoff = D(0x00ae3144);
        left = D(local + 0x10);
        xoff = D(0x00ae3180);
        clip_top = D(local + 4);
        clip_left = D(local);
        top += yoff;
        right = D(local + 8);
        left += xoff;
        bottom = D(local + 12);
        top += clip_top;
        ++right;
        D(local + 0x14) = top;
        ++bottom;
        top -= clip_top;
        left += clip_left;
        top += bottom;
        D(local + 0x10) = left;
        D(local + 0x1c) = top;
        left -= clip_left;
        left += right;
        source = D(0x00aebde4);
        D(local + 0x18) = left;
        dest = D(0x00ae31a4);
        D(local + 8) = right;
        D(local + 12) = bottom;
        vtable = D(dest);
        target = D(vtable + 0x14);
        ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32, u32, u32))target)
            (local + 0x10, dest, local + 0x10, source, local, 0x01000000, 0);
        bottom_arg = fourth;
        left_arg = first;
    }
    result = D(0x00aebe14);
    if (result != 0) {
        py = D(0x00b67a14);
        px = D(0x00b67a20);
        hi_y = py + 15u;
        hi_x = px + 15u;
        lo_y = py - 5u;
        lo_x = px - 5u;
        if ((s32)hi_x >= (s32)right_arg) hi_x = right_arg;
        if ((s32)lo_x <= (s32)left_arg) lo_x = left_arg;
        width = hi_x - lo_x;
        if ((s32)hi_y >= (s32)bottom_arg) hi_y = bottom_arg;
        second_arg = second;
        if ((s32)lo_y <= (s32)second_arg) lo_y = second_arg;
        height = hi_y - lo_y;
        height = (s32)height > 0 ? height : 0;
        result = (s32)width > 0 ? width : 0;
        if (height * result != 0) {
            target = D(0x009671c0);
            result = ((u32 (CM_STDCALL *)(u32))target)(1);
        }
    }
    D(0x00aebe10) = 1;
    return result;
}
