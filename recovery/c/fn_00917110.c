#include "cmre.h"

/* Preferred-image VA 0x00917110, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The virtual call has four stack arguments, including the earlier PUSH 0.
   The six-club allocation is not initialized beyond original stores. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern void CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32, u32, s16, u32, s16, u32, u32,
                                     u16, u8, u8, u16, u8, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0051c800(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00917110(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x40c];
    u32 prev = CM_FS(u32, 0), local = (u32)scratch;
    u32 clubs, table, slot, value, counter = 0, index, offset, row;
    u32 stages, stage, schedule, vtable, target, owner, rounds, round_flag;
    u32 text, name, ext, line, result;
    u8 flag;
    u16 year;
    frame.state = 0xffffffffu;
    frame.handler = 0x009658be;
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    clubs = cm_fn_00944e46(24);
    if (clubs == 0) {
        name = local + 0x20c; ext = local + 12; line = 0x5ff;
        goto error;
    }
    table = D(self + 0xb2);
    index = 25;
    slot = table + 100;
    do {
        value = D(slot);
        if (value != 0) {
            value = D(table + index * 4u);
            counter = 1;
            D(clubs) = value;
            table = D(self + 0xb2);
            D(table + index * 4u) = 0;
            break;
        }
        ++index;
        slot += 4;
    } while ((s32)index < 35);
    index = 0;
    if ((s16)W(self + 0x3e) > 0) {
        offset = 0;
        do {
            row = D(self + 0xa7);
            row += offset;
            if (B(row + 5) == 2) {
                value = D(row);
                slot = (u32)(s32)(s16)counter;
                ++counter;
                D(clubs + slot * 4u) = value;
            }
            value = (u32)(s32)(s16)W(self + 0x3e);
            ++index;
            offset += 6;
        } while ((s32)index < (s32)value);
    }
    stages = D(self + 12);
    stage = D(stages);
    slot = (u32)(s32)(s16)counter;
    table = D(stage + 0xb1);
    ++counter;
    value = D(table + 59);
    D(clubs + slot * 4u) = value;
    stages = D(self + 12);
    stage = D(stages + 4);
    slot = (u32)(s32)(s16)counter;
    table = D(stage + 0xb1);
    value = D(table + 59);
    D(clubs + slot * 4u) = value;
    vtable = D(self);
    target = D(vtable + 0x3c);
    schedule = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))target)
        (self, 2, local, local + 4, 0);
    stage = cm_fn_00944cf1(0xb2);
    D(local + 8) = stage;
    frame.state = 0;
    if (stage != 0) {
        flag = B(self + 0x51);
        round_flag = D(local + 4);
        year = W(self + 0x40);
        owner = D(self + 4);
        rounds = D(local);
        stage = cm_fn_00522e00(stage, self, 6, clubs, (s16)rounds, owner,
                              schedule, year, 2, 4, (u16)round_flag, flag, 0, 0, 0, 0);
    }
    stages = D(self + 12);
    frame.state = 0xffffffffu;
    D(stages + 8) = stage;
    cm_fn_009452ca(clubs);
    cm_fn_009452ca(schedule);
    stages = D(self + 12);
    value = D(stages + 8);
    if (value == 0) {
        name = local + 0x30c; ext = local + 0x10c; line = 0x629;
        goto error;
    }
    D(self + 0x30) = 2;
    stage = D(stages + 8);
    result = cm_fn_0051c800(stage, 0);
    goto done;
error:
    cm_fn_00944cff(0x00ad4850, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
