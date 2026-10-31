/* Preferred-image VA 008c6220, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Day high16 are dead:0054c5b0/0054c8e0 test and add/subtract SI;
   their negative branches only NEG/forward that argument to each other. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00417c10(u32 self, void *output, u32 source);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32 self, void *output, u32 days);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32 self, u32 low, u32 high);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32 self, u32 source, s16 count);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008c6220(u32 self, volatile u32 record_argument,
                              volatile s8 day_argument, volatile u8 tick_argument)
{
    u8 work[0x210];
    u32 temporary, record, date, low, high, difference, quotient, factor, index, table, version;
    unsigned long long product;
    s8 days;
    u8 tick, extra, position;
    if (W(self + 0x84fu) == 0) {
        cm_fn_00417c10(self + 0x84du, work, 0x00ae2c90u);
        tick = B(0x00ae2c88u);
        B(self + 0x855u) = tick;
    }
    days = day_argument;
    if (days >= 50) {
        cm_fn_00944cff(0x00a8d514u, 0, 0, work + 0x110, work + 0x10);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x110, work + 0x10, 0x18b9u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        days = 10;
    }
    temporary = cm_fn_0054c5b0(0x00ae2c90u, work, (u32)(s32)(s16)days);
    record = record_argument;
    date = record + 0x12u;
    cm_fn_00417c10(date, work + 8, temporary);
    tick = B(0x00ae2c88u);
    extra = tick_argument;
    tick = (u8)(tick + extra);
    B(record + 0x1au) = tick;
    while ((s8)tick > 2) {
        tick = (u8)(tick - 3u);
        B(record + 0x1au) = tick;
        temporary = cm_fn_0054c5b0(date, work + 8, 1);
        cm_fn_00417c10(date, work, temporary);
        tick = B(record + 0x1au);
    }
    high = D(self + 0x851u);
    low = D(self + 0x84du);
    difference = cm_fn_0054cdb0(date, low, high);
    extra = B(self + 0x84cu);
    position = (u8)((u8)difference + extra);
    if ((s8)position >= 50) {
        factor = (u32)(s32)(s8)position;
        product = (unsigned long long)factor * 0x51eb851fu;
        quotient = (u32)(product >> 32) >> 4;
        position = (u8)(position + (u8)(quotient * 0xceu));
    }
    factor = (u32)(s32)(s8)position;
    extra = B(record + 0x1au);
    index = factor * 3u + (u32)(s32)(s8)extra;
    table = self + index * 14u + 0x18u;
    return (u32)cm_fn_0068d0d0(table, record + 0x20u, 4);
}
