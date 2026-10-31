#include "cmre.h"

/* Preferred-image VA 0x006cc070, base 0x00400000, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The sole direct caller stores AL at VA 0x0071123b.
   Preserve signed-byte differences, loop counters and accumulator overflow.
   In the horizontal special case the original accepts x through 11, not 8. */
extern u8 CM_THISCALL cm_fn_006cd3d0(int self, s8 side, int cell, u32 distance,
                                    u32 arg4, s8 arg5, u32 arg6, u32 arg7);

static s8 abs_byte(s32 value)
{
    return (s8)(value < 0 ? -value : value);
}

static u32 distance_bits(s32 x, s32 y)
{
    union { float f; u32 bits; } value;
    s8 dx = abs_byte(x), dy = abs_byte(y);
    if (dx < 0 || dx > 8 || dy < 0 || dy > 11)
        return 0x447a0000u;
    value.f = CM_GLOBAL(volatile float, 0x00a15e44u + ((u32)dy * 9u + (u32)dx) * 4u);
    return value.bits;
}

u8 CM_THISCALL cm_fn_006cc070(u32 self, s8 x0, s8 y0, s8 x1, s8 y1)
{
    s8 dx = abs_byte((s32)x1 - x0);
    s8 dy = abs_byte((s32)y1 - y0);
    s8 x, y, step, error;
    s32 origin;
    u8 total = 0, side;
    u32 cell, bits;
    union { float f; u32 bits; } distance;

    if (dx == 0) {
        if (y0 > y1) { s8 swap = y0; y0 = y1; y1 = swap; }
        y = y0;
        while (y <= y1) {
            if (x0 < 0 || x0 > 8 || y < 0 || y > 11)
                return total;
            distance.f = (float)((s32)y - y0);
            cell = self + 0x215eu + ((u32)(s32)y + (u32)(s32)x0 * 12u) * 90u;
            side = *(volatile u8 *)(self + 0x8eaeu) == 0;
            total = (u8)(total + cm_fn_006cd3d0((int)self, (s8)side, (int)cell,
                                               distance.bits, 0, 2, 6, 0));
            y = (s8)(y + 1);
        }
        return total;
    }
    if (dy == 0) {
        if (x0 > x1) { s8 swap = x0; x0 = x1; x1 = swap; }
        x = x0;
        while (x <= x1) {
            if (x < 0 || x > 11 || y0 < 0 || y0 > 11)
                return total;
            distance.f = (float)((s32)x - x0);
            cell = self + 0x215eu + ((u32)(s32)y0 + (u32)(s32)x * 12u) * 90u;
            side = *(volatile u8 *)(self + 0x8eaeu) == 0;
            total = (u8)(total + cm_fn_006cd3d0((int)self, (s8)side, (int)cell,
                                               distance.bits, 0, 2, 6, 0));
            x = (s8)(x + 1);
        }
        return total;
    }

    if ((dx < dy && y0 > y1) || (dx >= dy && x0 > x1)) {
        s8 swap = x0; x0 = x1; x1 = swap;
        swap = y0; y0 = y1; y1 = swap;
    }
    error = 0;
    x = x0;
    y = y0;
    if (dx >= dy) {
        origin = y0;
        step = (s32)y1 - origin >= 0 ? 1 : -1;
        while (x <= x1) {
            if (error >= dx) {
                error = (s8)(error - dx);
                y = (s8)(y + step);
            }
            if (x < 0 || x > 8 || y < 0 || y > 11)
                return total;
            cell = self + 0x215eu + ((u32)(s32)y + (u32)(s32)x * 12u) * 90u;
            bits = distance_bits((s32)x - x0, (s32)y - origin);
            side = *(volatile u8 *)(self + 0x8eaeu) == 0;
            total = (u8)(total + cm_fn_006cd3d0((int)self, (s8)side, (int)cell,
                                               bits, 0, 2, 6, 0));
            error = (s8)(error + dy);
            x = (s8)(x + 1);
        }
    } else {
        origin = x0;
        step = (s32)x1 - origin >= 0 ? 1 : -1;
        while (y <= y1) {
            if (error >= dy) {
                error = (s8)(error - dy);
                x = (s8)(x + step);
            }
            if (x < 0 || x > 8 || y < 0 || y > 11)
                return total;
            cell = self + 0x215eu + ((u32)(s32)y + (u32)(s32)x * 12u) * 90u;
            bits = distance_bits((s32)x - origin, (s32)y - y0);
            side = *(volatile u8 *)(self + 0x8eaeu) == 0;
            total = (u8)(total + cm_fn_006cd3d0((int)self, (s8)side, (int)cell,
                                               bits, 0, 2, 6, 0));
            error = (s8)(error + dx);
            y = (s8)(y + 1);
        }
    }
    return total;
}
