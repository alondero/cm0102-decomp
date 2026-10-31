#include "cmre.h"
/* Preferred-image VA 0x007c0d70, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_004b0700(u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32, u32);
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *, u32);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
/* The listing pushes a DWORD whose upper three bytes are uninitialized.
   Both original callees read its low byte; retain the raw pushed DWORD. */
extern u32 CM_THISCALL cm_fn_007c3920(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007c5d80(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define B(a) (*(volatile u8 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define D(a) (*(volatile u32 *)(u32)(a))
#define Q(a) (*(volatile double *)(u32)(a))

static void report_error(u8 *first, u8 *second, u32 line)
{
    char *version;
    cm_fn_00944cff(0x00a78310, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
}

u32 CM_THISCALL cm_fn_007c0d70(u32 self, u32 match, u32 packet)
{
    u8 scratch[0x42c];
    u32 local = (u32)scratch;
    u32 side, selected, receiver, argument, value, id, base, pointer;
    u32 side_offset, opposite_score, entry, row, club_id, player_record, club_record;
    u32 pass, pushed_mode, record, history, delta, field;
    u8 mode, category, cl, dl, al;
    long double floating;
    if (match == 0) {
        report_error(scratch + 0x12c, scratch + 0x2c, 0x28c);
        return 0;
    }
    if (packet == 0) {
        report_error(scratch + 0x2c, scratch + 0x12c, 0x294);
        return 0;
    }
    if (D(match) == 0xfffffffeu)
        mode = (u8)(3u - (cm_fn_0090cfc0(9) != 0));
    else
        mode = B(local + 0x1fu);
    for (side = 0; side < 2; ++side) {
        if (D(match) != 0xfffffffeu) {
            selected = D(match + (side == 0 ? 0x1cu : 0x20u));
            receiver = D(0x00ae2c74);
            argument = D(match + 0x14u);
            value = (u32)cm_fn_005c6710((void *)receiver, argument);
            if (value != 0) {
                pointer = D(match + 0x1cu);
                value = cm_fn_0053b900((u32 *)pointer);
                mode = value != 0 ? 5 : 1;
            } else {
                pointer = D(match + 0x14u);
                id = D(pointer);
                if (id == D(0x009cf684) || id == D(0x009cf798) || id == D(0x009cf794)) {
                    mode = 1;
                } else if (selected == D(match + 0x1cu) && B(match + 0x40u) == 0) {
                    mode = 1;
                } else if (selected == D(match + 0x20u) && B(match + 0x41u) == 0) {
                    mode = 1;
                } else {
                    id = D(match);
                    base = D(0x00adadfc);
                    pointer = D(base + id * 4u);
                    if (pointer == 0) {
                        report_error(scratch + 0x2c, scratch + 0x12c, 0x103a);
                        mode = 2;
                    } else {
                        category = B(pointer + 0x42u);
                        if (category == 4) mode = 5;
                        else if (category == 2) mode = 4;
                        else if (category == 1)
                            mode = (u8)(3u - (cm_fn_004b0700(id) != 0));
                        else {
                            report_error(scratch + 0x32c, scratch + 0x22c, 0x105d);
                            mode = 2;
                        }
                    }
                }
            }
        }
        side_offset = side * 0x625u;
        opposite_score = packet - side_offset + 0x744u;
        entry = packet + side_offset + 0x190u;
        for (row = 0; row < 20; ++row, entry += 0x49u) {
            if (row > 10 && B(entry + 0x32u) == 0xffu) continue;
            id = D(entry);
            if (id == 0xffffffffu) continue;
            club_id = D(entry + 4u);
            if (club_id == 0xffffffffu) continue;
            base = D(0x00ae23bc);
            player_record = base + id * 110u;
            if (player_record != 0) {
                id = D(player_record);
                value = D(0x00ae2364);
                if ((s32)id >= (s32)value) continue;
            }
            base = D(0x00ae23c4);
            club_record = base + club_id * 70u;
            al = B(packet + 0x4cu);
            field = al != 0 ? 0x20u : 0x10u;
            value = B(entry + 0x35u);
            floating = (long double)value + (long double)Q(self + field);
            Q(self + field) = (double)floating;
            floating = (long double)Q(self + field + 8u);
            floating += (long double)Q(0x00967890);
            Q(self + field + 8u) = (double)floating;

            for (pass = 1; pass <= 2; ++pass) {
                B(local + 0x20u) = pass == 1 ? mode : 6;
                if (pass == 2 && (mode == 5 || mode == 1)) continue;
                pushed_mode = D(local + 0x20u);
                record = cm_fn_007c3920(self, club_record, pushed_mode);
                if (record == 0)
                    record = cm_fn_007c5d80(self, club_record, pushed_mode);
                dl = B(record);
                ++dl;
                B(record) = dl;
                if (row > 10) ++B(record + 1u);
                cl = B(entry + 0x21u); dl = B(record + 2u); dl = (u8)(dl + cl);
                cl = B(record + 3u); B(record + 2u) = dl;
                dl = B(entry + 0x22u); cl = (u8)(cl + dl);
                dl = B(record + 4u); B(record + 3u) = cl;
                cl = B(entry + 0x23u); dl = (u8)(dl + cl);
                cl = B(record + 5u); B(record + 4u) = dl;
                dl = B(entry + 0x24u); cl = (u8)(cl + dl);
                dl = B(record + 6u); B(record + 5u) = cl;
                cl = B(entry + 0x27u); dl = (u8)(dl + cl); B(record + 6u) = dl;
                cl = B(side_offset + packet + 0x11fu);
                dl = B(record + 7u); dl = (u8)(dl + cl); B(record + 7u) = dl;
                dl = B(opposite_score); B(record + 8u) = (u8)(B(record + 8u) + dl);
                dl = B(side_offset + packet + 0x11fu);
                if (dl > B(opposite_score)) ++B(record + 9u);
                dl = B(side_offset + packet + 0x11fu);
                if (dl < B(opposite_score)) ++B(record + 0xau);
                cl = B(entry + 0x2cu); dl = B(record + 0xbu); dl = (u8)(dl + cl);
                cl = B(record + 0xcu); B(record + 0xbu) = dl;
                dl = B(entry + 0x2du); cl = (u8)(cl + dl); B(record + 0xcu) = cl;
                id = D(entry + 4u);
                if (id == D(packet + 0x3cu)) ++B(record + 0xdu);
                value = B(entry + 0x35u); W(record + 0xeu) = (u16)(W(record + 0xeu) + value);
                value = B(entry + 0x3eu); W(record + 0x10u) = (u16)(W(record + 0x10u) + value);
                value = B(entry + 0x3fu); W(record + 0x12u) = (u16)(W(record + 0x12u) + value);
                value = B(entry + 0x42u); W(record + 0x14u) = (u16)(W(record + 0x14u) + value);
                value = B(entry + 0x28u); W(record + 0x16u) = (u16)(W(record + 0x16u) + value);
                value = B(entry + 0x25u); W(record + 0x18u) = (u16)(W(record + 0x18u) + value);
                value = B(entry + 0x26u); W(record + 0x1au) = (u16)(W(record + 0x1au) + value);
                if (row < 11) {
                    cl = B(entry + 0x34u);
                    if ((s8)cl > -1) delta = (u32)(s32)(s8)cl;
                    else {
                        cl = B(entry + 0x30u);
                        if ((s8)cl > -1) delta = (u32)(s32)(s8)cl;
                        else delta = (u32)(s32)(s16)W(packet + 0x4du);
                    }
                    D(record + 0x1cu) = D(record + 0x1cu) + delta;
                } else {
                    cl = B(entry + 0x32u);
                    if (cl != 0xffu) {
                        dl = B(entry + 0x34u);
                        if ((s8)dl > -1) delta = (u32)(s32)(s8)dl;
                        else {
                            dl = B(entry + 0x30u);
                            delta = (u32)(s32)(s8)dl;
                            if ((s8)dl <= -1) delta = (u32)(s32)(s16)W(packet + 0x4du);
                        }
                        delta -= (u32)(s32)(s8)cl;
                        D(record + 0x1cu) = D(record + 0x1cu) + delta;
                    }
                }
            }
            id = D(entry + 4u);
            base = D(self + 0x30u);
            history = base + id * 39u;
            if (mode == 2) goto update_history;
            id = D(match);
            if ((s32)id <= -1 || cm_fn_004b0700(id) == 0) goto rotate;
update_history:
            ++W(history + 0x20u);
            value = B(entry + 0x21u);
            W(history + 0x22u) = (u16)(W(history + 0x22u) + value);
            receiver = D(0x00ae1050);
            if (cm_fn_004cf6d0(receiver, player_record) == 0) {
                receiver = D(0x00ae1050);
                if (cm_fn_004cf5a0(receiver, player_record) != 0) {
                    receiver = D(0x00ae1050);
                    pointer = cm_fn_004cf5a0(receiver, player_record);
                    ++W(pointer + 0x3bu);
                }
            }
rotate:
            al = B(history + 0x25u);
            cl = B(history + 0x24u);
            dl = (u8)((al << 4) | (cl >> 4));
            cl = B(history + 0x26u);
            B(history + 0x24u) = dl;
            dl = (u8)((cl << 4) | (al >> 4));
            B(history + 0x25u) = dl;
            al = B(entry + 0x35u);
            al = (u8)(((al ^ cl) & 15u) ^ cl);
            B(history + 0x26u) = al;
        }
    }
    return 1;
}
