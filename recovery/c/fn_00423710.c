/* WIP: case17 outcome mismatch; indirect RET0 leaves native stack arguments. */
/* Preferred-image VA 00423710, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);
extern u32 CM_THISCALL cm_fn_00522e00(u32 self, u32 owner, u32 count, void *clubs, u32 rounds, u32 competition, u32 schedule, u32 year, u32 flag_a, u32 flag_b, u32 round_flag, u32 flag_c, u32 builder, u32 numbered, u32 value, u32 club_flags);
extern u32 CM_THISCALL cm_fn_0051c800(u32 self, u32 option);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00423710(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x214];
    u32 previous, table, holder, value, vtable, target, schedule, allocation, result;
    u32 round_flag, raw_year, competition, rounds, object, version;
    unsigned long long wide;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095417eu;
    previous = CM_FS(volatile u32, 0);
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    table = D(self + 0xb1u);
    holder = D(self + 0xcu);
    value = D(table);
    D((u32)work + 8u) = value;
    value = D(holder);
    table = D(value + 0xb1u);
    vtable = D(self);
    value = D(table);
    D((u32)work + 12u) = value;
    target = D(vtable + 0x3cu);
    schedule = ((u32 (CM_THISCALL *)(u32, u32, void *, void *, u32))target)(self, 7, work, work + 4, 0);
    wide = cm_fn_00944cf1(0xb2u);
    allocation = (u32)wide;
    D((u32)work + 0x10u) = allocation;
    frame.state = 0;
    if (allocation != 0) {
        round_flag = D((u32)work + 4u);
        raw_year = ((u32)(wide >> 32) & 0xffff0000u) | W(self + 0x40u);
        competition = D(self + 4u);
        rounds = D((u32)work);
        result = cm_fn_00522e00(allocation, self, 2, work + 8, rounds, competition, schedule,
                               raw_year, 7, 1, round_flag, 20, 1, 0, 0, 0);
    } else {
        result = 0;
    }
    holder = D(self + 0xcu);
    frame.state = 0xffffffffu;
    D(holder + 0x1cu) = result;
    holder = D(self + 0xcu);
    object = D(holder + 0x1cu);
    if (object == 0) {
        cm_fn_00944cff(0x00989d04u, 0, 0, work + 0x114, work + 0x14);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x114, work + 0x14, 0x59au);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
    } else {
        cm_fn_0051c800(object, 0);
        result = (u32)cm_fn_009452ca(schedule);
        D(self + 0x30u) = 7;
    }
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
