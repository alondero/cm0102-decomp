#include "cmre.h"

/* Preferred-image VA 0x007ad590, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The arithmetic's incoming ECX high WORD is not consumed: after three
   full subtractions and ADD 5000, MOVSX EAX,CX supplies the RNG argument.
   Scores and loop counters retain their original signed WORD comparisons. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_008ce990(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053c590(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_007ad590(u32 self, u32 player)
{
    u8 scratch[0x218];
    u32 local = (u32)scratch, nation, country, key, table, list, node;
    u32 club_id, base, club, filter, value, manager, profile, target_word;
    u32 score, magnitude, saved_self, count, text;
    s32 target, difference;
    nation = D(player + 0x1a);
    D(local + 0x10) = self;
    D(local + 0x0c) = 0xffff9e58u;
    country = D(nation);
    key = D(0x009cf510);
    D(local + 8) = 0;
    D(local) = country;
    if (country == key) goto redirect;
    if (country == D(0x009cf2dc)) goto redirect;
    if (country == D(0x009cf3e4)) {
        value = cm_fn_0090cfc0(5);
        if ((s32)value < 2) goto redirect;
    }
    goto lookup;
redirect:
    country = D(0x009cf2e4);
    D(local) = country;
lookup:
    table = D(self);
    list = D(table + country * 4u);
    if (list == 0) return 0;
    if (W(list) == 0) return 0;
    value = D(list + 2);
    D(local + 4) = 0;
    D(list + 10) = value;
    table = D(self);
    list = D(table + country * 4u);
    if ((s16)W(list) <= 0) return D(local + 8);
    do {
        node = cm_fn_0068d1b0(list);
        if (node == 0) goto error;
        club_id = D(node);
        base = D(0x00ae23b4);
        club = base + club_id * 581u;
        filter = D(player + 0x1e);
        if (filter != 0) {
            value = D(filter);
            key = D(0x009cf424);
            if (value == key) {
                value = D(club);
                key = D(0x009d0120);
                if (value != key && value != D(0x009d0134) &&
                    value != D(0x009d0128) && value != D(0x009d01a0) &&
                    value != D(0x009d0124)) goto next;
            }
        }
        manager = D(0x00de1f54);
        value = cm_fn_008ce990(manager, player, club, local + 0x16, 0);
        if (value == 0) goto next;
        value = cm_fn_0053c590(club, 0, 0);
        if ((s8)value >= 50) goto next;
        profile = D(player + 0x61);
        target_word = W(club + 0x80);
        target = (s16)target_word;
        difference = (s16)W(profile + 0x0b) - target;
        magnitude = difference < 0 ? 0u - (u32)difference : (u32)difference;
        score = target_word - magnitude;
        difference = (s16)W(profile + 7) * 50 - target;
        magnitude = difference < 0 ? 0u - (u32)difference : (u32)difference;
        score -= magnitude;
        difference = (s16)W(profile + 5) * 50 - target;
        magnitude = difference < 0 ? 0u - (u32)difference : (u32)difference;
        score -= magnitude;
        score += 5000;
        value = cm_fn_0090cfc0((u32)(s32)(s16)(u16)score);
        if ((s16)value > (s16)W(local + 0x0c)) {
            D(local + 8) = club;
            D(local + 0x0c) = value;
        }
        country = D(local);
next:
        saved_self = D(local + 0x10);
        count = D(local + 4);
        ++count;
        table = D(saved_self);
        D(local + 4) = count;
        list = D(table + country * 4u);
    } while ((s16)(u16)count < (s16)W(list));
    return D(local + 8);
error:
    cm_fn_00944cff(0x00a6bf9c, 0, 0, local + 0x118, local + 0x18);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x118, local + 0x18, 0x19e9);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
