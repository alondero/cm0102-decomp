#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);

/* Preferred-image VA008e2cd0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL3/RET12/full EAX, including the AL overwrite at8e2ffc.
   Scratch280/names300L28,L154. True:g0L24/g1L14/g2L18/g3L10/g6L1c/
   flagBL3/g8Lc/g9L8/g10L4/g11L20. Six format arities checked, including
   the unconditional two-club special branch. Original helper boundaries. */
u32 CM_STDCALL cm_fn_008e2cd0(volatile u32 event, volatile u32 mode,
                             volatile u32 text_object)
{
    u32 active = mode;
    u32 scratch[0x280 / 4];
    u32 l = (u32)scratch, name = l + 0x28, other_name = l + 0x154;
    u32 object = event, person, first, second, third, value, club, other_club;
    u32 club_offset, club_address, other_address, base, gender, other_gender;
    u32 receiver, offset, result, other_person, flag, format;
    if (active == 0) {
        person = cm_fn_00785830(object, 0);
        first = cm_fn_00785830(object, 1);
        second = cm_fn_00785830(object, 2);
        third = cm_fn_00785830(object, 3);
        cm_fn_0053e140(first, second, third, name, 300, 5, person);
        cm_fn_0066f4e0(0x00de1f64, 0x00a9ceac, 100, name);
        receiver = text_object;
        cm_fn_004ae660(receiver, 0x00de1f64);
        offset = person * 0x6eu;
        base = D(0x00ae23bc);
        result = cm_fn_0053e990(offset + base, first, second, third, 0);
        if (result == 0) return result;
        base = D(0x00ae23bc);
        receiver = text_object;
        return cm_fn_004ae8a0(receiver, name, 0x7d7, offset + base);
    }
    person = cm_fn_00785830(object, 0);
    D(l + 0x24) = person;
    value = cm_fn_00785830(object, 1);
    D(l + 0x14) = value;
    value = cm_fn_00785830(object, 2);
    D(l + 0x18) = value;
    value = cm_fn_00785830(object, 3);
    D(l + 0x10) = value;
    club = cm_fn_00785830(object, 4);
    other_club = cm_fn_00785830(object, 5);
    value = cm_fn_00785830(object, 6);
    D(l + 0x1c) = value;
    value = cm_fn_00785830(object, 7);
    third = D(l + 0x10);
    second = D(l + 0x18);
    B(l + 3) = (u8)value;
    first = D(l + 0x14);
    cm_fn_0053e140(first, second, third, name, 300, 1, person);
    flag = B(l + 3);
    if (flag != 0) {
        value = cm_fn_00785830(object, 8);
        D(l + 0xc) = value;
        value = cm_fn_00785830(object, 9);
        D(l + 8) = value;
        value = cm_fn_00785830(object, 10);
        D(l + 4) = value;
        value = cm_fn_00785830(object, 11);
        other_person = D(l + 0xc);
        first = D(l + 8);
        D(l + 0x20) = value;
        second = D(l + 4);
        cm_fn_0053e140(first, second, value, other_name, 300, 1, other_person);
        flag = B(l + 3);
        if (flag == 1 && D(l + 0x1c) == 0) {
            club_offset = club * 0x245u;
            base = D(0x00ae23b4);
            other_address = base + other_club * 0x245u;
            other_gender = (u32)(s32)(s8)B(other_address + 0x52);
            gender = (u32)(s32)(s8)B(club_offset + base + 0x52);
            cm_fn_0066f4e0(0x00de1f64, 0x00a9d25c, 100, other_gender, other_gender,
                           gender, gender, name, other_address + 0x38,
                           club_offset + base + 0x38, other_name);
            person = D(l + 0x24);
            goto formatted;
        }
        format = other_club == 0xffffffffu ? 0x00a9cef4 : 0x00a9cfc8;
    } else format = other_club == 0xffffffffu ? 0x00a9d0cc : 0x00a9d17c;
    base = D(0x00ae23b4);
    club_offset = club * 0x245u;
    if (other_club != 0xffffffffu) {
        other_address = base + other_club * 0x245u;
        other_gender = (u32)(s32)(s8)B(other_address + 0x52);
        gender = (u32)(s32)(s8)B(club_offset + base + 0x52);
        cm_fn_0066f4e0(0x00de1f64, format, 100, other_gender, other_gender, gender,
                       gender, name, other_address + 0x38, club_offset + base + 0x38);
    } else {
        gender = (u32)(s32)(s8)B(club_offset + base + 0x52);
        cm_fn_0066f4e0(0x00de1f64, format, 100, gender, gender, name,
                       club_offset + base + 0x38);
    }
formatted:
    receiver = text_object;
    cm_fn_004ae660(receiver, 0x00de1f64);
    base = D(0x00ae23b4);
    club_address = club_offset + base;
    cm_fn_004ae8a0(receiver, club_address + 0x38, 0x7d5, club_address);
    if (other_club != 0xffffffffu) {
        base = D(0x00ae23b4);
        other_address = base + other_club * 0x245u;
        cm_fn_004ae8a0(receiver, other_address + 0x38, 0x7d5, other_address);
    }
    third = D(l + 0x10);
    second = D(l + 0x18);
    first = D(l + 0x14);
    base = D(0x00ae23bc);
    offset = person * 0x6eu;
    result = cm_fn_0053e990(offset + base, first, second, third, 0);
    if (result != 0) {
        base = D(0x00ae23bc);
        result = cm_fn_004ae8a0(receiver, name, 0x7d7, offset + base);
    }
    flag = B(l + 3);
    result = (result & 0xffffff00u) | flag;
    if (flag == 0) return result;
    other_person = D(l + 0xc);
    third = D(l + 0x20);
    second = D(l + 4);
    base = D(0x00ae23bc);
    first = D(l + 8);
    offset = other_person * 0x6eu;
    result = cm_fn_0053e990(offset + base, first, second, third, 0);
    if (result == 0) return result;
    base = D(0x00ae23bc);
    return cm_fn_004ae8a0(receiver, other_name, 0x7d7, offset + base);
}
