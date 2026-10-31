#include "cmre.h"
/* Preferred-image VA006a0ad0/base00400000, pinned 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD3/RET12/full incidental EAX. Scratch19c: fieldsL0/L4/L8,
   textLc (400 bytes). DE1F64 is the original output buffer address. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern unsigned long long CM_THISCALL cm_fn_00785830(u32,u32);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32,u32);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32,u32,u32,u32);

u32 CM_STDCALL cm_fn_006a0ad0(volatile u32 input_arg, volatile u32 kind_arg,
                             volatile u32 output_arg)
{
    u8 scratch[0x19c];
    u32 l = (u32)scratch, kind, input, person, first, second, third;
    u32 flags, club, club_offset, base, text, format, output, offset, eax;
    s32 initial;
    kind = kind_arg;
    if (!kind) {
        input = input_arg;
        person = (u32)cm_fn_00785830(input, 0);
        first = (u32)cm_fn_00785830(input, 1);
        second = (u32)cm_fn_00785830(input, 2);
        eax = (u32)cm_fn_00785830(input, 3);
        D(l) = eax;
        eax = (u32)cm_fn_00785830(input, 5);
        third = D(l);
        D(l + 4) = eax;
        cm_fn_0053e140(first, second, third, l + 0xc, 400, 5, person);
        flags = D(l + 4);
        if (flags & 0x800u) format = 0x00a06020;
        else if (flags & 0x400u) format = 0x00a05fe0;
        else if (flags & 1u) format = 0x00a05fa4;
        else format = 0x00a05f64;
        cm_fn_0066f4e0(0x00de1f64, format, 100, l + 0xc);
        output = output_arg;
        cm_fn_004ae660(output, 0x00de1f64);
        base = D(0x00ae23bc);
        offset = person * 110u;
        third = D(l);
        eax = cm_fn_0053e990(base + offset, first, second, third, 0);
        if (eax) {
            base = D(0x00ae23bc);
            output = output_arg;
            eax = cm_fn_004ae8a0(output, l + 0xc, 0x7da, base + offset);
        }
        return eax;
    }
    input = input_arg;
    person = (u32)cm_fn_00785830(input, 0);
    eax = (u32)cm_fn_00785830(input, 1);
    D(l + 4) = eax;
    eax = (u32)cm_fn_00785830(input, 2);
    D(l + 8) = eax;
    eax = (u32)cm_fn_00785830(input, 3);
    D(l) = eax;
    club = (u32)cm_fn_00785830(input, 4);
    flags = (u32)cm_fn_00785830(input, 5);
    second = D(l + 8);
    third = D(l);
    first = D(l + 4);
    cm_fn_0053e140(first, second, third, l + 0xc, 400, 1, person);
    club_offset = club * 581u;
    base = D(0x00ae23b4);
    initial = (s8)B(base + club_offset + 0x37);
    text = base + club_offset + 4;
    if (flags & 0x800u) {
        format = flags & 0x200u ? 0x00a05eac : 0x00a05e00;
        cm_fn_0066f4e0(0x00de1f64, format, 100, initial, initial, text, l + 0xc);
    } else {
        if (flags & 0x400u) format = 0x00a05d18;
        else if (flags & 1u) format = 0x00a05c84;
        else if (flags & 0x200u) format = 0x00a05bb4;
        else format = 0x00a05b28;
        cm_fn_0066f4e0(0x00de1f64, format, initial, initial, text, l + 0xc);
    }
    output = output_arg;
    cm_fn_004ae660(output, 0x00de1f64);
    second = D(l + 8);
    first = D(l + 4);
    third = D(l);
    base = D(0x00ae23bc);
    offset = person * 110u;
    eax = cm_fn_0053e990(base + offset, first, second, third, 0);
    if (eax) {
        base = D(0x00ae23bc);
        cm_fn_004ae8a0(output, l + 0xc, 0x7da, base + offset);
    }
    base = D(0x00ae23b4);
    eax = cm_fn_004ae8a0(output, base + club_offset + 4, 0x7d5, base + club_offset);
    return eax;
}
