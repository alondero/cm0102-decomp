#include "cmre.h"
/* Preferred-image VA 0x006b66a0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The original tests full EAX after 005c2990, despite its draft byte return. */
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern u32 CM_THISCALL cm_fn_005c2990(u32, u32);
extern u8 CM_CDECL cm_fn_0048dd70(u32, u32);
extern u32 CM_THISCALL cm_fn_007f9720(u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

u32 CM_CDECL cm_fn_006b66a0(u32 report)
{
    u32 pointer, address, club, eax, id, index, base, nation, receiver;
    s32 rating, other;
    u8 category;
    if (D(0x00a15704)) {
        address = 0x00b63d60;
        do {
            pointer = D(address);
            if (pointer && (B(pointer + 0x11c) & 2)) {
                pointer = D(pointer + 0x71);
                if (pointer) {
                    index = D(pointer);
                    B(0x00b67a58 + index) = 1;
                }
            }
            address += 0x48;
        } while ((s32)address < 0x00b64738);
        D(0x00a15704) = 0;
    }
    if (!D(0x00dd7118)) return 1;
    club = D(report + 0x1c);
    if (cm_fn_00600a50(club, 1, 1, 0, 0)) return 0;
    club = D(report + 0x20);
    if (cm_fn_00600a50(club, 1, 1, 0, 0)) return 0;
    club = D(report + 0x1c);
    if (cm_fn_00540a50(club, 0, 1)) {
        club = D(report + 0x1c);
        eax = cm_fn_00540a50(club, 0, 1);
        if (cm_fn_00600a50(eax, 1, 1, 0, 0)) return 0;
    }
    club = D(report + 0x20);
    if (cm_fn_00540a50(club, 0, 1)) {
        club = D(report + 0x20);
        eax = cm_fn_00540a50(club, 0, 1);
        if (cm_fn_00600a50(eax, 1, 1, 0, 0)) return 0;
    }
    id = D(report);
    eax = D(0x009cf7bc);
    if (id == eax) return 0;
    if (id == D(0x009cf6e4)) return 0;
    if (id == D(0x009cf958)) {
        index = D(0x009cf2e4);
        base = D(0x00ae23a8);
        if (B(base + index * 0x122u + 0x11c) & 2) return 0;
        index = D(0x009cf510);
        if (B(base + index * 0x122u + 0x11c) & 2) return 0;
    }
    if (id == D(0x009cf684)) {
        club = D(report + 0x1c);
        if (!cm_fn_00600a50(club, 1, 1, 0, 0)) {
            club = D(report + 0x20);
            if (!cm_fn_00600a50(club, 1, 1, 0, 0)) goto final;
        }
    }
    index = D(0x00ae2364);
    base = D(0x00ae23bc);
    address = base + index * 0x6eu - 0x35u;
    for (index = 0; index < 16; ++index) {
        if (address - 0x39u && B(address + 0x26) == 1) {
            pointer = D(address);
            if (pointer) {
                club = D(report + 0x1c);
                nation = D(pointer + 0x57);
                if (nation == D(club + 0x57)) return 0;
                club = D(report + 0x20);
                if (nation == D(club + 0x57)) return 0;
            }
        }
        address -= 0x6e;
    }
    club = D(report + 0x20);
    eax = cm_fn_0053b900((u32 *)club);
    if (!eax) {
        pointer = D(report + 0x14);
        receiver = D(0x00ae2c74);
        eax = cm_fn_005c2990(receiver, pointer);
        if (eax) goto ratings;
    }
    id = D(report);
    eax = D(0x009cf6f8);
    if (id == eax) {
ratings:
        club = D(report + 0x20);
        pointer = D(report + 0x1c);
        rating = (s16)W(club + 0x80);
        other = (s16)W(pointer + 0x80);
        if (rating + other > 12000) return 0;
        goto final;
    }
    pointer = D(report + 0x14);
    category = cm_fn_0048dd70(pointer, report);
    address = 0x00b63d74;
    index = 0;
    do {
        if (B(address) == category) {
            category = (u8)index;
            break;
        }
        address += 0x48;
        ++index;
    } while ((s32)address < 0x00b6474c);
    if (!category || category == 1) return 0;
    if (category == 3 || category == 4 || category == 5 || category == 7 || category == 6) {
        pointer = D(report + 0x14);
        pointer = D(pointer + 0x59);
        if (pointer) {
            index = D(pointer);
            if (B(0x00b67a58 + index)) return 0;
        }
    }
    if ((s8)category > -1) {
        pointer = D(0x00b63d60 + (u32)(s32)(s8)category * 72u);
        if (pointer && (B(pointer + 0x11c) & 2)) return 0;
    }
final:
    receiver = D(0x00b74448);
    return cm_fn_007f9720(receiver, report) == 0;
}
