#include "cmre.h"

extern s32 CM_CDECL cm_fn_0090cfc0(s32 limit);
extern u8 CM_THISCALL cm_fn_006cd330(void *self, s32 side, u32 cell,
    u32 distance, u32 a10, u32 mode, u32 a18, u32 a1c);

/* Preferred-image VA 006CCB60, pinned 3.9.68 binary. THIS7, RET 1C;
   the sole indexed caller tests AL. All coordinate arithmetic and comparisons
   below reproduce the listing's signed bytes, including wrapped magnitudes.
   CD330 sign-extends only the side byte (6CD33A). Its mode DWORD is forwarded
   to 6F5C30, which reads only its byte at entry ESP+8 (6F5CD0/5D99/6A08).
   Thus neither partially written slot's unwritten upper bytes are observed.
   Original callees remain boundaries; this does not compose their C sources. */
static u32 distance_006ccb60(s8 x, s8 y, s32 start_x, s32 start_y)
{
    s32 dx = (s32)x - start_x;
    s32 dy = (s32)y - start_y;
    s8 bx, by;
    union { float f; u32 bits; } result;
    volatile long double loaded;

    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    bx = (s8)(u8)dx;
    by = (s8)(u8)dy;
    if (bx < 0 || bx > 8 || by < 0 || by > 11)
        return 0x447a0000u;
    /* FLD/FSTP, rather than a raw integer copy, also quiets a signalling NaN. */
    loaded = CM_GLOBAL(volatile float,
        0x00a15e44u + 4u * ((u32)(s32)by * 9u + (u32)(s32)bx));
    result.f = (float)loaded;
    return result.bits;
}

u8 CM_THISCALL cm_fn_006ccb60(void *self, u32 a04, u32 a08,
    u32 a0c, u32 a10, u32 a14, u32 a18, u32 a1c)
{
    s8 x, y, end_x, end_y, abs_x, abs_y, step;
    u8 mode, accumulator, side;
    s32 start_x, start_y, dx, dy, selector, random_value;
    u32 target, cell, distance;
    int path;

    selector = (s32)(s8)a14 - 6;
    if ((u32)selector <= 0x58u) {
        u8 map = CM_GLOBAL(volatile u8, 0x006cd2ccu + (u32)selector);
        target = CM_GLOBAL(volatile u32, 0x006cd2bcu + 4u * (u32)map);
        switch (target) {
        case 0x006ccba7u:
            mode = 4;
            break;
        case 0x006ccb85u:
            random_value = cm_fn_0090cfc0((s32)(s16)a18 + 0xbb8);
            mode = (u8)((random_value != 0) + 4);
            break;
        case 0x006ccbaeu:
            mode = (u8)((u8)cm_fn_0090cfc0(4) + 1u);
            break;
        default: /* table's 006CCBC0 target */
            mode = 0;
            break;
        }
    } else {
        mode = 0;
    }

    x = (s8)a04;
    y = (s8)a08;
    end_x = (s8)a0c;
    end_y = (s8)a10;
    start_x = (s32)x;
    start_y = (s32)y;
    dx = (s32)end_x - start_x;
    dy = (s32)end_y - start_y;
    abs_x = (s8)(u8)(dx < 0 ? -dx : dx);
    abs_y = (s8)(u8)(dy < 0 ? -dy : dy);

    /* Paths 0/1 are the two forward loops at 6CCC31/6CCDD5;
       paths 2/3 are the reverse loops at 6CCF85/6CD129. */
    if (abs_x >= abs_y) {
        if (x < end_x) goto forward;
        goto reverse_x;
    }
    if (y >= end_y) {
        if (abs_x < abs_y) goto reverse_y;
        goto reverse_x;
    }
forward:
    if (abs_x < abs_y) {
        path = 1;
        step = dx >= 0 ? 1 : -1;
        if (y > end_y) return 0;
    } else {
        path = 0;
        step = dy >= 0 ? 1 : -1;
        if (x > end_x) return 0;
    }
    goto traverse;
reverse_x:
    path = 2;
    step = dy >= 0 ? 1 : -1;
    if (x < end_x) return 0;
    goto traverse;
reverse_y:
    path = 3;
    step = dx >= 0 ? 1 : -1;
traverse:
    accumulator = 0;
    for (;;) {
        if (x < 0 || x > 8 || y < 0 || y > 11) return 0;
        if (path == 0 || path == 2) {
            if ((s8)accumulator >= abs_x) {
                accumulator = (u8)(accumulator - (u8)abs_x);
                y = (s8)(u8)((u8)y + (u8)step);
            }
        } else {
            if ((s8)accumulator >= abs_y) {
                accumulator = (u8)(accumulator - (u8)abs_y);
                x = (s8)(u8)((u8)x + (u8)step);
            }
        }
        if (x == end_x && y == end_y && (abs_x != 0 || abs_y != 0))
            goto advance;
        if (path == 0 || path == 2) {
            if (y < 0 || y > 11) return 0;
        } else {
            if (x < 0 || x > 8) return 0;
        }
        cell = (u32)self + 0x215eu +
            90u * (12u * (u32)(s32)x + (u32)(s32)y);
        /* The first forward loop loads distance before RNG. The other three
           call RNG first. Keep this order even when the table read faults. */
        if (path == 0) {
            distance = distance_006ccb60(x, y, start_x, start_y);
            side = (u8)cm_fn_0090cfc0(2);
        } else {
            side = (u8)cm_fn_0090cfc0(2);
            distance = distance_006ccb60(x, y, start_x, start_y);
        }
        if (cm_fn_006cd330(self, (s32)(s8)side, cell, distance,
                          a18, mode, a1c, 1) != 0)
            return 1;
        if (cm_fn_006cd330(self, side == 0, cell, distance,
                          a18, mode, a1c, 1) != 0)
            return 1;
advance:
        switch (path) {
        case 0:
            accumulator = (u8)(accumulator + (u8)abs_y);
            x = (s8)(u8)((u8)x + 1u);
            if (x > end_x) return 0;
            break;
        case 1:
            accumulator = (u8)(accumulator + (u8)abs_x);
            y = (s8)(u8)((u8)y + 1u);
            if (y > end_y) return 0;
            break;
        case 2:
            x = (s8)(u8)((u8)x - 1u);
            accumulator = (u8)(accumulator + (u8)abs_y);
            if (x < end_x) return 0;
            break;
        default:
            y = (s8)(u8)((u8)y - 1u);
            accumulator = (u8)(accumulator + (u8)abs_x);
            if (y < end_y) return 0;
            break;
        }
    }
}
