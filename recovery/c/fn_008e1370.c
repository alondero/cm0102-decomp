#include "cmre.h"

/* Preferred-image VA 0x008e1370, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Ret 0xc; the sole direct caller overwrites EAX. The two buffers retain
   their original relative positions within the 0x1a4-byte scratch area. */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern u32 CM_THISCALL cm_fn_00786d30(u32, u32, u32, u32, s8, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_006181d0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define S(a) (*(volatile s8 *)(u32)(a))

void CM_STDCALL cm_fn_008e1370(u32 source, u32 details, u32 output)
{
    u8 scratch[0x1a4];
    u32 person, first, second, third, club, other = 0, nation;
    u32 selected, value, object, count, table, base, target, receiver;
    u32 club_offset, other_offset, person_offset, person_record;
    u8 status;
    s32 nation_index, club_style, other_style;
    u32 names = (u32)scratch + 0x78u;
    u32 country = (u32)scratch + 0x14u;

    person = (u32)cm_fn_00785830((void *)source, 0);
    first = (u32)cm_fn_00785830((void *)source, 1);
    second = (u32)cm_fn_00785830((void *)source, 2);
    third = (u32)cm_fn_00785830((void *)source, 3);
    club = (u32)cm_fn_00785830((void *)source, 4);
    status = (u8)cm_fn_00785830((void *)source, 10);
    if (status == 1) {
        selected = (u32)cm_fn_00785830((void *)source, 11);
        value = (u32)cm_fn_00785830((void *)source, 12);
        object = D(0x00de1f54u);
        count = D(object + 4u);
        table = D(object);
        if ((s32)selected < (s32)count) table += selected * 4u;
        object = D(table);
        if (object == 0) {
            status = 4;
        } else if (D(object + 0x24u) == value) {
            status = B(object + 0x2cu);
            if (status == 1) goto format;
        } else if (person == D(object + 12u) && club == D(object + 8u)) {
            status = 0;
        } else {
            status = 4;
        }
        target = D(source + 0xd6u);
        nation_index = (s16)W(0x00b77c7au);
        value = D(0x00b74c26u + (u32)nation_index * 768u);
        receiver = D(0x00b7431cu);
        cm_fn_00786d30(receiver, value, target, 10, (s8)status, 0);
    }
format:
    if (details != 0) {
        other = (u32)cm_fn_00785830((void *)source, 5);
        nation = (u32)cm_fn_00785830((void *)source, 9);
        cm_fn_0053e140(first, second, third, names, 300, 1, person);
        base = D(0x00ae23a8u);
        cm_fn_006181d0(base + nation * 290u, country, 100, 10);
        base = D(0x00ae23b4u);
        club_offset = club * 581u;
        if (other != 0xffffffffu) {
            other_offset = other * 581u;
            other_style = S(base + other_offset + 0x52u);
            club_style = S(base + club_offset + 0x52u);
            cm_fn_0066f4e0(0x00de1f64u, 0x00a9c2e0u, 100,
                          other_style, other_style, club_style, club_style,
                          100, 100, names, base + other_offset + 0x38u,
                          base + club_offset + 0x38u, country);
        } else {
            club_style = S(base + club_offset + 0x52u);
            cm_fn_0066f4e0(0x00de1f64u, 0x00a9c218u, 100,
                          club_style, club_style, 100, 100, names,
                          base + club_offset + 0x38u, country);
        }
        cm_fn_004ae660((void *)output, 0x00de1f64u);
        if (status == 0 || status == 3) {
            base = D(0x00ae23b4u);
            club_style = S(base + club_offset + 0x52u);
            cm_fn_0066f4e0(0x00de1f64u, 0x00a9c1dcu,
                          club_style, club_style, base + club_offset + 0x38u);
        }
    } else {
        cm_fn_0053e140(first, second, third, names, 300, 5, person);
        base = D(0x00ae23b4u);
        club_offset = club * 581u;
        club_style = S(base + club_offset + 0x52u);
        cm_fn_0066f4e0(0x00de1f64u, 0x00a9c180u, 100,
                      club_style, club_style, names, base + club_offset + 0x38u);
        cm_fn_004ae660((void *)output, 0x00de1f64u);
    }
    base = D(0x00ae23bcu);
    person_offset = person * 110u;
    value = cm_fn_0053e990(base + person_offset, first, second, third, 0);
    if (value != 0) {
        base = D(0x00ae23bcu);
        person_record = base + person_offset;
        cm_fn_004ae8a0(output, names, 0x7d7, person_record);
    }
    base = D(0x00ae23b4u);
    target = club_offset + base;
    cm_fn_004ae8a0(output, target + 0x38u, 0x7d5, target);
    if (details != 0 && other != 0xffffffffu) {
        base = D(0x00ae23b4u);
        target = base + other * 581u;
        cm_fn_004ae8a0(output, target + 0x38u, 0x7d5, target);
    }
}
