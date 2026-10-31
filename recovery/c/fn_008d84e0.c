#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u8 CM_STDCALL cm_fn_008e4ae0(u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);

/* Preferred-image VA008d84e0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12/full incidental EAX. Scratch2d8: name300 atL1c,
   reason400 atL148; all saved DWORD/BYTE fields use their original offsets.
   8e4ae0 RET32 takes five DWORDs, two zero words of a double, and a pointer.
   Its entry overwrites ECX before calls; the caller's saved receiver is dead.
   No floating-point value is evaluated here. All callees remain original
   boundaries, including the existing capped 8e4ae0 source. */
u32 CM_THISCALL cm_fn_008d84e0(u32 self, u32 event, volatile u32 mode,
                              volatile u32 text_object)
{
    u32 scratch[0x2d8 / 4];
    u32 l = (u32)scratch, person, first, club, other, value;
    u32 base, offset, other_offset, receiver, second, third, extra;
    u32 gender, other_gender, name, other_name, eax;
    u8 reason_present;
    D(l + 0x10) = self;
    person = cm_fn_00785830(event, 0);
    first = cm_fn_00785830(event, 1);
    value = cm_fn_00785830(event, 2);
    D(l + 0xc) = value;
    value = cm_fn_00785830(event, 3);
    D(l + 8) = value;
    club = cm_fn_00785830(event, 5);
    value = cm_fn_00785830(event, 6);
    B(l + 3) = (u8)value;
    value = cm_fn_00785830(event, 7);
    D(l + 4) = value;
    value = cm_fn_00785830(event, 8);
    D(l + 0x18) = value;
    value = cm_fn_00785830(event, 9);
    D(l + 0x14) = value;
    if (mode != 0) {
        other = cm_fn_00785830(event, 4);
        third = D(l + 8);
        second = D(l + 0xc);
        cm_fn_0053e140(first, second, third, l + 0x1c, 300, 1, person);
        value = D(l + 4);
        extra = D(l + 0x14);
        B(l + 0x148) = 0;
        name = D(l + 0x18);
        reason_present = cm_fn_008e4ae0(other, club, name, extra, value, 0, 0, l + 0x148);
        if (other != 0xffffffffu) {
            if (B(l + 3) == 1) {
                base = D(0x00ae23b4);
                offset = club * 0x245u;
                other_offset = other * 0x245u;
                gender = (u32)(s32)(s8)B(offset + base + 0x52);
                name = offset + base + 0x38;
                D(l + 4) = gender;
                other_gender = (u32)(s32)(s8)B(base + other_offset + 0x52);
                other_name = base + other_offset + 0x38;
                gender = D(l + 4);
                cm_fn_0066f4e0(0x00de1f64, 0x00a94f30, other_gender, other_gender,
                              gender, gender, other_name, l + 0x1c, name);
            } else if (reason_present != 0) {
                base = D(0x00ae23b4);
                offset = club * 0x245u;
                other_offset = other * 0x245u;
                gender = (u32)(s32)(s8)B(offset + base + 0x52);
                D(l + 4) = gender;
                name = offset + base + 0x38;
                other_gender = (u32)(s32)(s8)B(base + other_offset + 0x52);
                D(l + 0x10) = other_gender;
                gender = D(l + 4);
                other_name = base + other_offset + 0x38;
                other_gender = D(l + 0x10);
                cm_fn_0066f4e0(0x00de1f64, 0x00a94e8c, other_gender, other_gender,
                              gender, gender, other_name, l + 0x1c, name, l + 0x148);
            } else {
                offset = club * 0x245u;
                other_offset = other * 0x245u;
                base = D(0x00ae23b4);
                gender = (u32)(s32)(s8)B(offset + base + 0x52);
                name = offset + base + 0x38;
                D(l + 4) = gender;
                other_gender = (u32)(s32)(s8)B(base + other_offset + 0x52);
                other_name = base + other_offset + 0x38;
                gender = D(l + 4);
                cm_fn_0066f4e0(0x00de1f64, 0x00a94e04, other_gender, other_gender,
                              gender, gender, other_name, l + 0x1c, name);
            }
        } else if (reason_present != 0) {
            offset = club * 0x245u;
            base = D(0x00ae23b4);
            gender = (u32)(s32)(s8)B(offset + base + 0x52);
            name = offset + base + 0x38;
            cm_fn_0066f4e0(0x00de1f64, 0x00a94d84, gender, gender,
                          l + 0x1c, name, l + 0x148);
        } else {
            base = D(0x00ae23b4);
            offset = club * 0x245u;
            gender = (u32)(s32)(s8)B(offset + base + 0x52);
            name = offset + base + 0x38;
            cm_fn_0066f4e0(0x00de1f64, 0x00a94d20, gender, gender, l + 0x1c, name);
        }
        receiver = text_object;
        cm_fn_004ae660(receiver, 0x00de1f64);
        if ((s32)other > -1) {
            base = D(0x00ae23b4);
            other_offset = other * 0x245u;
            receiver = text_object;
            value = base + other_offset;
            cm_fn_004ae8a0(receiver, value + 0x38, 0x7d5, value);
        } else receiver = text_object;
        base = D(0x00ae23b4);
        value = offset + base;
        cm_fn_004ae8a0(receiver, value + 0x38, 0x7d5, value);
        third = D(l + 8);
        second = D(l + 0xc);
        offset = person * 110u;
        base = D(0x00ae23bc);
        eax = cm_fn_0053e990(offset + base, first, second, third, 0);
        if (eax == 0) return eax;
        base = D(0x00ae23bc);
        return cm_fn_004ae8a0(receiver, l + 0x1c, 0x7d7, offset + base);
    }
    third = D(l + 8);
    second = D(l + 0xc);
    cm_fn_0053e140(first, second, third, l + 0x1c, 300, 5, person);
    offset = club * 0x245u;
    base = D(0x00ae23b4);
    gender = (u32)(s32)(s8)B(offset + base + 0x52);
    name = offset + base + 0x38;
    cm_fn_0066f4e0(0x00de1f64, 0x00a94cc4, gender, gender, l + 0x1c, name);
    receiver = text_object;
    cm_fn_004ae660(receiver, 0x00de1f64);
    third = D(l + 8);
    second = D(l + 0xc);
    other_offset = person * 110u;
    base = D(0x00ae23bc);
    eax = cm_fn_0053e990(other_offset + base, first, second, third, 0);
    if (eax != 0) {
        base = D(0x00ae23bc);
        receiver = text_object;
        cm_fn_004ae8a0(receiver, l + 0x1c, 0x7d7, other_offset + base);
    }
    base = D(0x00ae23b4);
    receiver = text_object;
    value = offset + base;
    return cm_fn_004ae8a0(receiver, value + 0x38, 0x7d5, value);
}
