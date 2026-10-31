#include "cmre.h"

/* Preferred-image VA006CC540/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS6/RET24/full EAX. Caller703A95->704B02 returns the high EAX bits;
   its other branch overwrites EAX703A9B. Byte-only writes retain those bits.
   All four native walks use signed BYTE thresholds and wrapping BYTE steps.
   Native6CD3D0 overwrites EAX at entry; argument1 reads only its low BYTE. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
extern s32 CM_CDECL cm_fn_0090cfc0(s32);
extern u32 CM_THISCALL cm_fn_006cd3d0(u32, u32, u32, u32, u32, u32, u32, u32);

static u32 magnitude(s32 value)
{
    return value < 0 ? 0u - (u32)value : (u32)value;
}

static u32 cell_cost(s32 x, s32 y, s32 original_x, s32 original_y)
{
    s8 dx = (s8)(u8)magnitude(x - original_x);
    s8 dy = (s8)(u8)magnitude(y - original_y);
    volatile union { float value; u32 bits; } saved;
    if (dx < 0 || dx > 8 || dy < 0 || dy > 11) return 0x447a0000u;
    /* Native FLD/FSTP rounds back to float32 before either original call. */
    saved.value = CM_GLOBAL(volatile float, 0x00a15e44u +
                           ((u32)(s32)dy * 9u + (u32)(s32)dx) * 4u);
    return saved.bits;
}

static u32 try_cell(u32 self, u32 cell, u32 cost, u8 forward_x,
                    volatile u32 *third, volatile u32 *fifth, volatile u32 *sixth)
{
    u8 random = (u8)cm_fn_0090cfc0(2);
    u32 last = *sixth, before, after, result;
    if (forward_x) {
        before = *third;
        after = *fifth;
    } else {
        after = *fifth;
        before = *third;
    }
    result = cm_fn_006cd3d0(self, random, cell, cost, before, 1, after, last);
    if ((u8)result != 0) return result;
    last = *sixth;
    after = *fifth;
    before = *third;
    return cm_fn_006cd3d0(self, random == 0, cell, cost, before, 1, after, last);
}

u32 CM_THISCALL cm_fn_006cc540(u32 self, volatile u32 first,
                              volatile u32 second, volatile u32 third,
                              volatile u32 fourth, volatile u32 fifth,
                              volatile u32 sixth)
{
    u8 target = (u8)fourth == 1 ? 11 : 0;
    s32 original_x = (s8)(u8)first;
    s32 delta_x = 4 - original_x;
    u8 y = (u8)second, x = (u8)original_x;
    s32 original_y, delta_y, step;
    u8 abs_x, abs_y, accumulator;
    u32 eax, cell, cost;

    D(self + 0xf5ae) = 0;
    abs_x = (u8)magnitude(delta_x);
    original_y = (s8)y;
    delta_y = (s32)(s8)target - original_y;
    eax = magnitude(delta_y);
    abs_y = (u8)eax;
    if (abs_x == 0 && abs_y == 0) return eax & 0xffffff00u;

    if ((s8)abs_x >= (s8)abs_y) {
        step = delta_y >= 0 ? 1 : -1;
        eax = (u32)step;
        accumulator = 0;
        if ((s8)x < 4) {
            /* Original6CC5D1..6CC72F: increasing X. */
            if ((s8)x > 4) return eax & 0xffffff00u;
            for (;;) {
                if ((s8)accumulator >= (s8)abs_x) {
                    accumulator = (u8)(accumulator - abs_x);
                    y = (u8)((u32)y + (u32)step);
                }
                eax = (eax & 0xffffff00u) | x;
                if ((s8)x < 0 || (s8)x > 8 || (s8)y < 0 || (s8)y > 11)
                    return eax & 0xffffff00u;
                cell = self + ((u32)(s32)(s8)x * 12u + (u32)(s32)(s8)y) * 90u + 0x215eu;
                cost = cell_cost((s8)x, (s8)y, original_x, original_y);
                eax = try_cell(self, cell, cost, 1, &third, &fifth, &sixth);
                if ((u8)eax != 0) return eax;
                ++x;
                eax = (eax & 0xffffff00u) | x;
                accumulator = (u8)(accumulator + abs_y);
                if ((s8)x > 4) return eax & 0xffffff00u;
            }
        } else {
            /* Original6CC8A4..6CCA02: decreasing X. */
            if ((s8)x < 4) return eax & 0xffffff00u;
            for (;;) {
                if ((s8)accumulator >= (s8)abs_x) {
                    accumulator = (u8)(accumulator - abs_x);
                    y = (u8)((u32)y + (u32)step);
                }
                eax = (eax & 0xffffff00u) | x;
                if ((s8)x < 0 || (s8)x > 8 || (s8)y < 0 || (s8)y > 11)
                    return eax & 0xffffff00u;
                cell = self + ((u32)(s32)(s8)x * 12u + (u32)(s32)(s8)y) * 90u + 0x215eu;
                cost = cell_cost((s8)x, (s8)y, original_x, original_y);
                eax = try_cell(self, cell, cost, 0, &third, &fifth, &sixth);
                if ((u8)eax != 0) return eax;
                --x;
                eax = (eax & 0xffffff00u) | x;
                accumulator = (u8)(accumulator + abs_y);
                if ((s8)x < 4) return eax & 0xffffff00u;
            }
        }
    } else {
        step = delta_x >= 0 ? 1 : -1;
        accumulator = 0;
        if ((s8)y < (s8)target) {
            /* Original6CC732..6CC899: increasing Y. */
            if ((s8)y > (s8)target) return eax & 0xffffff00u;
            for (;;) {
                eax = (eax & 0xffffff00u) | abs_y;
                if ((s8)accumulator >= (s8)abs_y) {
                    accumulator = (u8)(accumulator - abs_y);
                    x = (u8)((u32)x + (u32)step);
                }
                if ((s8)x < 0 || (s8)x > 8 || (s8)y < 0 || (s8)y > 11)
                    return eax & 0xffffff00u;
                cell = self + ((u32)(s32)(s8)x * 12u + (u32)(s32)(s8)y) * 90u + 0x215eu;
                cost = cell_cost((s8)x, (s8)y, original_x, original_y);
                eax = try_cell(self, cell, cost, 0, &third, &fifth, &sixth);
                if ((u8)eax != 0) return eax;
                accumulator = (u8)(accumulator + abs_x);
                ++y;
                eax = (eax & 0xffffff00u) | y;
                if ((s8)y > (s8)target) return eax & 0xffffff00u;
            }
        } else {
            /* Original6CCA05..6CCB54: decreasing Y, no initial target guard. */
            for (;;) {
                eax = (eax & 0xffffff00u) | abs_y;
                if ((s8)accumulator >= (s8)abs_y) {
                    accumulator = (u8)(accumulator - abs_y);
                    x = (u8)((u32)x + (u32)step);
                }
                if ((s8)x < 0 || (s8)x > 8 || (s8)y < 0 || (s8)y > 11)
                    return eax & 0xffffff00u;
                cell = self + ((u32)(s32)(s8)x * 12u + (u32)(s32)(s8)y) * 90u + 0x215eu;
                cost = cell_cost((s8)x, (s8)y, original_x, original_y);
                eax = try_cell(self, cell, cost, 0, &third, &fifth, &sixth);
                if ((u8)eax != 0) return eax;
                accumulator = (u8)(accumulator + abs_x);
                --y;
                eax = (eax & 0xffffff00u) | y;
                if ((s8)y < (s8)target) return eax & 0xffffff00u;
            }
        }
    }
}
