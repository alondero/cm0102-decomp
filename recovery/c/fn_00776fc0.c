/* Preferred-image VA00776fc0, base00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original date helpers and native SEH/unwind remain explicit boundaries. */
#include "cmre.h"
#define R8(a) CM_GLOBAL(volatile u8, (u32)(a))
#define R16(a) CM_GLOBAL(volatile u16, (u32)(a))
#define R32(a) CM_GLOBAL(volatile u32, (u32)(a))
#define LOW16(a, b) (((a) & 0xffff0000u) | (u16)(b))
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *, u32);

u32 CM_THISCALL cm_fn_00776fc0(u32 self, volatile u32 argument)
{
    struct {
        volatile u32 cached_self;
        u8 before[8], after[8], date[8];
        volatile struct { u32 prev, handler, state; } seh;
    } local;
    u32 record, value, table, entries, national_limit, result = 1u;
    u16 lower_year, year;
    s8 count;
    volatile u8 *total = (volatile u8 *)&argument;

    local.seh.state = 0xffffffffu;
    local.seh.handler = 0x0095df48u;
    u32 previous = CM_FS(u32, 0);
    local.seh.prev = previous;
    CM_FS(u32, 0) = (u32)&local.seh;
    record = argument;
    local.cached_self = self;
    u32 second = LOW16(previous, R16(record + 0x28u));
    u32 first = LOW16(self, R16(record + 0x2au));
    cm_fn_00549ef0(local.date, first, second);
    local.seh.state = 0;
    cm_fn_0054c8e0((u32)local.date, (u32)local.before, 1u);
    *(volatile u8 *)&local.seh.state = 1u;
    cm_fn_0054c5b0((u32)local.date, (u32)local.after, 1u);
    value = R32(record + 0x14u);
    *(volatile u8 *)&local.seh.state = 2u;
    table = R32(0x00ae2c74u);
    if ((u32)cm_fn_005c6710((void *)table, value) != 0)
        goto done;

    value = R32(record + 0x1cu);
    lower_year = R16((u32)local.before + 2u);
    national_limit = R32(0x00ae235cu);
    if (value != 0) {
        u32 nations = R32(0x00ae2350u);
        u32 id = R32(value);
        if ((s32)id < (s32)(national_limit - nations * 2u)) {
            table = R32(self + 0x20u);
            *total = 0;
            count = (s8)R8(table + id * 6u + 5u);
            if (count > 0) {
                entries = R32(table + id * 6u);
                for (u32 index = 0; index < (u32)(u8)count; ++index) {
                    u32 item = entries + index * 9u;
                    year = R16(item + 2u);
                    if (lower_year > year)
                        continue;
                    if (lower_year == year) {
                        s16 lower_day = (s16)R16((u32)local.before);
                        s16 day = (s16)R16(item);
                        if (lower_day > day)
                            goto restore_lower_year;
                    }
                    year = R16(item + 2u);
                    u16 upper_year = R16((u32)local.after + 2u);
                    if (upper_year < year) {
                        lower_year = R16((u32)local.before + 2u);
                        break;
                    }
                    if (upper_year == year) {
                        s16 upper_day = (s16)R16((u32)local.after);
                        s16 day = (s16)R16(item);
                        if (upper_day < day) {
                            lower_year = R16((u32)local.before + 2u);
                            break;
                        }
                    }
                    u8 amount = R8(item + 8u);
                    u8 old_total = *total;
                    *total = (u8)(old_total + amount);
restore_lower_year:
                    lower_year = R16((u32)local.before + 2u);
                }
                if ((s8)*total > 2) {
                    result = 0;
                    goto done;
                }
                self = local.cached_self;
                national_limit = R32(0x00ae235cu);
            }
        }
    }

    value = R32(record + 0x20u);
    if (value != 0) {
        u32 nations = R32(0x00ae2350u);
        u32 id = R32(value);
        if ((s32)id < (s32)(national_limit - nations * 2u)) {
            table = R32(self + 0x20u);
            *total = 0;
            count = (s8)R8(table + id * 6u + 5u);
            if (count > 0) {
                entries = R32(table + id * 6u);
                for (u32 index = 0; index < (u32)(u8)count; ++index) {
                    u32 item = entries + index * 9u;
                    year = R16(item + 2u);
                    if (lower_year > year)
                        continue;
                    if (lower_year == year) {
                        s16 lower_day = (s16)R16((u32)local.before);
                        s16 day = (s16)R16(item);
                        if (lower_day > day)
                            continue;
                    }
                    year = R16(item + 2u);
                    u16 upper_year = R16((u32)local.after + 2u);
                    if (upper_year < year)
                        break;
                    if (upper_year == year) {
                        s16 upper_day = (s16)R16((u32)local.after);
                        s16 day = (s16)R16(item);
                        if (upper_day < day)
                            break;
                    }
                    u8 amount = R8(item + 8u);
                    u8 old_total = *total;
                    *total = (u8)(old_total + amount);
                }
                if ((s8)*total > 2)
                    result = 0;
            }
        }
    }
done:
    CM_FS(u32, 0) = local.seh.prev;
    return result;
}
