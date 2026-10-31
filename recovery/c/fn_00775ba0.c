#include "cmre.h"

/* Thiscall, one stack argument (ret 4): the record pointer rec. Fields used:
   rec+0x1a (pointer to a u32 index), rec+0x61 (pointer to a table with u16 at
   +9), rec+0x24 (pointer to a 50-slot array at +0xd7), rec+0x5e (flag byte).
   Object (ECX) fields: +0x04 (base for the index), +0x0c (slot array pointer).
   Entries are 4-byte cells stored after the count byte; 0x0076d810 is stdcall. */

extern u8 CM_STDCALL cm_fn_0076d810(u32 rec, u32 entry, u32 zero);

void CM_THISCALL cm_fn_00775ba0(u32 self, u32 rec)
{
    u32 index_ptr;
    u32 v;
    u32 q;
    u32 entry;
    s32 cnt;
    u16 rec16;
    u32 base;
    u32 sum;
    u32 ptr_last;
    u32 last;
    u32 total;
    u32 arr;
    u32 idx;
    u32 slot;
    u8 al;
    u32 p;
    u32 scan;
    u32 count2;
    u32 i;

    index_ptr = *(volatile u32 *)(rec + 0x1au);
    if (index_ptr == 0) {
        return;
    }
    q = *(volatile u32 *)(rec + 0x61u);
    if (q == 0) {
        return;
    }

    v = *(volatile u32 *)index_ptr;
    entry = *(volatile u32 *)(self + 4u) + 110u * v;

    cnt = (s8)cm_fn_0076d810(rec, entry, 0u);

    q = *(volatile u32 *)(rec + 0x61u);
    rec16 = *(volatile u16 *)(entry + (u32)cnt * 4u + 0xfu);
    if ((s16)rec16 >= (s16)*(volatile u16 *)(q + 9u)) {
        goto search_done;
    }

    base = *(volatile u32 *)(entry + 8u);
    sum = base;
    if (cnt > 0) {
        p = entry + 0xeu;
        count2 = (u32)cnt;
        do {
            u8 b = *(volatile u8 *)p;
            p = p + 4u;
            sum = sum + b;
            count2 = count2 - 1u;
        } while (count2 != 0u);
    }

    last = *(volatile u8 *)(entry + (u32)cnt * 4u + 0xdu);
    ptr_last = entry + (u32)cnt * 4u + 0xdu;
    total = last + sum;
    base = *(volatile u32 *)(entry + 8u);
    if (cnt > 0) {
        p = entry + 0xeu;
        count2 = (u32)cnt;
        do {
            u8 b = *(volatile u8 *)p;
            p = p + 4u;
            base = base + b;
            count2 = count2 - 1u;
        } while (count2 != 0u);
    }

    if ((s32)base >= (s32)total) {
        goto search_done;
    }

    arr = *(volatile u32 *)(self + 0x0cu);
    scan = arr + base * 4u;
    for (i = base; (s32)i < (s32)total; i++) {
        if (*(volatile u32 *)scan == rec) {
            *(volatile u8 *)ptr_last = (u8)(last - 1u);
            arr = *(volatile u32 *)(self + 0x0cu);
            idx = *(volatile u32 *)(arr + total * 4u - 4u);
            *(volatile u32 *)(arr + i * 4u) = idx;
            break;
        }
        scan = scan + 4u;
    }
search_done:
    slot = *(volatile u32 *)(rec + 0x24u);
    if (slot == 0) {
        return;
    }
    for (al = 0; (s8)al < 0x32; al++) {
        if (*(volatile u32 *)(slot + (u32)(s32)(s8)al * 4u + 0xd7u) == rec) {
            *(volatile u32 *)(slot + (u32)(s32)(s8)al * 4u + 0xd7u) = 0;
            al = *(volatile u8 *)(rec + 0x5eu) & 3u;
            *(volatile u32 *)(rec + 0x24u) = 0;
            *(volatile u8 *)(rec + 0x5eu) = al;
            return;
        }
    }
    al = *(volatile u8 *)(rec + 0x5eu);
    *(volatile u32 *)(rec + 0x24u) = 0;
    al = al & 3u;
    *(volatile u8 *)(rec + 0x5eu) = al;
}
