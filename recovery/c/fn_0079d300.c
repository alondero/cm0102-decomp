/* WIP: regs (EAX high word) mismatch, case 13: the null-argument path returns the
   incoming EAX high word, which plain C cannot read. Blocker recorded as evidence-needed.
   fn_0079d300 (module unknown), preferred-image VA 0x0079d300, 170 bytes.
   Convention: thiscall (ECX = this) with one stack argument (ret 4).
   Return: EAX at full width. Failure paths use `xor ax, ax` and the cap path uses
   `mov ax, 0x64`, so the high word of EAX is kept from the value already in EAX. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_004b0700(u32 v);
extern int cm_fn_0090cfc0(int param_1);

u32 CM_THISCALL cm_fn_0079d300(u32 self, u32 arg1)
{
    u32 key;
    u32 entry;
    u32 flag;
    u32 ptr;
    s32 idx;
    u32 block;
    s32 cnt;
    s32 n;
    u32 row;
    u32 value;
    u8 low;

    if (arg1 == 0) {
        /* 0x79d30d: no load happens; the function returns the incoming EAX with
           `xor ax, ax` applied. Its high word is an input that C cannot read. */
        return 0;
    }

    key = *(u32 *)arg1;
    entry = *(u32 *)(CM_GLOBAL(u32, 0x00adadfc) + key * 4);
    if (entry == 0) {
        return 0;
    }
    low = *(u8 *)(entry + 0x42);
    if (low == 4) {
        return entry & 0xffff0000u;
    }
    if (low == 2) {
        return entry & 0xffff0000u;
    }
    if (cm_fn_004b0700(key) == 0) {
        return 0;
    }

    ptr = *(u32 *)(arg1 + 0x5d);
    if (ptr == 0) {
        idx = cm_fn_0090cfc0((int)CM_GLOBAL(u32, 0x00ae2350));
    } else {
        idx = (s32)*(u32 *)ptr;
    }

    block = *(u32 *)(self + 0xc) + (u32)idx * 0x102u;
    cnt = (s32)*(s16 *)(block + 0x10);

    flag = 0;
    value = 0;
    if (cnt > 0) {
        row = block + 0x16;
        n = cnt;
        do {
            if (flag == 0 && *(u32 *)(row + 8) == *(u32 *)arg1) {
                value = *(u32 *)row;
                flag = 1;
            }
            row = row + 0x10;
            n = n - 1;
        } while (n != 0);
        if (flag != 0) {
            goto check;
        }
    }
    value = *(u32 *)(block + 4);

check:
    if ((s32)value > 100) {
        return (value & 0xffff0000u) | 0x64u;
    }
    return value;
}
