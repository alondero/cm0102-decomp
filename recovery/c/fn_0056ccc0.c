#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945333(u32, u32, u32, u32);

/* Preferred-image VA0056ccc0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX input/RET0/full EAX. Five scans retain each base/count reload,
   signed low-WORD index comparison and packed six-byte output access.
   Scratch200: diagnostic filenameL100/extL0. Sort/comparator stay original. */
u32 CM_FASTCALL cm_fn_0056ccc0(u32 self)
{
    u32 scratch[0x200 / 4];
    u32 l = (u32)scratch, count = 0, p, version, phase;
    u32 clubs, club, index, offset, nation_pointer, nation, wanted;
    u32 capacity, output, prior, prior_id, special, value, entry;
    u8 byte_value;
    W(self + 0x3e) = 0x5c;
    B(self + 0xb2) = 0;
    p = cm_fn_00944e46(0x228);
    D(self + 0xa7) = p;
    if (p == 0) {
        cm_fn_00944cff(0x009c4224, 0, 0, l + 0x100, l);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x100, l, 0x18f);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0xffffffffu;
    }
    for (phase = 0; phase < 5; ++phase) {
        clubs = D(0x00ae235c);
        club = D(0x00ae23b4);
        index = 0;
        if ((s32)clubs <= 0) continue;
        offset = count * 6u;
        do {
            nation_pointer = D(club + 0x57);
            if (nation_pointer == 0) goto advance;
            capacity = (u32)(s32)(s16)W(self + 0x3e);
            if (count == capacity) goto advance;
            nation = D(nation_pointer);
            if (phase == 0) {
                wanted = D(0x009cf5c8);
                if (nation != wanted) goto advance;
            } else if (phase == 1) {
                wanted = D(0x009cf5c4);
                if (nation != wanted) goto advance;
            } else if (phase == 2) {
                wanted = D(0x009cf5c0);
                if (nation != wanted) goto advance;
            } else {
                wanted = D(0x009cf5bc);
                if (nation != wanted) {
                    if (phase == 3) goto advance;
                    wanted = D(0x009cf5c0);
                    if (nation != wanted) {
                        wanted = D(0x009cf5c4);
                        if (nation != wanted) {
                            wanted = D(0x009cf5c8);
                            if (nation != wanted) goto advance;
                        }
                    }
                }
            }
            special = D(club + 0x1db);
            if (phase != 4) {
                if (special != 0xffffffffu) goto advance;
            } else if (special == 0xffffffffu) goto advance;

            if (phase < 2) {
                output = D(self + 0xa7);
                ++count;
                D(output + offset) = club;
                output = D(self + 0xa7);
                B(output + offset + 4) = 0;
                output = D(self + 0xa7);
                B(output + offset + 5) = 0;
                offset += 6;
                capacity = (u32)(s32)(s16)W(self + 0x3e);
            } else if (phase < 4) {
                output = D(self + 0xa7);
                D(output + offset) = club;
                output = D(self + 0xa7);
                B(output + offset + 5) = 0;
                prior = D(club + 0x5b);
                if (prior == 0) goto no_prior;
                prior_id = D(prior);
                wanted = D(0x009cf5bc);
                if (prior_id == wanted) {
                    output = D(self + 0xa7);
                    value = phase == 2 ? 0x46 : 0x78;
                    byte_value = B(club + 0x5f);
                    B(output + offset + 4) = (u8)(value - byte_value);
                } else {
                    /* Both original scans reread the pointed-to DWORD. */
                    if (prior == 0) goto no_prior;
                    prior_id = D(prior);
                    wanted = D(0x009cf5c0);
                    if (prior_id != wanted) goto no_prior;
                    output = D(self + 0xa7);
                    value = phase == 2 ? 0x28 : 0x64;
                    byte_value = B(club + 0x5f);
                    B(output + offset + 4) = (u8)(value - byte_value);
                }
                goto prior_done;
no_prior:
                output = D(self + 0xa7);
                B(output + offset + 4) = 0;
prior_done:
                capacity = (u32)(s32)(s16)W(self + 0x3e);
                ++count;
                offset += 6;
            } else {
                byte_value = B(self + 0xb2);
                offset += 6;
                ++count;
                B(self + 0xb2) = (u8)(byte_value + 1);
                output = D(self + 0xa7);
                D(output + offset - 6) = club;
                output = D(self + 0xa7);
                B(output + offset - 2) = 0x78;
                output = D(self + 0xa7);
                B(output + offset - 1) = 0;
                capacity = (u32)(s32)(s16)W(self + 0x3e);
            }
            if (count == capacity) break;
advance:
            clubs = D(0x00ae235c);
            ++index;
            club += 0x245;
        } while ((s32)(s16)index < (s32)clubs);
    }
    output = D(self + 0xa7);
    cm_fn_00945333(output, count, 6, 0x005221c0);
    index = 0;
    if ((s32)count > 0) {
        do {
            output = D(self + 0xa7);
            entry = output + (u32)(s32)(s16)index * 6u;
            club = D(entry);
            if (club == 0) goto category_three;
            nation_pointer = D(club + 0x57);
            if (nation_pointer != 0) {
                nation = D(nation_pointer);
                wanted = D(0x009cf5bc);
                if (nation == wanted) {
                    B(entry + 4) = 1;
                    goto category_done;
                }
            }
            if (club == 0) goto category_three;
            nation_pointer = D(club + 0x57);
            if (nation_pointer == 0) goto category_three;
            nation = D(nation_pointer);
            wanted = D(0x009cf5c0);
            if (nation != wanted) goto category_three;
            B(entry + 4) = 2;
            goto category_done;
category_three:
            B(entry + 4) = 3;
category_done:
            ++index;
        } while ((s32)(s16)index < (s32)count);
    }
    capacity = (u32)(s32)(s16)W(self + 0x3e);
    if (count != capacity) return 0;
    byte_value = B(self + 0xb2);
    if ((s8)byte_value > 12) B(self + 0xb2) = 12;
    return 1;
}
