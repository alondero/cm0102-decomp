#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
typedef u16 word_4daa00 __attribute__((may_alias));
#define W(a) CM_GLOBAL(volatile word_4daa00, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);

/* Preferred-image VA004daa00/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD3/RET12/full incidental EAX. Scratch140: kindBYTE L3,
   name keysL4/L8/Lc, manager flagL10, 300-byte nameL14.
   Each branch has its own output receiver reload timing. */
u32 CM_STDCALL cm_fn_004daa00(u32 event, volatile u32 mode_argument,
                             volatile u32 output_argument)
{
    u32 scratch[0x140 / 4];
    u32 l = (u32)scratch, buffer = l + 0x14, old_club, person, new_club;
    u32 value, selected, manager_index, manager, club, base, receiver;
    u32 first, second, third, offset, eax;
    s32 gender;
    u8 kind;

    old_club = cm_fn_00785830(event, 0);
    person = cm_fn_00785830(event, 2);
    value = cm_fn_00785830(event, 3);
    D(l + 0xc) = value;
    value = cm_fn_00785830(event, 4);
    D(l + 8) = value;
    value = cm_fn_00785830(event, 5);
    D(l + 4) = value;
    value = cm_fn_00785830(event, 6);
    B(l + 3) = (u8)value;
    new_club = cm_fn_00785830(event, 7);
    value = cm_fn_00785830(event, 8);
    manager_index = (u32)(s32)(s16)W(0x00b77c7a);
    selected = value == 1;
    manager = D(0x00b74c26 + manager_index * 768u);
    if (manager != 0) {
        manager = D(manager + 0x39);
        if (manager != 0) {
            value = D(manager);
            D(l + 0x10) = old_club == value;
        } else {
            D(l + 0x10) = 0;
        }
    } else {
        D(l + 0x10) = 0;
    }
    third = D(l + 4);
    second = D(l + 8);
    first = D(l + 0xc);
    cm_fn_0053e140(first, second, third, buffer, 300, 1, person);
    value = mode_argument;
    if (value == 0) {
        if (selected != 0) {
            value = D(l + 0x10);
            if (value == 0) {
                base = D(0x00ae23b4);
                club = base + old_club * 581u;
                gender = (s32)(s8)B(club + 0x52);
                cm_fn_0066f4e0(0x00de1f64, 0x009aeff4,
                               gender, gender, buffer, club + 0x38);
                goto first_append;
            }
        }
        kind = B(l + 3);
        if (kind == 5)
            cm_fn_0066f4e0(0x00de1f64, 0x009aefbc, 100, buffer);
        else if (kind == 6)
            cm_fn_0066f4e0(0x00de1f64, 0x009aef88, 100, buffer);
        else if (kind != 1 && selected == 0)
            cm_fn_0066f4e0(0x00de1f64, 0x009aef50, 100, buffer);
        else
            cm_fn_0066f4e0(0x00de1f64, 0x009aef20, buffer);
first_append:
        receiver = output_argument;
        cm_fn_004ae660(receiver, 0x00de1f64);
        third = D(l + 4);
        offset = person * 110u;
        base = D(0x00ae23bc);
        first = D(l + 0xc);
        second = D(l + 8);
        eax = cm_fn_0053e990(base + offset, first, second, third, 0);
        if (eax != 0) {
            base = D(0x00ae23bc);
            eax = cm_fn_004ae8a0(receiver, buffer, 0x7da, base + offset);
        }
        if (old_club != 0xffffffffu) {
            base = D(0x00ae23b4);
            club = base + old_club * 581u;
            eax = cm_fn_004ae8a0(receiver, club + 0x38, 0x7d5, club);
        }
        return eax;
    }

    kind = B(l + 3);
    if (kind == 5) {
        cm_fn_0066f4e0(0x00de1f64, 0x009aeec8, buffer);
    } else if (kind == 6) {
        base = D(0x00ae23b4);
        club = base + new_club * 581u;
        gender = (s32)(s8)B(club + 0x52);
        cm_fn_0066f4e0(0x00de1f64, 0x009aee54,
                       gender, gender, buffer, club + 0x38);
    } else if (kind == 1) {
        cm_fn_0066f4e0(0x00de1f64, 0x009aedc8, 100, buffer);
    } else if (selected != 0) {
        value = D(l + 0x10);
        if (value != 0) {
            cm_fn_0066f4e0(0x00de1f64, 0x009aed5c, 100, buffer);
        } else {
            base = D(0x00ae23b4);
            club = base + old_club * 581u;
            gender = (s32)(s8)B(club + 0x52);
            cm_fn_0066f4e0(0x00de1f64, 0x009aeccc,
                           gender, gender, 100, club + 0x38, buffer);
        }
    } else {
        cm_fn_0066f4e0(0x00de1f64, 0x009aec58, 100, buffer);
    }
    receiver = output_argument;
    cm_fn_004ae660(receiver, 0x00de1f64);
    third = D(l + 4);
    offset = person * 110u;
    base = D(0x00ae23bc);
    first = D(l + 0xc);
    second = D(l + 8);
    eax = cm_fn_0053e990(base + offset, first, second, third, 0);
    if (eax != 0) {
        base = D(0x00ae23bc);
        value = base + offset;
        receiver = output_argument;
        eax = cm_fn_004ae8a0(receiver, buffer, 0x7da, value);
    } else {
        receiver = output_argument;
    }
    if (new_club != 0xffffffffu) {
        base = D(0x00ae23b4);
        club = base + new_club * 581u;
        eax = cm_fn_004ae8a0(receiver, club + 0x38, 0x7d5, club);
    }
    if (old_club != 0xffffffffu) {
        base = D(0x00ae23b4);
        club = base + old_club * 581u;
        eax = cm_fn_004ae8a0(receiver, club + 0x38, 0x7d5, club);
    }
    return eax;
}
