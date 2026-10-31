#include "cmre.h"

/* Preferred-image VA 0x00743370, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The null-player exit retains the incoming SEH predecessor in EAX. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00785790(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00544520(u32, u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32, u32, u32);

u32 CM_STDCALL cm_fn_00743370(u32 player, volatile u32 club,
                            volatile u32 kind, volatile u32 last)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0xe0];
    u32 local = (u32)scratch, prev, result, person, owner, pointer, value;
    u32 category, reason, date_low, date_high, manager, item;
    s8 skill, requested;
    prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095d7bb;
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    result = prev;
    if (player == 0) goto done;
    result = D(player + 0x61);
    if (result == 0) goto done;
    owner = club;
    if (owner == 0) goto done;
    result = cm_fn_00600a50(owner, 0, 1, 0, 0);
    if (result == 0) goto done;
    cm_fn_0077bbe0(local, 0xbcf, 0);
    value = D(player);
    frame.state = 0;
    cm_fn_00785790(local, 0, value);
    pointer = D(player + 4);
    value = D(pointer + 0x33);
    cm_fn_00785790(local, 1, value);
    pointer = D(player + 8);
    value = D(pointer + 0x33);
    cm_fn_00785790(local, 2, value);
    pointer = D(player + 0x0c);
    value = pointer != 0 ? D(pointer + 0x33) : 0xffffffffu;
    cm_fn_00785790(local, 3, value);
    value = D(owner);
    cm_fn_00785790(local, 4, value);
    person = D(player + 0x61);
    if ((s16)W(person + 0x0b) > 0x109a) {
        category = 1;
        if ((s8)B(player + 0x57) > 15 && (s8)B(player + 0x5d) < 6) {
            reason = 1;
            goto classified;
        }
        requested = (s8)kind;
        if (requested == 32 || requested == 1 || requested == 30) {
            if ((s8)B(player + 0x59) > 15 && (s8)B(player + 0x5b) > 10) {
                date_high = D(player + 0x42);
                date_low = D(player + 0x3e);
                value = cm_fn_0054cdb0(0x00ae2c90, date_low, date_high);
                if ((s32)value > 0xd2) {
                    reason = 3;
                    goto classified;
                }
            }
        }
    } else {
        category = 2;
        skill = (s8)B(player + 0x5b);
        if (skill < 10 && (s8)B(player + 0x59) < 6) {
            reason = 1;
            goto classified;
        }
        requested = (s8)kind;
        if (requested == 32 || requested == 1 || requested == 30) {
            if ((s8)B(player + 0x59) > 15 && skill > 10) {
                date_high = D(player + 0x42);
                date_low = D(player + 0x3e);
                value = cm_fn_0054cdb0(0x00ae2c90, date_low, date_high);
                if ((s32)value > 0xd2) {
                    reason = 3;
                    goto classified;
                }
            }
        }
    }
    reason = 2;
classified:
    cm_fn_00785790(local, 5, category);
    cm_fn_00785790(local, 6, reason);
    item = (u32)(s32)(s8)kind;
    cm_fn_00785790(local, 7, item);
    item = (u32)(s32)(s16)last;
    cm_fn_00785790(local, 8, item);
    value = cm_fn_00544520(player, 0);
    cm_fn_00785790(local, 9, value != 0);
    manager = D(0x00b7431c);
    result = cm_fn_007861e0(manager, local, owner);
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
