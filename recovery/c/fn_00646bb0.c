/* WIP: -O0 case 323 differs on non-stack memory; -O2 passes. Original
   weight reads at VA 0x00646c6d/71 extend beyond the 0x270-byte scratch area
   into caller stack bytes (trace events 4886/4887 read 3 and 204). Do not
   initialize unknown weights or tune the C frame to match one build. */
#include "cmre.h"

/* Preferred-image VA 0x00646bb0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve the contiguous weights, temporary list and report buffers. Indices
   are sign-extended low words and addresses use wrapping DWORD arithmetic.
   No direct callers establish a narrower return; retain original full EAX. */
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32, u32, s16, u32, s16, u32,
                                    u32, u16, u8, u8, u16, u8,
                                    u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0051c800(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_00646bb0(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x270];
    u32 work = (u32)scratch, teams, index = 0, row, offset, base;
    u32 count = 0, current, value, other, list_index, eax, table, target;
    u32 result, allocation, round_data, first, second, owner;
    u16 score, year;
    char *version;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095ba4eu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    B(work + 5u) = 2;
    B(work + 6u) = 2;
    B(work + 7u) = 2;
    B(work + 20u) = 3;
    B(work + 21u) = 3;
    B(work + 22u) = 3;
    B(work + 23u) = 3;
    teams = W(self + 0x3eu);
    D(work) = 0;
    B(work + 4u) = 1;
    for (row = 8; row < 20; ++row) B(work + row) = 0;
    if ((s16)teams <= 0) goto pairs_ready;
next_row:
    row = (u32)(s32)(s16)index;
    if ((s32)row < (s32)(s16)teams - 1) {
        base = D(self + 0xb1u);
        offset = row * 59u;
        score = W(base + offset + 0x19u);
        if (score != W(base + offset + 0x54u)) goto previous;
        value = B(work + row + 4u);
        if (value == B(work + row + 5u)) goto previous;
        list_index = (u32)(s32)(s16)D(work);
        current = D(base + offset);
        D(work + 32u + list_index * 4u) = current;
        value = D(work) + 1u;
        ++row;
        D(work) = value;
        list_index = (u32)(s32)(s16)value;
        other = D(base + row * 59u);
        count = D(work);
        D(work + 32u + list_index * 4u) = other;
        ++count;
        B(base + offset + 0x3au) = (u8)(index + 2u);
        base = D(self + 0xb1u);
        D(work) = count;
        ++index;
        B(base + offset + 0x74u) = (u8)index;
        base = D(self + 0xb1u);
        B(base + offset + 0x37u) = 1;
        base = D(self + 0xb1u);
        B(base + offset + 0x72u) = 1;
        goto advance;
    }
previous:
    if ((s16)index > 0) {
        offset = row * 59u;
        base = D(self + 0xb1u);
        score = W(base + offset + 0x19u);
        current = base + offset;
        if (score == W(current - 0x22u)) {
            B(current + 0x3au) = (u8)(index + 1u);
            base = D(self + 0xb1u);
            value = B(base + offset + 0x3au);
            B(base + offset + 0x39u) = (u8)value;
        }
    }
advance:
    teams = W(self + 0x3eu);
    ++index;
    if ((s16)index < (s16)teams) goto next_row;
    count = D(work);
pairs_ready:
    eax = (u32)((s32)(s16)count / 2);
    B(self + 0xeeu) = (u8)eax;
    if ((u8)eax == 0) {
        D(self + 0x2cu) = 0;
        D(self + 0x30u) = 0xffffffffu;
        D(self + 0xe6u) = 0;
        goto done;
    }
    table = D(self);
    target = D(table + 0x3cu);
    round_data = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))target)
        (self, 0, work + 24u, work + 28u, 0);
    allocation = (u32)cm_fn_00944cf1(0xb2);
    D(work) = allocation;
    frame.state = 0;
    if (allocation != 0) {
        second = D(work + 28u);
        year = W(self + 0x40u);
        owner = D(self + 4u);
        first = D(work + 24u);
        eax = cm_fn_00522e00(allocation, self, (s16)count, work + 32u,
                            (s16)first, owner, round_data, year, 0, 1,
                            (u16)second, 20, 0, 0, 0, 0);
    } else {
        eax = 0;
    }
    table = D(self + 12u);
    frame.state = 0xffffffffu;
    D(table) = eax;
    table = D(self + 12u);
    if (D(table) == 0) {
        cm_fn_00944cff(0x009ea384u, 0, 0, scratch + 0x170, scratch + 0x70);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                      scratch + 0x170, scratch + 0x70, 0x28e);
        eax = cm_fn_005e8290((const char *)0x009870e0u,
                            (const char *)0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
    } else {
        cm_fn_009452ca(round_data);
        table = D(self + 12u);
        result = D(table);
        eax = cm_fn_0051c800(result, 0);
        D(self + 0x30u) = 0;
        B(self + 0xc5u) = 6;
    }
done:
    CM_FS(u32, 0) = frame.prev;
    return eax;
}
