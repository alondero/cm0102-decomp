#include "cmre.h"

/* Preferred-image VA 0x007f7270, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Raw counter slots receive only BYTE stores; 7f6cd0 consumes their low
   signed BYTE. Keep the two separate 7ae950 calls in the assignment scan. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007f6cd0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ae950(u32);
extern u32 CM_THISCALL cm_fn_007ae8a0(u32, u32);
extern u32 CM_CDECL cm_fn_0053cd60(u32, u32);
extern u32 CM_THISCALL cm_fn_008043c0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_007f7270(u32 self, u32 person)
{
    u8 scratch[0x230];
    u32 local = (u32)scratch, selected, club, task, assigned, value, cursor, remaining;
    u32 raw, text, name, ext, line;
    u8 index, free_index, day, total;
    D(local + 4) = self;
    if (person == 0) {
        selected = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + selected * 768u);
        if (person == 0) {
            name = local + 0x130; ext = local + 0x30; line = 0x1ba;
            goto error;
        }
    }
    index = 0;
    club = D(person + 0x39);
    B(local) = index;
    do {
        raw = D(local);
        value = D(local + 4);
        task = cm_fn_007f6cd0(value, raw, person);
        if (task != 0 && B(task + 10) == 4) {
            assigned = cm_fn_007ae950(task);
            if (club == 0 || (assigned != 0 &&
                (D(assigned + 0x39) != club || B(assigned + 0x3d) != 9)))
                cm_fn_007ae8a0(task, 0);
        }
        index = (u8)(index + 1u);
        B(local) = index;
    } while ((s8)index < 10);
    if (club != 0) {
        cm_fn_0053cd60(club, local + 0x14);
        D(local) = local + 0x14;
        D(local + 0x10) = 7;
        do {
            cursor = D(local);
            club = D(cursor);
            if (club != 0) {
                index = 0;
                B(local + 8) = 0xff;
                B(local + 12) = 0;
                do {
                    raw = D(local + 12);
                    value = D(local + 4);
                    task = cm_fn_007f6cd0(value, raw, person);
                    if (task != 0 && B(task + 10) == 4) {
                        assigned = cm_fn_007ae950(task);
                        if (assigned == club) goto next_club;
                        assigned = cm_fn_007ae950(task);
                        if (assigned == 0 && B(local + 8) == 0xff)
                            B(local + 8) = index;
                    }
                    index = (u8)(index + 1u);
                    B(local + 12) = index;
                } while ((s8)index < 10);
                free_index = B(local + 8);
                if ((s8)free_index > -1) {
                    raw = D(local + 8);
                    value = D(local + 4);
                    task = cm_fn_007f6cd0(value, raw, person);
                    cm_fn_007ae8a0(task, club);
                }
            }
next_club:
            cursor = D(local);
            remaining = D(local + 0x10);
            D(local) = cursor + 4u;
            D(local + 0x10) = remaining - 1u;
        } while (remaining != 1);
    }
    day = B(person);
    total = B(0x00ae2364);
    day = (u8)(day - total + 16u);
    if ((s8)day < 0 || (s8)day > 15) {
        name = local + 0x30; ext = local + 0x130; line = 0x212;
        goto error;
    }
    value = D(local + 4);
    value = D(value + (u32)(s32)(s8)day * 4u + 12);
    if (value != 0) cm_fn_008043c0(value);
    return 1;
error:
    cm_fn_00944cff(0x00a7b094, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
