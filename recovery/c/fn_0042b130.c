#include "cmre.h"

/* Preferred-image VA 0x0042b130, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   L is ESP after SUB ESP,270h. Preserve the two lists, signed byte quotas,
   original calls, and unchecked index twelve after an unsuccessful search. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern void CM_CDECL cm_fn_004afbd0(u32, s16, s16);
extern u32 CM_THISCALL cm_fn_006826d0(u32, u32);
extern u8 CM_CDECL cm_fn_00544820(u32 *);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern void CM_THISCALL cm_fn_0069ba50(u32, u32, u32);
extern void CM_THISCALL cm_fn_0069b790(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cd030(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_CDECL cm_fn_0042b130(void)
{
    static const u32 competition_ids[12] = {
        0x009cf9e4, 0x009cf9dc, 0x009cf9d4, 0x009cf9d0,
        0x009cf998, 0x009cf9bc, 0x009cf9c0, 0x009cf9b0,
        0x009cf9b4, 0x009cf994, 0x009cf9a4, 0x009cf9c8
    };
    static const u32 state_ids[12] = {
        0x009cf99c, 0x009cf97c, 0x009cf988, 0x009cf98c,
        0x009cf96c, 0x009cf974, 0x009cf968, 0x009cf970,
        0x009cf984, 0x009cf978, 0x009cf980, 0x009cf990
    };
    u8 scratch[0x270];
    u32 work = (u32)scratch, base, index, value, i, allocation;
    u32 selected = 0, club_index = 0, total, club, state, match, cursor, remaining;
    u32 competition, manager, p, league, start, capacity, receiver;
    s8 quota;
    char *version;

    base = D(0x00adadfc);
    index = D(competition_ids[0]);
    for (i = 0; i < 12; ++i) {
        value = D(base + index * 4u);
        index = i < 11 ? D(competition_ids[i + 1]) : D(state_ids[0]);
        D(work + 4u + i * 4u) = value;
    }
    value = index * 107u;
    base = D(0x00ae23d0);
    D(work + 0x34) = base + value;
    index = D(state_ids[1]);
    for (i = 1; i < 12; ++i) {
        value = base + index * 107u;
        if (i < 11) index = D(state_ids[i + 1]);
        D(work + 0x34u + i * 4u) = value;
    }
    for (i = 0; i < 12; ++i) {
        p = D(work + 4u + i * 4u);
        value = B(p + 0xc1);
        B(work + 0x64u + i) = (u8)value;
    }
    allocation = (u32)cm_fn_00944e46(800);
    D(work) = allocation;
    if (!allocation) {
        cm_fn_00944cff(0x0098aa30, 0, 0, scratch + 0x170, scratch + 0x70);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      scratch + 0x170, scratch + 0x70, 0x3d0);
        value = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return value;
    }
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    if ((s32)total > 0) {
        do {
            if ((s16)selected >= 200) break;
            state = D(club + 0x60);
            if (state) {
                for (match = 0; match < 12; ++match) {
                    if (D(work + 0x34u + match * 4u) == state) {
                        index = (u32)(s32)(s16)selected;
                        ++selected;
                        D(allocation + index * 4u) = club;
                        break;
                    }
                }
            }
            total = D(0x00ae235c);
            ++club_index;
            club += 581;
        } while ((s32)club_index < (s32)total);
    }
    cm_fn_004afbd0(allocation, (s16)selected, 25);
    remaining = (u32)(s32)(s16)selected;
    if ((s32)remaining > 0) {
        cursor = allocation;
        do {
            club = D(cursor);
            state = D(club + 0x60);
            for (match = 0; match < 12; ++match) {
                if (state == D(work + 0x34u + match * 4u)) break;
            }
            quota = (s8)B(work + 0x64u + match);
            if (quota > 0) {
                competition = D(work + 4u + match * 4u);
                value = D(competition + 4);
                D(club + 0x60) = value;
                club = D(cursor);
                if (D(club + 0x57) && !D(club + 0xcf)) {
                    manager = D(0x00b67a44);
                    cm_fn_0069ba50(manager, club, 0);
                }
                B(work + 0x64u + match) = (u8)((u8)quota - 1u);
            }
            cursor += 4;
            --remaining;
        } while (remaining);
        allocation = D(work);
    }
    for (i = 0; i < 12; ++i) {
        competition = D(work + 4u + i * 4u);
        capacity = (u32)(s32)(s16)W(competition + 0x3e);
        value = (u32)(s32)(s8)B(competition + 0xc1);
        start = capacity - value;
        if ((s32)start < (s32)capacity) {
            state = D(work + 0x34u + i * 4u);
            do {
                p = cm_fn_006826d0(competition, start);
                club = D(p);
                D(club + 0x60) = state;
                p = cm_fn_006826d0(competition, start);
                club = D(p);
                if (!cm_fn_00544820((u32 *)club)) {
                    p = cm_fn_006826d0(competition, start);
                    club = D(p);
                    if (cm_fn_00600a50(club, 1, 1, 0, 0)) {
                        p = cm_fn_006826d0(competition, start);
                        club = D(p);
                        manager = D(0x00b67a44);
                        cm_fn_0069b790(manager, club, 1, 7, 0);
                    }
                }
                league = D(competition + 4);
                p = cm_fn_006826d0(competition, start);
                club = D(p);
                receiver = D(0x00ae1050);
                cm_fn_004cd030(receiver, club, league);
                capacity = (u32)(s32)(s16)W(competition + 0x3e);
                ++start;
            } while ((s32)start < (s32)capacity);
            allocation = D(work);
        }
    }
    return (u32)cm_fn_009452ca(allocation);
}
