#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern void CM_THISCALL cm_fn_0090d860(u32, u32, u32);
extern void CM_THISCALL cm_fn_0090dae0(u32, u32, u32, u32, u32, u32);
extern void CM_STDCALL cm_fn_0090e760(u32, u32, u32, u32, u32);
extern u32 CM_STDCALL cm_fn_0090e9a0(u32, u32, u32);

/* Both original scans stop at the first unrelated eligible record. */
static u32 cm_scan(u32 club, u32 total)
{
    u32 index, record, seen = 0;
    for (index = 0; index != 50; ++index) {
        record = D(club + index * 4u + 0xd7);
        if (record != 0 && (s32)D(record) >= (s32)total) {
            if (D(record + 0x39) == club || D(record + 0x24) == club)
                seen = 1;
            else
                return 2;
        }
    }
    return seen;
}

/* Sole caller00484215 overwrites full EAX at0048421a before using it. */
void CM_THISCALL cm_fn_0090d4f0(u32 self, u32 club)
{
    /* L+0 and the original argument slot L+0xc are distinct outparameters.
       EBP retains the incoming club when the argument slot is overwritten. */
    u8 scratch[0x10];
    u32 local = (u32)scratch;
    u32 group, pointer, total, index, slot, node, record, old, side, first;
    u32 out_first, out_second, alternate;
    group = (u32)(s32)(s16)W(0x00b77c7a);
    pointer = D(0x00b74c26 + group * 768u);
    total = D(0x00ae2364);
    D(local + 0xc) = club;
    if (pointer == 0) {
        index = 16;
    } else {
        index = (self & 0xffff0000u) | (u16)((u16)total - W(pointer));
        --index;
    }
    D(local + 4) = index;
    slot = self + (u32)(s32)(s16)index * 4u + 0x18;
    old = D(slot);
    if (old == club && cm_scan(club, total) == 1)
        return;
    if (D(slot) != 0) {
        pointer = D(self + 0x5e);
        D(self + 0x66) = pointer;
        node = cm_fn_0068d1b0(self + 0x5c);
        while (node != 0) {
            record = D(node);
            old = D(slot);
            if (D(record + 0x1c) == old || D(record + 0x20) == old)
                break;
            node = cm_fn_0068d1b0(self + 0x5c);
        }
        if (node != 0) {
            pointer = D(node);
            first = (record & 0xffff0000u) | W(node + 4);
            old = D(slot);
            side = D(pointer + 0x1c) == old;
            cm_fn_0090e760(first, 1, side, local, local + 0xc);
            out_second = D(local + 0xc);
            out_first = D(local);
            old = D(slot);
            cm_fn_0090e9a0(old, out_first, out_second);
        } else {
            alternate = 0;
            do {
                if ((u16)alternate != W(local + 4)) {
                    pointer = D(self + alternate * 4u + 0x18);
                    old = D(slot);
                    if (pointer == old)
                        break;
                }
                ++alternate;
            } while ((s16)alternate < 16);
            if (alternate < 16) {
                cm_fn_0090e760(alternate, 0, 0, local, local + 0xc);
                out_second = D(local + 0xc);
                out_first = D(local);
                old = D(slot);
                cm_fn_0090e9a0(old, out_first, out_second);
            } else {
                old = D(slot);
                cm_fn_0090d860(self, old, 0);
            }
        }
        total = D(0x00ae2364);
    }
    if (cm_scan(club, total) != 1) {
        index = D(local + 4);
        cm_fn_0090e760(index, 0, 0, local, local + 0xc);
        out_second = D(local + 0xc);
        out_first = D(local);
        cm_fn_0090dae0(self, club, out_first, out_second, 0, 0);
    }
    D(slot) = club;
}
