/* Preferred-image VA 007e6ba0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch offsets follow ESP after SUB ESP,28h, before four register saves. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define LD(o) (*(volatile u32 *)(work + (o)))
#define LW(o) (*(volatile u16 *)(work + (o)))
#define LB(o) (*(volatile u8 *)(work + (o)))
extern void CM_FASTCALL cm_fn_0054a110(void *self);
extern u32 CM_THISCALL cm_fn_00549f70(void *self, s16 day, u8 month, u16 year, u32 flag);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 day, u32 year);
extern void CM_THISCALL cm_fn_00417c10(void *self, void *output, u32 source);
extern u32 CM_CDECL cm_fn_00542740(u32 club);
extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 person);
extern u32 CM_CDECL cm_fn_0053ab90(u32 person, u32 date);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);

u32 CM_STDCALL cm_fn_007e6ba0(u32 object, u32 mode)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x28];
    u32 predecessor, club, nation, competition, id, target, pointer, first, second;
    u32 receiver, player, index, phase, result = 0;
    u16 year, day, first_year, second_year;
    u8 available, required, young;
    frame.state = 0xffffffffu;
    frame.handler = 0x009607f0u;
    predecessor = CM_FS(volatile u32, 0);
    frame.prev = predecessor;
    CM_FS(volatile u32, 0) = (u32)&frame;
    LB(7) = 0xffu;
    LB(6) = 0;
    LB(3) = 0;
    LB(5) = 0;
    cm_fn_0054a110(work + 8);
    frame.state = 0;
    LB(4) = 0;
    if (mode != 0) {
        if (W(object + 0x30u) != 0)
            goto done;
        club = D(object + 0x1cu);
        nation = D(club + 0x53u);
        id = D(nation);
        target = D(0x009cf4dcu);
        if (id == target) {
            competition = D(club + 0x57u);
            if (competition == 0)
                goto done;
            id = D(competition);
            target = D(0x009cf8d4u);
            if (id != target)
                goto done;
        }
        year = W(object + 0x32u);
        if (year == 0x418u || year == 0x419u || year == 0xa0u)
            goto done;
        year = W(object + 0x28u);
        pointer = cm_fn_00549f70(work + 0x18, 26, 11, year, 0xffffffffu);
        first = D(pointer);
        second = D(pointer + 4u);
        year = W(object + 0x28u);
        day = W(object + 0x2au);
        *(volatile u8 *)&frame.state = 1;
        LD(0x10) = first;
        LD(0x14) = second;
        pointer = (u32)cm_fn_00549ef0(work + 0x20, day, year);
        first_year = LW(0x12);
        second_year = W(pointer + 2u);
        id = 0;
        if (first_year == second_year && (u16)first == W(pointer))
            id = 1;
        *(volatile u8 *)&frame.state = 0;
        if (id != 0)
            goto done;
    }
    club = D(object + 0x1cu);
    if (club != 0) {
        competition = D(club + 0x57u);
        if (competition != 0) {
            id = D(competition);
            target = D(0x009cf5dcu);
            if (id != target) {
                nation = D(club + 0x53u);
                target = D(0x009cf4dcu);
                id = D(nation);
            }
            if (id == target) {
                year = W(object + 0x28u);
                day = W(object + 0x2au);
                LB(7) = 15;
                pointer = (u32)cm_fn_00549ef0(work + 0x20, day, year);
                cm_fn_00417c10(work + 8, work + 0xc, pointer);
            }
        }
    }
    club = D(object + 0x1cu);
    if (cm_fn_00542740(club) == 0)
        goto done;
    club = D(object + 0x20u);
    if (cm_fn_00542740(club) == 0)
        goto done;
    available = 0;
    for (phase = 0; phase < 2u; ++phase) {
        if (phase != 0) {
            LB(6) = 0;
            LB(3) = 0;
            LB(5) = 0;
            LB(4) = 0;
            available = 0;
        }
        for (index = 0; index < 50u; ++index) {
            club = D(object + (phase == 0 ? 0x1cu : 0x20u));
            player = D(club + 0xd7u + index * 4u);
            if (player == 0) {
                LB(4) = (u8)(LB(4) + 1u);
                continue;
            }
            if (D(player + 0x39u) != club)
                continue;
            receiver = D(0x00b63d28u);
            if (cm_fn_0062c400(receiver, player) != 0)
                continue;
            id = D(player);
            target = D(0x00ae2364u);
            if ((s32)id < (s32)target)
                LB(3) = (u8)(LB(3) + 1u);
            if (LB(7) != 0xffu) {
                id = cm_fn_0053ab90(player, (u32)(work + 8));
                if ((s8)id <= 21)
                    LB(6) = (u8)(LB(6) + 1u);
            }
            if ((B(player + 0x5eu) & 1u) == 0) {
                target = D(0x00ae2364u);
                id = D(player);
                if ((s32)id < (s32)target)
                    LB(5) = (u8)(LB(5) + 1u);
                available = (u8)(available + 1u);
            }
        }
        if ((s8)available < 11) {
            if ((s8)LB(3) > 36)
                goto done;
            if ((s8)LB(4) < 14 - (s32)(s8)available)
                goto done;
        }
        club = D(object + (phase == 0 ? 0x1cu : 0x20u));
        id = cm_fn_00600a50(club, 1, 1, 0, 0);
        if (id != 0 && ((s8)available < 11 || (s8)LB(5) < 1))
            goto done;
        required = LB(7);
        young = LB(6);
        if ((s8)young < (s8)required)
            goto done;
    }
    result = 1;
done:
    predecessor = frame.prev;
    CM_FS(volatile u32, 0) = predecessor;
    return result;
}
