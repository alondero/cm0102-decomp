/* Preferred-image VA 00630af0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0062c680(u32 self, u32 record);
extern void CM_THISCALL cm_fn_0054cc30(u32 self, void *output, u32 first, u32 second);
extern unsigned long long CM_CDECL cm_fn_007e5780(s32 remaining, u32 destination, s32 capacity, u32 mode);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00630af0(u32 self, u32 record, volatile u32 second,
                              volatile u32 third, volatile u8 fourth, volatile u32 fifth)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x208];
    u32 previous, id, total, table, injury, high, low, packed, day, elapsed;
    u32 threshold, mode, remaining, capacity, destination, result, version;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095b1abu;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    if (record == 0) {
        cm_fn_00944cff(0x009e7f28u, 0, 0, work + 0x108, work + 8);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x108, work + 8, 0x106du);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        result = 0;
        goto done;
    }
    id = D(record);
    total = D(0x00ae2364u);
    if ((s32)id >= (s32)total) {
        result = 0;
        goto done;
    }
    cm_fn_0062c680(self, record);
    id = D(record);
    table = D(self);
    injury = D(table + id * 31u + 14u);
    if (injury == 0xffffffffu) {
        destination = second;
        B(destination) = 0;
        result = 0;
        goto done;
    }
    table = D(self + 4u);
    injury = table + injury * 48u;
    high = D(injury + 0x24u);
    low = D(injury + 0x20u);
    cm_fn_0054cc30(0x00ae2c90u, work, low, high);
    packed = D((u32)work + 2u);
    frame.state = 0;
    day = (u32)(s32)(s16)W((u32)work);
    elapsed = (packed & 0xffffu) * 365u + day;
    if (fourth == 0)
        threshold = (u32)(s32)(s16)W(injury + 0x2au);
    else
        threshold = (u32)(s32)(s16)W(injury + 0x28u);
    mode = fifth;
    remaining = threshold - elapsed;
    capacity = third;
    destination = second;
    result = (u32)cm_fn_007e5780((s32)remaining, destination, (s32)capacity, mode);
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
