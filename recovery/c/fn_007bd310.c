/* Preferred-image VA 007bd310, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   RET 8; ECX receiver. The sole indexed caller overwrites AX before use;
   its later EAX uses follow full writes. No observed result contract. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_004afa80(u32 p, u32 s);
extern u32 CM_THISCALL cm_fn_005afde0(u32 base, u32 *arg);
extern void CM_THISCALL cm_fn_005b09d0(u32 self, u32 person, u32 club);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip, u8 flag, u32 person, u32 force);
extern u32 CM_THISCALL cm_fn_007bd700(u32 self, u32 person, u32 allocate);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

void CM_THISCALL cm_fn_007bd310(u32 self, u32 match, u32 data)
{
    u8 work[0x200];
    u32 clubbase, club, row, person, id, related, total, sidebase, slot, result;
    u32 i, temporary;
    u8 side, index, count, position, value;
    clubbase = D(0x00ae23b4u);
    id = D(self);
    club = clubbase + id * 581u;
    row = self + 4u;
    for (i = 50; i != 0; --i, row += 418u) {
        id = D(row);
        if (id == 0xffffffffu)
            continue;
        person = D(0x00ae23bcu) + id * 110u;
        if (person != 0) {
            total = D(0x00ae2364u);
            id = D(person);
            if ((s32)id >= (s32)total)
                continue;
        }
        related = D(person + 0x39u);
        if (related != 0) {
            id = D(related);
            temporary = D(self);
            if (id == temporary)
                continue;
        }
        D(row) = 0xffffffffu;
        W(row + 4u) = 0;
        B(row + 0x1a0u) = 0;
        B(row + 0x1a1u) = 0;
    }
    id = D(self);
    total = D(match + 0xcu);
    if (total == id) {
        total = D(match + 0x10u);
        side = 0;
    } else {
        side = 1;
    }
    sidebase = (u32)side * 1573u;
    index = 0;
    do {
        slot = data + 0x190u + sidebase + (u32)(s32)(s8)index * 73u;
        id = D(slot);
        if (id == 0xffffffffu)
            goto next;
        if (D(slot + 4u) == 0xffffffffu)
            goto next;
        if ((s8)index > 10 && B(slot + 0x32u) == 0xff)
            goto next;
        person = D(0x00ae23bcu) + id * 110u;
        if (person != 0) {
            temporary = D(0x00ae2364u);
            id = D(person);
            if ((s32)id >= (s32)temporary)
                goto next;
        }
        if (D(club + 0xcfu) != 0) {
            result = cm_fn_00600a50(club, 0, 1, 0, 0);
            if (result != 0 && B(0x009cc270u) != 0) {
                related = D(club + 0xcfu);
                temporary = D(0x00ae2a60u);
                result = cm_fn_005afde0(temporary, (u32 *)related);
                if (result != 0)
                    cm_fn_005b09d0(result, person, 0);
            }
        }
        row = cm_fn_007bd700(self, person, 1);
        if (row == 0) {
            cm_fn_00944cff(0x00a701b0u, 0, 0, work + 0x100, work);
            result = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, result, work + 0x100, work, 0x2065u);
            cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
            D(0x00b67a34u) = 0;
            return;
        }
        count = B(row + 0x1a1u);
        W(row + 4u) = (u16)(W(row + 4u) + 1u);
        position = B(row + 0x1a0u);
        if (count == 10) {
            count = (u8)(position + 1u);
            B(row + 0x1a0u) = count;
            if (count == 10)
                B(row + 0x1a0u) = 0;
        } else {
            position = (u8)(position + count);
            if ((s8)position >= 10)
                position = (u8)(position - 10u);
            count = (u8)(count + 1u);
            B(row + 0x1a1u) = count;
        }
        row += (u32)(s32)(s8)position * 41u;
        B(row + 0x17u) = B(slot + 0x27u);
        B(row + 0x18u) = B(slot + 0x22u);
        W(row + 6u) = W(match + 0x2au);
        W(row + 8u) = W(match + 0x28u);
        B(row + 0x16u) = B(slot + 0x21u);
        B(row + 0x1bu) = B(slot + 0x35u);
        B(row + 0x1au) = B(slot + 0x2du);
        B(row + 0x19u) = B(slot + 0x2cu);
        value = B(match + (side == 0 ? 0x43u : 0x44u));
        B(row + 0xeu) = value;
        value = B(match + (side == 0 ? 0x44u : 0x43u));
        B(row + 0xfu) = value;
        D(row + 0x10u) = D(match + 8u);
        clubbase = D(0x00ae23b4u);
        id = D(self);
        result = cm_fn_004afa80(clubbase + id * 581u, match);
        B(row + 0x14u) = (u8)result;
        B(row + 0x1cu) = B(slot + 0x3eu);
        B(row + 0x1du) = B(slot + 0x3fu);
        B(row + 0x1eu) = B(slot + 0x40u);
        B(row + 0x1fu) = B(slot + 0x41u);
        B(row + 0x20u) = B(slot + 0x42u);
        B(row + 0x21u) = B(slot + 0x43u);
        B(row + 0x22u) = B(slot + 0x44u);
        B(row + 0x23u) = B(slot + 0x45u);
        B(row + 0x24u) = B(slot + 0x46u);
        B(row + 0x25u) = B(slot + 0x47u);
        B(row + 0x26u) = B(slot + 0x28u);
        B(row + 0x27u) = B(slot + 0x25u);
        B(row + 0x28u) = B(slot + 0x26u);
        B(row + 0x29u) = B(slot + 0x29u);
        B(row + 0x2au) = B(slot + 0x2au);
        B(row + 0x2bu) = B(slot + 0x2bu);
        B(row + 0x2cu) = B(slot + 0x23u);
        B(row + 0x2du) = B(slot + 0x24u);
        B(row + 0x2eu) = 0;
        id = D(match);
        temporary = D(0x009cf684u);
        D(row + 0xau) = id == temporary ? 0u - total : total;
        if ((s8)index < 11) {
            B(row + 0x2eu) = 1;
            value = B(slot + 0x34u);
            if ((s8)value <= -1) {
                if ((s8)B(slot + 0x30u) > -1) {
                    B(row + 0x2eu) = 5;
                    B(row + 0x15u) = B(slot + 0x30u);
                    goto next;
                }
                value = B(data + 0x4du);
            }
        } else {
            B(row + 0x2eu) = 2;
            count = B(slot + 0x32u);
            if (count == 0xff) {
                B(row + 0x15u) = 0;
                goto next;
            }
            value = B(slot + 0x34u);
            if ((s8)value <= -1) {
                if ((s8)B(slot + 0x30u) > -1) {
                    B(row + 0x2eu) = 6;
                    value = B(slot + 0x30u);
                    B(row + 0x15u) = (u8)(value - B(slot + 0x32u));
                    goto next;
                }
                value = B(data + 0x4du);
            }
            value = (u8)(value - count);
        }
        B(row + 0x15u) = value;
next:
        index = (u8)(index + 1u);
    } while ((s8)index < 20);
}
