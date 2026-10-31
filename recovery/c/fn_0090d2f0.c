#include "cmre.h"

/* Preferred-image VA 0x0090d2f0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All four nonrecursive indexed callers next call00944cf1. Its00944e58 path
   overwrites EAX at00944e81 or00944e84 before reading it. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern u32 CM_CDECL cm_fn_0053ca90(u32);
extern u32 CM_STDCALL cm_fn_0090e760(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0090dae0(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0068d0d0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0090d6b0(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

void CM_THISCALL cm_fn_0090d2f0(u32 self, volatile u32 first,
                               volatile u32 second, volatile u32 third)
{
    u8 scratch[0x214];
    u32 local = (u32)scratch, list = self + 0x5c, row, record, selection;
    u32 club, value, a, b, flag, left, right, i, text;
    u16 low;
    value = D(self + 0x5e);
    D(list + 0xa) = value;
    row = cm_fn_0068d1b0(list);
    record = first;
    while (row != 0) {
        if (record == D(row)) return;
        row = cm_fn_0068d1b0(list);
    }
    selection = second;
    if ((u16)selection == 0xffff) {
        club = D(record + 0x1c);
        value = cm_fn_00600a50(club, 0, 1, 0, 0);
        if (value != 0) {
            club = D(record + 0x1c);
        } else {
            club = D(record + 0x20);
            value = cm_fn_00600a50(club, 0, 1, 0, 0);
            if (value == 0) {
                selection = 0xfffffffeu;
                goto selected;
            }
            club = D(record + 0x20);
        }
        row = cm_fn_0053ca90(club);
        low = W(0x00ae2364);
        low = (u16)(low - W(row));
        selection = (selection & 0xffff0000u) | low;
        --selection;
    }
selected:
    cm_fn_0090e760(selection, 1, 1, local + 4, local + 8);
    flag = B(record + 0x40);
    b = D(local + 8);
    a = D(local + 4);
    club = D(record + 0x1c);
    cm_fn_0090dae0(self, club, a, b, record, flag == 0);
    cm_fn_0090e760(selection, 1, 0, local + 4, local + 8);
    flag = B(record + 0x41);
    b = D(local + 8);
    a = D(local + 4);
    club = D(record + 0x20);
    cm_fn_0090dae0(self, club, a, b, record, flag == 0);
    D(local + 0xc) = record;
    W(local + 0x10) = (u16)selection;
    cm_fn_0068d0d0(list, local + 0xc, 6);
    flag = *(volatile u8 *)&third;
    B(local + 2) = 0;
    B(local + 3) = 0;
    if (flag != 0) return;
    right = D(record + 0x20);
    left = D(record + 0x1c);
    right += 0xd7;
    left += 0xd7;
    for (i = 50; i != 0; --i) {
        if (D(left) != 0) B(local + 2) = (u8)(B(local + 2) + 1);
        if (D(right) != 0) B(local + 3) = (u8)(B(local + 3) + 1);
        left += 4; right += 4;
    }
    flag = B(local + 2);
    if ((s8)flag >= 15 && (s8)B(local + 3) >= 15) return;
    cm_fn_00944cff(0x00ad439c, 0, 0, local + 0x114, local + 0x14);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x114, local + 0x14, 0xc3);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    cm_fn_0090d6b0(self, record);
    cm_fn_0090d2f0(self, record, selection, 1);
}
