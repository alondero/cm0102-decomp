#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);

/* Preferred-image VA00587800/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL3/full EAX from the final club link. Scratch184: g3L0/g2L4/g9L8/
   g1Lc/g10L10/g8L14/g6L18; three120-byte names atL1c/L94/L10c.
   All eleven getters, nine format arities, and receiver reloads follow
   original instructions. Callees remain original boundaries. Name helper
   initializes byte0 before formatting; no scratch initialization invented. */
u32 CM_CDECL cm_fn_00587800(volatile u32 event, volatile u32 mode,
                           volatile u32 text_object)
{
    u32 scratch[0x184 / 4];
    u32 l = (u32)scratch, name = l + 0x1c, other_name = l + 0x94;
    u32 alternate_name = l + 0x10c;
    u32 object = event, person, club, other, value, first, second, third;
    u32 base, club_offset, club_address, gender, city, city_gender, style;
    u32 receiver, person_offset, other_offset, result;
    person = cm_fn_00785830(object, 0);
    value = cm_fn_00785830(object, 1);
    D(l + 0xc) = value;
    value = cm_fn_00785830(object, 2);
    D(l + 4) = value;
    value = cm_fn_00785830(object, 3);
    D(l) = value;
    cm_fn_00785830(object, 4);
    club = cm_fn_00785830(object, 5);
    value = cm_fn_00785830(object, 6);
    D(l + 0x18) = value;
    other = cm_fn_00785830(object, 7);
    value = cm_fn_00785830(object, 8);
    D(l + 0x14) = value;
    value = cm_fn_00785830(object, 9);
    D(l + 8) = value;
    value = cm_fn_00785830(object, 10);
    third = D(l);
    second = D(l + 4);
    first = D(l + 0xc);
    D(l + 0x10) = value;
    cm_fn_0053e140(first, second, third, name, 120, 1, person);
    third = D(l);
    second = D(l + 4);
    cm_fn_0053e140(first, second, third, alternate_name, 120, 5, person);
    if ((s32)other > -1) {
        third = D(l + 0x10);
        second = D(l + 8);
        first = D(l + 0x14);
        cm_fn_0053e140(first, second, third, other_name, 120, 1, other);
    }
    club_offset = club * 0x245u;
    if (mode == 0) {
        base = D(0x00ae23b4);
        gender = (u32)(s32)(s8)B(club_offset + base + 0x52);
        cm_fn_0066f4e0(0x00de1f64, 0x009c4d00, gender, gender,
                       club_offset + base + 0x38, alternate_name);
    } else {
        style = D(l + 0x18);
        if (style != 0 && style != 1 && (s32)other > -1) {
            base = D(0x00ae23b4);
            city = D(club_offset + base + 0x69);
            if (city != 0) {
                gender = (u32)(s32)(s8)B(club_offset + base + 0x52);
                city_gender = (u32)(s32)(s8)B(city + 0x37);
                cm_fn_0066f4e0(0x00de1f64, 0x009c52b8, gender, gender, city_gender,
                               club_offset + base + 0x38, name, other_name, alternate_name, city + 4);
            } else {
                gender = (u32)(s32)(s8)B(club_offset + base + 0x52);
                cm_fn_0066f4e0(0x00de1f64, 0x009c5184, gender, gender,
                               club_offset + base + 0x38, name, other_name, alternate_name);
            }
        } else {
            base = D(0x00ae23b4);
            gender = (u32)(s32)(s8)B(club_offset + base + 0x52);
            club_address = club_offset + base + 0x38;
            if (style == 0) {
                if ((s32)other <= -1)
                    cm_fn_0066f4e0(0x00de1f64, 0x009c4d60, gender, gender, club_address, name);
                else if (other & 1u)
                    cm_fn_0066f4e0(0x00de1f64, 0x009c4ef4, gender, gender, club_address, other_name, name);
                else
                    cm_fn_0066f4e0(0x00de1f64, 0x009c4df0, gender, gender, club_address, name, other_name, alternate_name);
            } else if (style == 1) {
                if ((s32)other > -1)
                    cm_fn_0066f4e0(0x00de1f64, 0x009c5040, gender, gender, club_address, other_name, name);
                else
                    cm_fn_0066f4e0(0x00de1f64, 0x009c4fb0, gender, gender, club_address, name);
            } else
                cm_fn_0066f4e0(0x00de1f64, 0x009c5100, gender, gender, club_address, name);
        }
    }
    receiver = text_object;
    cm_fn_004ae660(receiver, 0x00de1f64);
    third = D(l);
    second = D(l + 4);
    first = D(l + 0xc);
    base = D(0x00ae23bc);
    person_offset = person * 0x6eu;
    result = cm_fn_0053e990(person_offset + base, first, second, third, 0);
    if (result != 0) {
        base = D(0x00ae23bc);
        receiver = text_object;
        cm_fn_004ae8a0(receiver, name, 0x7d7, person_offset + base);
        base = D(0x00ae23bc);
        cm_fn_004ae8a0(receiver, alternate_name, 0x7d7, person_offset + base);
    } else receiver = text_object;
    if ((s32)other > -1) {
        third = D(l + 0x10);
        second = D(l + 8);
        first = D(l + 0x14);
        base = D(0x00ae23bc);
        other_offset = other * 0x6eu;
        result = cm_fn_0053e990(other_offset + base, first, second, third, 0);
        if (result != 0) {
            base = D(0x00ae23bc);
            cm_fn_004ae8a0(receiver, other_name, 0x7d7, other_offset + base);
        }
    }
    base = D(0x00ae23b4);
    club_address = club_offset + base;
    return cm_fn_004ae8a0(receiver, club_address + 0x38, 0x7d5, club_address);
}
