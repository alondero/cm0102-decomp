#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
/* 4afbd0 observes the low WORD of both counts. Its EAX result is not used:
   the root overwrites AX/AL before pushing an index whose callee reads BYTE. */
extern u32 CM_CDECL cm_fn_004afbd0(u32, u16, u16);
extern u32 CM_THISCALL cm_fn_00687430(u32, u8, u32, u8, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Preferred-image VA008533d0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/fullbool. Scratch39c: directionL0,countL4,scanL8,
   diagnosticsLc/L10c,100-pointer candidate arrayL20c. No scratch zero-fill. */
u32 CM_THISCALL cm_fn_008533d0(u32 self)
{
    u32 scratch[0x39c / 4];
    u32 l = (u32)scratch, eax, nation_count, club_count, index, club;
    u32 pointer, division, nation, id, value, pass, pool_count, pool_entry;
    u16 limit, keep, count_word;
    u8 selected = 0;

    W(self + 0x3e) = 22;
    D(l + 4) = 0;
    eax = cm_fn_00944e46(0x512);
    D(self + 0xb1) = eax;
    if (eax == 0) {
        cm_fn_00944cff(0x00a82880, 0, 0, l + 0xc, l + 0x10c);
        value = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, value, l + 0xc, l + 0x10c, 0x2ac);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    nation_count = D(0x00ae2350);
    club_count = D(0x00ae235c);
    if ((s32)(club_count - nation_count * 2U) > 0) {
        pointer = D(0x00ae23b4) + 0x57U;
        index = 0;
        do {
            division = D(pointer);
            if (division != 0) {
                id = D(division);
                value = D(0x009cf6b0);
                if (id == value) {
                    id = D(0x009cf764);
                    value = D(0x00ae23d0);
                    D(pointer) = value + id * 107U;
                    nation_count = D(0x00ae2350);
                }
            }
            club_count = D(0x00ae235c);
            ++index;
            pointer += 581U;
        } while ((s32)(s16)(u16)index < (s32)(club_count - nation_count * 2U));
    }

    for (pass = 0; pass != 2; ++pass) {
        club_count = D(0x00ae235c);
        club = D(0x00ae23b4);
        index = 0;
        if ((s32)(club_count - nation_count * 2U) > 0) {
            do {
                division = D(club + 0x57);
                if (division == 0) goto next_club;
                nation = D(club + 0x53);
                if (nation == 0) goto next_club;
                value = D(0x009cf488);
                if (D(nation) != value) goto next_club;
                id = D(division);
                value = D(0x009cf624);
                if (id == value) goto next_club;
                if (id == D(0x009cf628)) goto next_club;
                eax = cm_fn_00540a50(club, l, 1);
                if (eax == 0) goto clear_club;
                value = D(l);
                if (value != 0) goto clear_club;
                division = D(eax + 0x57);
                if (division == 0) goto clear_club;
                limit = W(self + 0x3e);
                if ((s16)(s8)selected >= (s16)limit) goto clear_club;
                id = D(division);
                value = D(pass == 0 ? 0x009cf624 : 0x009cf628);
                if (id != value) goto clear_club;
                {
                    u8 slot = selected;
                    ++selected;
                    cm_fn_00687430(self, slot, club, 0, 0);
                }
                value = D(self + 4);
                D(club + 0x60) = value;
                goto next_club;
clear_club:
                D(club + 0x60) = 0;
next_club:
                nation_count = D(0x00ae2350);
                club_count = D(0x00ae235c);
                ++index;
                club += 581U;
            } while ((s32)(s16)(u16)index < (s32)(club_count - nation_count * 2U));
            /* Only the first of these two original loops reloads this here. */
            if (pass == 0)
                nation_count = D(0x00ae2350);
        }
    }

    club_count = D(0x00ae235c);
    club = D(0x00ae23b4);
    D(l + 8) = 0;
    if ((s32)club_count > 0) {
        do {
            pool_count = D(l + 4);
            if ((s16)pool_count >= 100) break;
            division = D(club + 0x57);
            if (division != 0) {
                nation = D(club + 0x53);
                if (nation != 0) {
                    id = D(division);
                    value = D(0x009cf62c);
                    if (id == value) goto candidate;
                    if (id == D(0x009cf630)) goto candidate;
                    if (id == D(0x009cf634)) goto candidate;
                    if (id == D(0x009cf638)) goto candidate;
                    value = D(0x009cf488);
                    if (D(nation) != value) goto next_candidate;
                    if (id != D(0x009cf764)) goto next_candidate;
candidate:
                    eax = cm_fn_00540a50(club, l, 1);
                    if (eax == 0) {
                        index = (u32)(s32)(s16)(u16)pool_count;
                        ++pool_count;
                        D(l + 0x20cU + index * 4U) = club;
                        D(l + 4) = pool_count;
                    }
                }
            }
next_candidate:
            index = D(l + 8);
            club += 581U;
            ++index;
            D(l + 8) = index;
            club_count = D(0x00ae235c);
        } while ((s32)(s16)(u16)index < (s32)club_count);
    } else {
        pool_count = D(l + 4);
    }
    limit = W(self + 0x3e);
    keep = (u16)(limit - (u16)(s16)(s8)selected);
    cm_fn_004afbd0(l + 0x20c, (u16)pool_count, keep);
    index = 0;
    if ((s16)pool_count > 0) {
        do {
            limit = W(self + 0x3e);
            if ((s16)(s8)selected >= (s16)limit) break;
            pool_entry = l + 0x20cU + (u32)(s32)(s16)(u16)index * 4U;
            club = D(pool_entry);
            {
                u8 slot = selected;
                ++selected;
                cm_fn_00687430(self, slot, club, 0, 0);
            }
            club = D(pool_entry);
            value = D(self + 4);
            ++index;
            count_word = W(l + 4);
            D(club + 0x60) = value;
        } while ((s16)(u16)index < (s16)count_word);
    }
    limit = W(self + 0x3e);
    return (s16)(s8)selected == (s16)limit;
}
