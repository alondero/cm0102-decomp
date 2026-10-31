#include "cmre.h"

/* Preferred-image VA 0x00896c60, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Outer/inner counters retain BYTE writes followed by DWORD loads. The
   original 00897990/00897bb0 consume signed low BYTE row/column indices.
   Clearing the two auxiliary slots does not itself set the result flag. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_008982b0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00897dc0(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00897ee0(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00897990(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00897bb0(u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00896c60(u32 self, u32 club, volatile u32 person_arg,
                             volatile u32 last_arg, volatile u32 option)
{
    u8 scratch[0x20c];
    u32 local = (u32)scratch, person, current_last, index, offset, walk;
    u32 base, slot, value, wanted, row, column, next_last, text, name, ext, line;
    u8 low;
    if (club == 0) {
        name = local + 0x10c; ext = local + 0x0c; line = 0x34d;
        goto error;
    }
    person = person_arg;
    if (person == 0) {
        name = local + 0x0c; ext = local + 0x10c; line = 0x355;
        goto error;
    }
    current_last = last_arg;
    index = cm_fn_008982b0(self, club, current_last);
    if (index == 0xffffffffu) return 0;
    offset = index * 145u;
    D(local + 4) = 0;
    D(local) = offset;
    walk = 0;
    for (;;) {
        base = D(self + 0x601);
        slot = offset + walk + base + 2;
        value = D(slot);
        wanted = D(person);
        if (value == wanted) {
            D(slot) = 0xffffffffu;
            D(local + 4) = 1;
        }
        walk += 4;
        if (walk == 0x50) break;
        offset = D(local);
    }
    if (option != 0) {
        value = cm_fn_00897dc0(self, club, 0, current_last);
        if (value == person) cm_fn_00897ee0(self, club, 0, 0, current_last);
        value = cm_fn_00897dc0(self, club, 1, current_last);
        if (value == person) cm_fn_00897ee0(self, club, 1, 0, current_last);
        B(local) = 0;
        do {
            row = D(local);
            B(local + 8) = 0;
            for (;;) {
                column = D(local + 8);
                value = cm_fn_00897990(self, club, row, column, current_last);
                wanted = person_arg;
                if (value == wanted) {
                    next_last = last_arg;
                    cm_fn_00897bb0(self, club, row, column, 0, next_last);
                    D(local + 4) = 1;
                }
                low = (u8)(B(local + 8) + 1u);
                B(local + 8) = low;
                if ((s8)low >= 2) break;
                current_last = last_arg;
            }
            low = (u8)(B(local) + 1u);
            current_last = last_arg;
            B(local) = low;
        } while ((s8)low < 5);
    }
    if (B(self + 0x5f0) != 0) {
        slot = self + 0x38;
        for (walk = 16; walk != 0; --walk) {
            if (D(slot) != 0 && D(slot - 8) == club && D(slot - 4) == current_last) {
                D(slot) = 0;
                low = B(self + 0x5f0);
                B(self + 0x5f0) = (u8)(low - 1u);
            }
            slot += 0x5c;
        }
    }
    return D(local + 4);
error:
    cm_fn_00944cff(0x00a8b908, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
