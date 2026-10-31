#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_005409b0(u32);
extern u32 CM_THISCALL cm_fn_0058cf70(u32, u32);
extern u32 CM_THISCALL cm_fn_0086cc50(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008ce920(u32, u32, u16);
extern u32 CM_THISCALL cm_fn_004a7f20(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_007706c0(u32, u32, u32, u32, u32, u32);

/* Preferred-image VA0076b600/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/full EAX, including the final helper high24 bits with AL=2.
   Scratch60: sideBYTE L2/indexBYTE L3/savedselfL4/teamL8/countBYTE Lc,
   twenty initialized-prefix pointersL10. Full1568B7706c0 reads count only
   as a BYTE; its incoming ECX is dead. Raw unwritten count upper24 bits
   can therefore be omitted at that original boundary. No scratch zero-fill. */
u32 CM_THISCALL cm_fn_0076b600(u32 self, volatile u32 match_arg,
                              volatile u32 statistics_arg)
{
    u32 scratch[0x60 / 4];
    u32 l = (u32)scratch, match, statistics, pointer, tag, type, mode;
    u32 owner, team, nation, id, table, slot, value, player, entry;
    u32 old, current, opponent, context, result, group, rank, date_lo, date_hi;
    u32 leader, roster, base, phase, i;
    u8 count, a, b, c, unset;
    u16 year_count;
    match = match_arg;
    D(l + 4) = self;
    pointer = D(match + 0x1c);
    result = cm_fn_0053b900(pointer);
    if (result == 0) return result;
    pointer = D(match + 0x20);
    result = cm_fn_0053b900(pointer);
    if (result == 0) return result;
    pointer = D(match + 0x14);
    tag = D(0x009cf794);
    type = D(pointer);
    statistics = statistics_arg;
    if (type == tag) mode = 0;
    else {
        tag = D(0x009cf798);
        if (type == tag) mode = 1;
        else {
            tag = D(0x009cf7a0);
            mode = type == tag ? 2 : 3;
        }
    }
    if (mode == 3) {
        context = D(self + 0x1c);
        cm_fn_0058cf70(context, match);
    }
    if (mode != 2) {
        if (mode != 0) B(l + 2) = 0;
        phase = 0;
        do {
            if (mode != 0 && phase != 0) phase = B(l + 2);
            if (mode == 0 && phase == 1) owner = D(l + 4);
            else owner = self;
            if (mode == 3 && phase == 1) match = match_arg;
            team = D(match + (phase == 0 ? 0x1c : 0x20));
            nation = D(team + 0x53);
            id = D(nation);
            if (mode != 0) owner = D(l + 4);
            table = D(owner + 4);
            slot = table + id * 0x6eu + 2;
            year_count = W(slot);
            W(slot) = (u16)(year_count + 1);
            group = (u32)(s32)(s8)phase * 0x625u;
            entry = statistics + group + 0x190;
            if (mode == 3) B(l + 3) = 0;
            i = 0;
            do {
                id = D(entry);
                if (id == 0xffffffffu) goto next_player;
                if ((s8)i >= 11 && B(entry + 0x32) == 0xff) goto next_player;
                base = D(0x00ae23bc);
                player = base + id * 0x6eu;
                if (player != 0) {
                    id = D(player);
                    value = D(0x00ae2364);
                    if ((s32)id >= (s32)value) goto next_player;
                }
                if (mode == 0) {
                    id = D(player);
                    base = D(0x00ae2d0c);
                    B(base + id * 0x4fu + 0xd) = 1;
                } else if (mode == 1) {
                    id = D(player);
                    base = D(0x00ae2d0c);
                    a = B(base + id * 0x4fu + 0xc);
                    B(base + id * 0x4fu + 0xc) = a | 2;
                    current = D(team + 0x53);
                    old = D(player + 0x1e);
                    if (current == old) {
                        value = D(player + 0x1a);
                        D(player + 0x1e) = value;
                        current = D(team + 0x53);
                        D(player + 0x1a) = current;
                    }
                    match = match_arg;
                } else {
                    a = B(player + 0x22);
                    if (a == 0) {
                        current = D(team + 0x53);
                        old = D(player + 0x1e);
                        if (current == old) {
                            value = D(player + 0x1a);
                            D(player + 0x1e) = value;
                            current = D(team + 0x53);
                            D(player + 0x1a) = current;
                        }
                        pointer = match_arg;
                        opponent = D(pointer + 0x1c);
                        if (team == opponent) opponent = D(pointer + 0x20);
                        context = D(0x00dd7f00);
                        cm_fn_0086cc50(context, player, opponent);
                    }
                    a = B(player + 0x22);
                    b = B(player + 0x23);
                    B(player + 0x22) = (u8)(a + 1);
                    c = B(entry + 0x21);
                    B(player + 0x23) = (u8)(b + c);
                    nation = D(team + 0x53);
                    id = D(nation);
                    owner = D(l + 4);
                    table = D(owner + 4);
                    year_count = W(table + id * 0x6eu + 2);
                    context = D(0x00de1f54);
                    cm_fn_008ce920(context, player, year_count);
                    i = B(l + 3);
                }
next_player:
                i = (u8)(i + 1);
                entry += 0x49;
                if (mode == 3) B(l + 3) = (u8)i;
            } while ((s8)i < 20);
            if (mode == 0) phase = (u8)(phase + 1);
            else {
                a = B(l + 2);
                ++a;
                B(l + 2) = a;
                phase = a;
            }
        } while ((s8)phase < 2);
        if (mode == 3) match = match_arg;
    }
    B(l + 2) = 0;
    do {
        phase = B(l + 2);
        team = D(match + (phase == 0 ? 0x1c : 0x20));
        D(l + 8) = team;
        unset = B(match + 0x40) == 0;
        result = cm_fn_005409b0(team);
        rank = unset ? 1u : result == 0 ? 0u : 2u;
        date_hi = D(0x00ae2c94);
        nation = D(team + 0x53);
        owner = D(l + 4);
        date_lo = D(0x00ae2c90);
        id = D(nation);
        table = D(owner + 4);
        result = cm_fn_004a7f20(table + id * 0x6eu + rank * 20u + 0x3a,
                                 date_lo, date_hi);
        if (result != 0) {
            nation = D(team + 0x53);
            id = D(nation);
            table = D(owner + 4);
            B(table + id * 0x6eu + rank * 20u + 0x44) = 0;
        }
        phase = B(l + 2);
        base = D(0x00ae23bc);
        B(l + 0xc) = 0;
        group = (u32)(s32)(s8)phase * 0x625u;
        id = (u32)(s32)(s8)B(statistics + group + 0x153);
        id = D(statistics + group + id * 0x49u + 0x190);
        leader = base + id * 0x6eu;
        entry = statistics + group + 0x190;
        count = 0;
        i = 0;
        do {
            id = D(entry);
            if (id == 0xffffffffu) goto next_candidate;
            if ((s8)i >= 11 && B(entry + 0x32) == 0xff) goto next_candidate;
            player = base + id * 0x6eu;
            if (player != 0) {
                value = D(0x00ae2364);
                if ((s32)D(player) >= (s32)value) goto next_candidate;
            }
            value = count;
            ++count;
            D(l + 0x10 + value * 4u) = player;
next_candidate:
            i = (u8)(i + 1);
            entry += 0x49;
        } while ((s8)i < 20);
        match = match_arg;
        B(l + 0xc) = count;
        team = D(l + 8);
        owner = D(l + 4);
        (void)owner; /* The original loads ECX here; 7706c0 overwrites it before use. */
        roster = l + 0x10;
        result = cm_fn_007706c0(team, match, statistics, leader, roster, count);
        a = B(l + 2);
        ++a;
        B(l + 2) = a;
        result = (result & 0xffffff00u) | a;
        if ((s8)a >= 2) break;
        match = match_arg;
    } while (1);
    return result;
}
