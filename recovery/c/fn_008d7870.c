#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);

/* Preferred-image VA008d7870/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL3/RET12/full EAX. Scratch144/name300L18. Nonzero mode stores
   g0L14/g1L4/g2L8/g3L10/contextLc; zero mode stores g3L4/g4L8/flagBL3.
   All nine formats and actual varargs independently checked. 785830's
   valid fixed indices define full EAX; original53e140/53e2e0 initialize
   the first name byte before use. All helpers remain original boundaries. */
u32 CM_STDCALL cm_fn_008d7870(volatile u32 event, volatile u32 mode,
                             volatile u32 text_object)
{
    u32 active = mode;
    u32 scratch[0x144 / 4];
    u32 l = (u32)scratch, name = l + 0x18;
    u32 object, value, person, first, second, third, club, other, enabled, flag;
    u32 context, current, base, format, receiver, offset, club_address;
    u32 gender, other_gender, result;
    if (active != 0) {
        current = (u32)(s32)(s16)W(0x00b77c7a);
        D(l + 0xc) = 0;
        value = D(0x00b74c26 + current * 0x300u);
        if (value != 0) D(l + 0xc) = D(value + 0x39);
        object = event;
        value = cm_fn_00785830(object, 0);
        D(l + 0x14) = value;
        value = cm_fn_00785830(object, 1);
        D(l + 4) = value;
        value = cm_fn_00785830(object, 2);
        D(l + 8) = value;
        value = cm_fn_00785830(object, 3);
        D(l + 0x10) = value;
        club = cm_fn_00785830(object, 4);
        other = cm_fn_00785830(object, 5);
        enabled = cm_fn_00785830(object, 6) != 0;
        flag = (u8)cm_fn_00785830(object, 9);
        person = D(l + 0x14);
        third = D(l + 0x10);
        second = D(l + 8);
        first = D(l + 4);
        cm_fn_0053e140(first, second, third, name, 300, 1, person);
        if (enabled == 0 && club == 0xffffffffu) {
            offset = other * 0x245u;
            base = D(0x00ae23b4);
            gender = (u32)(s32)(s8)B(offset + base + 0x52);
            cm_fn_0066f4e0(0x00de1f64, 0x00a94aa4, gender, gender, name,
                           offset + base + 0x38);
        } else {
            if (enabled != 0) format = 0x00a947d8;
            else {
                context = D(l + 0xc);
                if (flag == 1) {
                    if (context != 0 && D(context) == other) format = 0x00a94a14;
                    else format = 0x00a94988;
                } else {
                    if (context != 0 && D(context) == other) format = 0x00a948fc;
                    else format = 0x00a94874;
                }
            }
            base = D(0x00ae23b4);
            offset = other * 0x245u;
            club_address = base + club * 0x245u;
            gender = (u32)(s32)(s8)B(club_address + 0x52);
            other_gender = (u32)(s32)(s8)B(offset + base + 0x52);
            cm_fn_0066f4e0(0x00de1f64, format, gender, gender, other_gender,
                           other_gender, club_address + 0x38, offset + base + 0x38, name);
        }
        receiver = text_object;
        cm_fn_004ae660(receiver, 0x00de1f64);
        if ((s32)club > -1) {
            base = D(0x00ae23b4);
            club_address = base + club * 0x245u;
            cm_fn_004ae8a0(receiver, club_address + 0x38, 0x7d5, club_address);
        }
        base = D(0x00ae23b4);
        club_address = offset + base;
        cm_fn_004ae8a0(receiver, club_address + 0x38, 0x7d5, club_address);
        person = D(l + 0x14);
        third = D(l + 0x10);
        offset = person * 0x6eu;
        base = D(0x00ae23bc);
        first = D(l + 4);
        second = D(l + 8);
        result = cm_fn_0053e990(offset + base, first, second, third, 0);
        if (result == 0) return result;
        base = D(0x00ae23bc);
        return cm_fn_004ae8a0(receiver, name, 0x7d7, offset + base);
    }
    object = event;
    person = cm_fn_00785830(object, 0);
    first = cm_fn_00785830(object, 1);
    second = cm_fn_00785830(object, 2);
    value = cm_fn_00785830(object, 3);
    D(l + 4) = value;
    value = cm_fn_00785830(object, 4);
    D(l + 8) = value;
    value = cm_fn_00785830(object, 9);
    third = D(l + 4);
    B(l + 3) = (u8)value;
    cm_fn_0053e140(first, second, third, name, 300, 5, person);
    club = D(l + 8);
    if (club == 0xffffffffu) format = 0x00a94790;
    else if (B(l + 3) == 1) format = 0x00a94754;
    else format = 0x00a94714;
    cm_fn_0066f4e0(0x00de1f64, format, name);
    receiver = text_object;
    cm_fn_004ae660(receiver, 0x00de1f64);
    offset = person * 0x6eu;
    base = D(0x00ae23bc);
    third = D(l + 4);
    result = cm_fn_0053e990(offset + base, first, second, third, 0);
    if (result == 0) return result;
    base = D(0x00ae23bc);
    receiver = text_object;
    return cm_fn_004ae8a0(receiver, name, 0x7d7, offset + base);
}
