/* WIP: coverage 0.4729; actual vtable slot 0096b4f0 -> 005e8bf0
   is absent from the pinned function inventory, so the original callback is
   an interior boundary. Three failed C test invocations; equal 2M budgets
   resolve the observed copy-loop asymmetry, with zero inconclusive cases.
   Preferred-image VA 005ea060, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole direct caller overwrites EAX at its next call. Scratch offsets are
   relative to ESP immediately after SUB ESP,220h, before register saves. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define LD(o) (*(volatile u32 *)(work + (o)))
#define LW(o) (*(volatile u16 *)(work + (o)))
#define LB(o) (*(volatile u8 *)(work + (o)))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 allocation);
extern void CM_CDECL cm_fn_00945333(u32 base, u32 count, u32 size, u32 comparator);
extern void CM_THISCALL cm_fn_004a2540(u32 self, u32 club, u8 rank);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);
typedef u32 (CM_THISCALL *StageMethod)(u32, u32, void *, void *, u32);

void CM_THISCALL cm_fn_005ea060(u32 self)
{
    u8 work[0x220];
    u8 *name, *extension;
    u32 allocation, count, table, value, copy_counter, index, vtable;
    u32 row, remaining, selected = 0, local_counter, seed, receiver;
    u32 result, line, text;
    u8 rank;

    LB(4) = 1;
    LB(0) = 1;
    if (D(self + 8u) == 0) {
        name = work + 0x120;
        extension = work + 0x20;
        line = 0x3f0u;
        goto diagnostic;
    }
    count = D(self + 0xf2u);
    allocation = (u32)cm_fn_00944e46(count * 4u);
    LD(8) = allocation;
    if (allocation == 0) {
        name = work + 0x20;
        extension = work + 0x120;
        line = 0x3f8u;
        goto diagnostic;
    }
    count = D(self + 0xf2u);
    copy_counter = 0;
    if ((s32)count > 0) {
        index = 0;
        do {
            table = D(self + 0xeeu);
            copy_counter += 1u;
            value = D(table + index * 4u);
            D(allocation + index * 4u) = value;
            count = D(self + 0xf2u);
            index = (u32)(s32)(s16)copy_counter;
        } while ((s32)index < (s32)count);
    }
    count = D(self + 0xf2u);
    cm_fn_00945333(allocation, count, 4, 0x004b07d0u);
    vtable = D(self);
    result = ((StageMethod)D(vtable + 0x3cu))(self, 10, work + 0x14, work + 0x1e, 0);
    count = LD(0x14);
    LD(0x18) = result;
    index = count - 1u;
    if ((s16)index < 0)
        goto remainder;
    index = (u32)(s32)(s16)index;
    LD(0xc) = index;
    row = result + index * 104u + 0x1au;
    LD(0x10) = index + 1u;
    do {
        count = (u32)(s32)(s16)LW(0x14) - 1u;
        if (index == count && (s16)W(row) > 0) {
            local_counter = 0;
            do {
                table = LD(8);
                seed = LD(4);
                value = D(table + (u32)(s32)(s16)selected * 4u);
                selected += 1u;
                receiver = D(self + 8u);
                cm_fn_004a2540(receiver, value, (u8)seed);
                rank = LB(0);
                rank = (u8)(rank + 1u);
                local_counter += 1u;
                count = W(row);
                LB(0) = rank;
            } while ((s16)local_counter < (s16)count);
            index = LD(0xc);
        }
        rank = LB(0);
        local_counter = 0;
        count = W(row);
        LB(4) = rank;
        if ((s16)count > 0) {
            do {
                table = LD(8);
                seed = LD(4);
                receiver = D(self + 8u);
                value = D(table + (u32)(s32)(s16)selected * 4u);
                selected += 1u;
                cm_fn_004a2540(receiver, value, (u8)seed);
                rank = LB(0);
                rank = (u8)(rank + 1u);
                local_counter += 1u;
                count = W(row);
                LB(0) = rank;
            } while ((s16)local_counter < (s16)count);
            index = LD(0xc);
        }
        remaining = LD(0x10);
        index -= 1u;
        row -= 104u;
        remaining -= 1u;
        LD(0xc) = index;
        LD(0x10) = remaining;
    } while (remaining != 0);
    allocation = LD(8);

remainder:
    count = D(self + 0xf2u);
    rank = LB(0);
    index = (u32)(s32)(s16)selected;
    LB(4) = rank;
    if ((s32)index < (s32)count) {
        seed = LD(4);
        do {
            value = D(allocation + index * 4u);
            receiver = D(self + 8u);
            cm_fn_004a2540(receiver, value, (u8)seed);
            rank = LB(0);
            rank = (u8)(rank + 1u);
            LB(0) = rank;
            count = LD(0);
            if ((count & 0xffu) == (seed & 0xffu) + 11u) {
                rank = LB(0);
                LB(4) = rank;
                seed = LD(4);
            }
            count = D(self + 0xf2u);
            selected += 1u;
            index = (u32)(s32)(s16)selected;
        } while ((s32)index < (s32)count);
    }
    value = LD(0x18);
    cm_fn_009452ca(value);
    cm_fn_009452ca(allocation);
    return;

diagnostic:
    cm_fn_00944cff(0x009cdab0u, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
}
