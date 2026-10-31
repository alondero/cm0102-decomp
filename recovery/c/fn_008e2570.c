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

/* Preferred-image VA008e2570/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL3/RET12/full EAX. Scratch140/name300L14. Ten original
   format/arity combinations; unsupported kinds retain the old global text.
   Fixed getter/name initialization proofs use original helpers, not C composition. */
u32 CM_STDCALL cm_fn_008e2570(volatile u32 event, volatile u32 mode,
                             volatile u32 text_object)
{
    u32 scratch[0x140 / 4];
    u32 l = (u32)scratch, name = l + 0x14;
    u32 selected_mode, object, person, first, second, third, value;
    u32 club_a, club_b, offset_a, offset_b, base, different, index, kind;
    u32 gender_a, gender_b, nation, nation_name, gender_n, format;
    u32 receiver, result, person_offset, record_a, record_b;
    selected_mode = mode;
    object = event;
    person = cm_fn_00785830(object, 0);
    if (selected_mode == 0) {
        first = cm_fn_00785830(object, 1);
        second = cm_fn_00785830(object, 2);
        third = cm_fn_00785830(object, 3);
        cm_fn_0053e140(first, second, third, name, 300, 5, person);
        cm_fn_0066f4e0(0x00de1f64, 0x00a9c790, name);
        receiver = text_object;
        cm_fn_004ae660(receiver, 0x00de1f64);
        base = D(0x00ae23bc);
        person_offset = person * 0x6eu;
        result = cm_fn_0053e990(person_offset + base, first, second, third, 0);
        if (result != 0) {
            base = D(0x00ae23bc);
            receiver = text_object;
            result = cm_fn_004ae8a0(receiver, name, 0x7d7, person_offset + base);
        }
        return result;
    }
    value = cm_fn_00785830(object, 1);
    D(l + 8) = value;
    value = cm_fn_00785830(object, 2);
    D(l + 4) = value;
    value = cm_fn_00785830(object, 3);
    D(l + 0xc) = value;
    club_a = cm_fn_00785830(object, 4);
    club_b = cm_fn_00785830(object, 5);
    value = cm_fn_00785830(object, 6);
    B(l + 3) = (u8)value;
    value = cm_fn_00785830(object, 7);
    D(l + 0x10) = value;
    base = D(0x00ae23b4);
    offset_b = club_b * 0x245u;
    value = D(offset_b + base + 0xcf);
    different = 1;
    if (value != 0) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        if (value == D(0x00b74c26 + index * 0x300u)) different = 0;
    }
    third = D(l + 0xc);
    second = D(l + 4);
    first = D(l + 8);
    cm_fn_0053e140(first, second, third, name, 300, 1, person);
    kind = B(l + 3);
    offset_a = club_a * 0x245u;
    if (kind == 4 || kind == 9) {
        if (different != 0) {
            base = D(0x00ae23b4);
            gender_b = (u32)(s32)(s8)B(offset_b + base + 0x52);
            record_a = offset_a + base;
            gender_a = (u32)(s32)(s8)B(record_a + 0x52);
            if (kind == 4)
                cm_fn_0066f4e0(0x00de1f64, 0x00a9cdc4, gender_b, gender_b, 100,
                               gender_a, gender_a, gender_a, gender_a,
                               offset_b + base + 4, name, record_a + 0x38, record_a + 0x38);
            else
                cm_fn_0066f4e0(0x00de1f64, 0x00a9cc44, gender_b, gender_b, 100,
                               gender_a, gender_a, offset_b + base + 4, name, record_a + 0x38);
        } else {
            base = D(0x00ae23b4);
            record_a = offset_a + base;
            gender_a = (u32)(s32)(s8)B(record_a + 0x52);
            if (kind == 4)
                cm_fn_0066f4e0(0x00de1f64, 0x00a9cd04, 100,
                               gender_a, gender_a, gender_a, gender_a,
                               name, record_a + 4, record_a + 4);
            else
                cm_fn_0066f4e0(0x00de1f64, 0x00a9cbac, 100,
                               gender_a, gender_a, name, record_a + 4);
        }
    } else if (kind == 0x26 || kind == 0x1e || kind == 0x25) {
        base = D(0x00ae23b4);
        record_a = offset_a + base;
        gender_a = (u32)(s32)(s8)B(record_a + 0x52);
        format = kind == 0x26 ? 0x00a9cae0 : kind == 0x1e ? 0x00a9ca18 : 0x00a9c958;
        cm_fn_0066f4e0(0x00de1f64, format, 100,
                       gender_a, gender_a, gender_a, gender_a, name, record_a + 4, record_a + 4);
    } else if (kind == 0x1d) {
        index = D(l + 0x10);
        base = D(0x00ae23d0);
        nation = base + index * 0x6bu;
        base = D(0x00ae23b4);
        nation_name = nation + 4;
        gender_n = (u32)(s32)(s8)B(nation + 0x37);
        record_a = offset_a + base;
        gender_a = (u32)(s32)(s8)B(record_a + 0x52);
        cm_fn_0066f4e0(0x00de1f64, 0x00a9c888, 100, gender_a, gender_a,
                       gender_n, name, record_a + 4, nation_name);
    } else if (kind == 0x1f) {
        base = D(0x00ae23b4);
        record_a = offset_a + base;
        gender_a = (u32)(s32)(s8)B(record_a + 0x52);
        gender_b = (u32)(s32)(s8)B(offset_b + base + 0x52);
        cm_fn_0066f4e0(0x00de1f64, 0x00a9c7c8, 100, gender_a, gender_a,
                       gender_b, gender_b, name, record_a + 4, offset_b + base + 4);
    }
    receiver = text_object;
    cm_fn_004ae660(receiver, 0x00de1f64);
    base = D(0x00ae23b4);
    record_a = offset_a + base;
    cm_fn_004ae8a0(receiver, record_a + 4, 0x7d5, record_a);
    base = D(0x00ae23b4);
    record_b = offset_b + base;
    cm_fn_004ae8a0(receiver, record_b + 4, 0x7d5, record_b);
    second = D(l + 4);
    first = D(l + 8);
    third = D(l + 0xc);
    base = D(0x00ae23bc);
    person_offset = person * 0x6eu;
    result = cm_fn_0053e990(person_offset + base, first, second, third, 0);
    if (result != 0) {
        base = D(0x00ae23bc);
        result = cm_fn_004ae8a0(receiver, name, 0x7d7, person_offset + base);
    }
    return result;
}
