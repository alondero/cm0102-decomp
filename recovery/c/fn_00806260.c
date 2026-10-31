#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern s32 CM_CDECL cm_fn_0090c1d0(u32, u32, s32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

static u32 scan_length(u32 pointer)
{
    u32 remaining = 0xffffffffu;
    u8 byte;
    do {
        if (remaining == 0)
            break;
        --remaining;
        byte = B(pointer);
        ++pointer;
    } while (byte != 0);
    return ~remaining - 1;
}

u32 CM_CDECL cm_fn_00806260(u32 left, u32 right, u32 words)
{
    u8 scratch[0x210];
    u32 local = (u32)scratch;
    u32 left_length, right_length, segment_length, limit, index, position;
    u32 segment, found, text;
    if (left == 0 || right == 0) {
        cm_fn_00944cff(0x00a7de94, 0, 0, local + 0x110, local + 0x10);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      local + 0x110, local + 0x10, 0x371);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    left_length = scan_length(left);
    right_length = scan_length(right);
    D(local + 0xc) = left_length;
    D(local) = right_length;
    if (right_length > left_length)
        return 0;
    if (words == 0) {
        limit = left_length - right_length + 1;
        D(local) = limit;
        if (limit != 0) {
            index = 0;
            do {
                if (cm_fn_0090c1d0(left + index, right, 0, right_length) == 0)
                    return 1;
                limit = D(local);
                ++index;
            } while (index < limit);
        }
        return 0;
    }
    position = 0;
    D(local + 4) = right;
    if (right_length == 0)
        return 0;
    do {
        if (B(right + position) == 0x20 || position == right_length - 1) {
            D(local + 8) = 0;
            if (position < right_length - 1)
                B(right + position) = 0;
            segment = D(local + 4);
            segment_length = scan_length(segment);
            if (segment_length != 0 && segment_length <= left_length) {
                limit = left_length - segment_length + 1;
                index = 0;
                if (limit != 0) {
                    do {
                        found = D(local + 8);
                        if (found != 0)
                            break;
                        segment = D(local + 4);
                        if (cm_fn_0090c1d0(left + index, segment, 0, segment_length) == 0)
                            D(local + 8) = 1;
                        left_length = D(local + 0xc);
                        ++index;
                    } while (index < limit);
                }
            }
            right_length = D(local);
            if (position < right_length - 1)
                B(right + position) = 0x20;
            found = D(local + 8);
            if (found != 0)
                return 1;
            right_length = D(local);
            D(local + 4) = right + position + 1;
        }
        ++position;
    } while (position < right_length);
    return 0;
}
