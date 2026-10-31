#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_005a0590(u32, u32);
extern unsigned long long CM_STDCALL cm_fn_00945e80(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00541f90(u32, u32, u32);
extern void CM_CDECL cm_fn_006e2000(u32, u32, u32, u32, u32, u32, u32, u32,
    u32, u32, u32, u32, u32, u32, u32);
/* 006e6340 consumes the second slot through BL/BX or masks <=0x1000;
   later direct reads use AL or &0x800, before the slot is reused as scratch.
   The original caller's upper ECX fragment is dead at this boundary. */
extern u32 CM_CDECL cm_fn_006e6340(u32, u16, u32, u32, u32, u32);

u32 CM_CDECL cm_fn_004c0720(u32 club)
{
    u8 scratch[0x1f0];
    u32 local = (u32)scratch;
    u32 score = 0, context, record, high, low, shifted, value, index, selected;
    u32 cursor, role_cursor, result_cursor, remaining, count, role;
    long long product;
    if (club == 0)
        return 0xff676980u;
    context = D(0x00ae2a38);
    if (context == 0) {
        value = D(club + 0x65);
        product = (long long)(s32)value * 0x51eb851fll;
        high = (u32)((unsigned long long)product >> 32);
        shifted = (high >> 7) | ((high & 0x80000000u) != 0 ? 0xfe000000u : 0);
        score = shifted + (shifted >> 31);
    } else {
        record = cm_fn_005a0590(context, club);
        if (record != 0) {
            high = D(record + 4);
            low = D(record);
            score = (u32)cm_fn_00945e80(low, high, 300, 0);
        }
    }
    value = (u32)(s32)(s16)W(club + 0x80);
    score += value * 16u;
    for (index = 0; index != 50; ++index)
        D(local + 0x128 + index * 4u) = 0;
    for (index = 0; index != 50; ++index)
        D(local + 0x60 + index * 4u) = 0;
    for (index = 0; index != 15; ++index)
        D(local + 0x24 + index * 4u) = 0;
    W(local + 0xa) = 0x204;
    W(local + 0xc) = 0x204;
    W(local + 0xe) = 0x204;
    W(local + 0x14) = 0x210;
    W(local + 0x16) = 0x210;
    W(local + 0x18) = 0x210;
    W(local + 4) = 1;
    W(local + 6) = 0x804;
    W(local + 8) = 0x84;
    W(local + 0x10) = 0x810;
    W(local + 0x12) = 0x90;
    W(local + 0x1a) = 0x40;
    W(local + 0x1c) = 0x40;
    W(local + 0x1e) = 0x40;
    W(local + 0x20) = 0x40;
    B(local) = 0;
    cm_fn_00541f90(club, local + 0x60, 0);
    selected = 0;
    cursor = local + 0x60;
    for (index = 0; index != 50; ++index, cursor += 4) {
        record = D(cursor);
        if (record != 0) {
            value = D(record + 0x61);
            if (value != 0) {
                value = (u32)(s32)(s8)selected;
                ++selected;
                D(local + 0x128 + value * 4u) = record;
            }
        }
    }
    B(local) = (u8)selected;
    /* Only the low byte was initialized; retain the original DWORD read. */
    count = D(local);
    cm_fn_006e2000(local + 0x128, count, local + 4, 15, 0, local + 0x24,
                  0x840, club, 0, 0, 0, 0, 0, 0, 0);
    result_cursor = local + 0x24;
    role_cursor = local + 4;
    D(local) = 15;
    do {
        record = D(result_cursor);
        if (record != 0) {
            role = W(role_cursor);
            value = cm_fn_006e6340(record, (u16)role, 0x840, club, 0xffffffffu, 0);
            score += value;
        }
        remaining = D(local);
        role_cursor += 2;
        result_cursor += 4;
        --remaining;
        D(local) = remaining;
    } while (remaining != 0);
    return score;
}
