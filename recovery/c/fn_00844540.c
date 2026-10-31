#include "cmre.h"
/* Preferred-image VA 0x00844540, image base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The date helper returns the address read immediately after its call. */
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008c5cf0(u32, u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_00844540(u32 self, u32 club, u8 filter)
{
    u8 scratch[0x14];
    u32 work = (u32)scratch;
    u32 id, table, row, offset = 0, remaining = 40, pointer, low, high, person;
    u32 receiver, record, base;
    u16 year, limit;
    id = D(club);
    B(work + 3) = 0;
    base = D(self + 0xdc);
    row = base + id * 9u;
    do {
        table = D(row);
        record = table + offset;
        if ((s32)D(record + 0xf) < 0) goto next;
        if ((s32)D(record + 0x17) < 0) goto next;
        if (filter != 0xff && B(record) != filter) goto next;
        if (B(row + 4) != 2) goto count;
        if (!cm_fn_00600a50(club, 1, 1, 0, 0)) goto count;
        pointer = cm_fn_0054c8e0(0x00ae2c90, work + 0xc, 20);
        low = D(pointer);
        high = D(pointer + 4);
        D(work + 8) = high;
        person = D(club + 0xcf) + 0x3e;
        D(work + 4) = low;
        year = W(work + 6);
        limit = W(person + 2);
        if (year < limit) goto count;
        if (year == limit && (s16)low <= (s16)W(person)) goto count;
        table = D(row);
        id = D(table + offset + 0xf);
        base = D(0x00ae23bc);
        person = base + id * 110u;
        receiver = D(0x00de1f54);
        if (cm_fn_008c5cf0(receiver, person, 0, 0)) goto count;
        table = D(row);
        D(table + offset + 0xf) = 0xffffffffu;
        table = D(row);
        D(table + offset + 0x17) = 0xffffffffu;
        goto next;
count:
        B(work + 3) = (u8)(B(work + 3) + 1);
next:
        offset += 0x25;
        --remaining;
    } while (remaining);
    return B(work + 3);
}
