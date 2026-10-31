/* Preferred-image VA 0042e980, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original00522e31 reads only the WORD of the year stack argument. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);
extern u32 CM_THISCALL cm_fn_00522e00(u32 self, u32 owner, u32 count, void *clubs, u32 rounds, u32 competition, u32 schedule, u32 year, u32 flag_a, u32 flag_b, u32 round_flag, u32 flag_c, u32 builder, u32 numbered, u32 value, u32 club_flags);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0042e980(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x214];
    u32 previous, index, holder, child, table, value, vtable, target, schedule;
    u32 allocation, round_flag, year, competition, rounds, result, version;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095459eu;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    for (index = 8; index < 10; ++index) {
        holder = D(self + 0xcu);
        child = D(holder + index * 4u);
        table = D(child + 0xb1u);
        value = D(table);
        D((u32)work + 12u + (index - 8u) * 4u) = value;
    }
    vtable = D(self);
    target = D(vtable + 0x3cu);
    schedule = ((u32 (CM_THISCALL *)(u32, u32, void *, void *, u32))target)(self, 10, work + 4, work, 0);
    allocation = (u32)cm_fn_00944cf1(0xb2u);
    D((u32)work + 8u) = allocation;
    frame.state = 0;
    if (allocation != 0) {
        round_flag = D((u32)work);
        year = W(self + 0x40u);
        competition = D(self + 4u);
        rounds = D((u32)work + 4u);
        result = cm_fn_00522e00(allocation, self, 2, work + 12, rounds, competition, schedule,
                               year, 10, 1, round_flag, 20, 0, 0, 0, 0);
    } else {
        result = 0;
    }
    holder = D(self + 0xcu);
    frame.state = 0xffffffffu;
    D(holder + 0x28u) = result;
    holder = D(self + 0xcu);
    result = D(holder + 0x28u);
    if (result == 0) {
        cm_fn_00944cff(0x0098aba4u, 0, 0, work + 0x114, work + 0x14);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x114, work + 0x14, 0x392u);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
    } else {
        result = (u32)cm_fn_009452ca(schedule);
        D(self + 0x30u) = 10;
    }
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
