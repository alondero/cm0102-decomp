#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(u32 *self, u32 *arg1, u16 *arg2);
extern short *CM_THISCALL cm_fn_00549f70(short *self, short arg1, char arg2,
                                          u16 arg3, u32 arg4);

void CM_STDCALL cm_fn_008d0bb0(void *target_a, void *target_b)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local[48];
    volatile u16 *range_high = (volatile u16 *)(local + 0);
    volatile u16 *value_high = (volatile u16 *)(local + 8);
    volatile u16 *range_low = (volatile u16 *)(local + 16);
    volatile u16 *value_low = (volatile u16 *)(local + 24);
    volatile u16 *output_a = (volatile u16 *)(local + 32);
    volatile u16 *output_b = (volatile u16 *)(local + 40);
    s16 current;
    s16 first_low;
    u16 year;
    short *result;

    frame.state = 0xffffffffu;
    frame.handler = 0x00964090u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    year = CM_GLOBAL(volatile u16, 0x00ae2c92u);
    cm_fn_00549f70((short *)range_low, 1, 6, year, 0xffffffffu);
    year = CM_GLOBAL(volatile u16, 0x00ae2c92u);
    frame.state = 0;
    cm_fn_00549f70((short *)range_high, 0x1f, 6, year, 0xffffffffu);
    year = CM_GLOBAL(volatile u16, 0x00ae2c92u);
    *(volatile u8 *)&frame.state = 1;
    cm_fn_00549f70((short *)value_low, 0xf, 0xb, year, 0xffffffffu);
    year = CM_GLOBAL(volatile u16, 0x00ae2c92u);
    *(volatile u8 *)&frame.state = 2;
    cm_fn_00549f70((short *)value_high, 0xf, 0, year, 0xffffffffu);

    current = (s16)CM_GLOBAL(volatile u32, 0x00ae2c90u);
    first_low = (s16)range_low[0];
    *(volatile u8 *)&frame.state = 3;
    if (current >= first_low && current <= (s16)range_high[0]) {
        cm_fn_00417c10((u32 *)target_a, (u32 *)output_a,
                       (u16 *)0x00ae2c90u);
        if (target_b != (void *)0)
            cm_fn_00417c10((u32 *)target_b, (u32 *)output_a,
                           (u16 *)range_high);
    } else if (current >= (s16)value_low[0]) {
        cm_fn_00417c10((u32 *)target_a, (u32 *)output_a,
                       (u16 *)0x00ae2c90u);
        if (target_b != (void *)0) {
            year = CM_GLOBAL(volatile u16, 0x00ae2c92u);
            result = cm_fn_00549f70((short *)output_a, 0xf, 0,
                                    (u16)(year + 1u), 0xffffffffu);
            cm_fn_00417c10((u32 *)target_b, (u32 *)output_b,
                           (u16 *)result);
        }
    } else if (current <= (s16)value_high[0]) {
        cm_fn_00417c10((u32 *)target_a, (u32 *)output_b,
                       (u16 *)0x00ae2c90u);
        if (target_b != (void *)0)
            cm_fn_00417c10((u32 *)target_b, (u32 *)output_b,
                           (u16 *)value_high);
    } else if (current < first_low) {
        cm_fn_00417c10((u32 *)target_a, (u32 *)output_b,
                       (u16 *)range_low);
        if (target_b != (void *)0)
            cm_fn_00417c10((u32 *)target_b, (u32 *)output_b,
                           (u16 *)range_high);
    } else {
        cm_fn_00417c10((u32 *)target_a, (u32 *)output_b,
                       (u16 *)value_low);
        if (target_b != (void *)0) {
            year = CM_GLOBAL(volatile u16, 0x00ae2c92u);
            result = cm_fn_00549f70((short *)output_b, 0xf, 0,
                                    (u16)(year + 1u), 0xffffffffu);
            cm_fn_00417c10((u32 *)target_b, (u32 *)output_a,
                           (u16 *)result);
        }
    }

    CM_FS(u32, 0) = frame.prev;
}
