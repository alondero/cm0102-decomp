#include "cmre.h"
/* Preferred-image VA 0x0086c390, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve full EAX. When the cached low byte of the sixth stack word is
   not 1, the original returns the control pointer, signed date result,
   search result, player ID, or last original-call result at 0086c632.
   004cda70's seven stack words retain the double bits 3ff00000:00000000. */
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern void CM_THISCALL cm_fn_0058e340(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32, u32, u32);
extern s32 CM_THISCALL cm_fn_0054cdb0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00849ce0(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cda70(u32, u32, u32, u32, u32, u32, u32, u32);
extern s32 CM_CDECL cm_fn_0090cfc0(s32);
extern u32 CM_CDECL cm_fn_0058a070(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0086f5b0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_0086c390(u32 self, u32 player, u32 club,
                             u32 other, u32 value, u32 attribute_value, u32 mode)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 scratch[0x440];
    u32 local = (u32)scratch;
    u32 predecessor = CM_FS(u32, 0);
    u32 id, base, row, receiver, first, second, line, text;
    u32 found, index, mapped_club, record, control, date0, date1, result;
    u8 flag, attribute, decision;
    frame.state = 0xffffffffu;
    frame.handler = 0x00962cdb;
    frame.prev = predecessor;
    CM_FS(u32, 0) = (u32)&frame;
    if (player == 0 || other == 0) {
        first = local + 0x340u;
        second = local + 0x140u;
        line = 0x796;
        goto error;
    }
    cm_fn_0054a110(local);
    frame.state = 0;
    cm_fn_00417c10(local, local + 0x38u, 0x00ae2c90);
    B(local + 0xcu) = 0x34;
    if (club != 0) id = D(club);
    else id = 0xffffffffu;
    D(local + 8u) = id;
    id = D(other);
    decision = (u8)mode;
    D(local + 0xdu) = id;
    D(local + 0x11u) = value & 0xffffu;
    D(local + 0x15u) = (u32)(s32)(s8)(u8)attribute_value;
    D(local + 0x19u) = (u32)(s32)(s8)decision;
    if (local != 0) {
        id = D(player);
        base = D(self + 0xcu);
        row = base + id * 8u;
        receiver = D(self);
        cm_fn_0058e340(receiver, row, row + 4u, local);
    } else {
        first = local + 0x240u;
        second = local + 0x40u;
        line = 0x1010;
error:
        cm_fn_00944cff(0x00a83b78, 0, 0, first, second);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, first, second, line);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        if (player == 0 || other == 0) goto done;
    }
    flag = B(player + 0x3du);
    if (flag == 5 || flag == 12) goto special;
    flag = B(player + 0x28u);
    if (flag == 5 || flag == 12) goto special;
    result = D(player + 0x61u);
    if (result == 0) goto done;
    receiver = D(0x00ae1050);
    found = cm_fn_004cf5a0(receiver, player);
    D(0x00ae1080) = found;
    result = found;
    if (found == 0) goto done;
    flag = B(found + 0x35u) & 0x3fu;
    if (flag == 5) goto done;
    index = D(found + 4u);
    base = D(0x00ae23b4);
    result = base + index * 581u;
    if (result == 0) goto done;
    receiver = D(0x00ae1050);
    found = cm_fn_004cf5a0(receiver, player);
    D(0x00ae1080) = found;
    mapped_club = 0;
    if (found != 0) {
        flag = B(found + 0x35u) & 0x3fu;
        if (flag != 5) {
            index = D(found + 4u);
            base = D(0x00ae23b4);
            mapped_club = base + index * 581u;
        }
    }
    receiver = D(0x00ae1050);
    record = cm_fn_004cf5f0(receiver, player, mapped_club);
    result = record;
    if (record == 0) goto done;
    control = D(0x00de1f54);
    result = control;
    if (control != 0) {
        date1 = D(record + 0x29u);
        date0 = D(record + 0x25u);
        result = (u32)cm_fn_0054cdb0(0x00ae2c90, date0, date1);
        if ((s32)result > 300) {
            receiver = D(0x00ae1050);
            found = cm_fn_004cf5a0(receiver, player);
            D(0x00ae1080) = found;
            mapped_club = 0;
            if (found != 0) {
                flag = B(found + 0x35u) & 0x3fu;
                if (flag != 5) {
                    index = D(found + 4u);
                    base = D(0x00ae23b4);
                    mapped_club = base + index * 581u;
                }
            }
            control = D(0x00de1f54);
            receiver = D(control + 0x856u);
            found = cm_fn_00849ce0(receiver, mapped_club, player, 0, 0);
            result = found;
            if (found != 0) {
                id = D(found + 0xfu);
                index = D(player);
                result = index;
                if (id == index) {
                    receiver = D(0x00ae1050);
                    result = cm_fn_004cda70(receiver, player, 0, 0, 0xffffffffu,
                                   0, 0x3ff00000, 1);
                }
            }
        }
    }
    if (decision != 1) goto done;
    result = D(record + 0x45u);
    if (result != 0) goto done;
    attribute = B(player + 0x59u);
    result = attribute;
    if ((s8)attribute <= 15) goto done;
    if ((s8)B(player + 0x5bu) <= 10) goto done;
    result = (u32)cm_fn_0090cfc0((s32)(s8)attribute);
    if ((s32)result <= 5) goto done;
    result = cm_fn_0058a070(player, club, other);
    goto done;
special:
    result = cm_fn_0086f5b0(self, player, 1);
    if (result != 0) W(result + 0x9eu) = (u16)(W(result + 0x9eu) + 1u);
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
