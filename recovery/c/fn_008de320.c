#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_0043f460(u32, u32);
extern u32 CM_CDECL cm_fn_00441c10(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);

/* Preferred-image VA008de320/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL3/RET12/full EAX. SEH9645fb/scratch2e4. NameB300L28,
   money100L154, nameA300L1b8; four-byte money objectL24. Helpers
   43f460(THIS1/RET4/EDX:EAX) and441c10(CDECL5) stay original code.
   All five formats and pending argument groups checked independently. */
u32 CM_STDCALL cm_fn_008de320(volatile u32 event, volatile u32 mode,
                             volatile u32 text_object)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[0x2e4 / 4];
    u32 l = (u32)scratch, name_b = l + 0x28, money = l + 0x154, name_a = l + 0x1b8;
    u32 object, value, person_b, first_b, second_b, third_b;
    u32 club_a, club_b, person_a, first, second, third, category;
    u32 amount, comparison, offset_a, offset_b, base, gender_a, gender_b;
    u32 format, receiver, offset, result;
    unsigned long long converted;
    frame.state = 0xffffffffu;
    frame.handler = 0x009645fb;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    object = event;
    person_b = cm_fn_00785830(object, 6);
    D(l + 4) = person_b;
    first_b = cm_fn_00785830(object, 7);
    D(l + 0x10) = first_b;
    second_b = cm_fn_00785830(object, 8);
    D(l + 8) = second_b;
    value = cm_fn_00785830(object, 9);
    D(l) = value;
    if (mode == 0) {
        cm_fn_0053e140(first_b, second_b, value, name_b, 300, 5, person_b);
        cm_fn_0066f4e0(0x00de1f64, 0x00a94fc0, name_b);
        receiver = text_object;
        cm_fn_004ae660(receiver, 0x00de1f64);
        third_b = D(l);
        base = D(0x00ae23bc);
        offset = person_b * 0x6eu;
        result = cm_fn_0053e990(offset + base, first_b, second_b, third_b, 0);
        if (result != 0) {
            base = D(0x00ae23bc);
            receiver = text_object;
            result = cm_fn_004ae8a0(receiver, name_b, 0x7d7, offset + base);
        }
        goto done;
    }
    club_a = cm_fn_00785830(object, 0);
    club_b = cm_fn_00785830(object, 1);
    person_a = cm_fn_00785830(object, 2);
    value = cm_fn_00785830(object, 3);
    D(l + 0x20) = value;
    value = cm_fn_00785830(object, 4);
    D(l + 0x14) = value;
    value = cm_fn_00785830(object, 5);
    D(l + 0x18) = value;
    value = cm_fn_00785830(object, 10);
    D(l + 0x1c) = value;
    value = cm_fn_00785830(object, 11);
    D(l + 0xc) = value;
    category = cm_fn_00785830(object, 12);
    third = D(l + 0x18);
    second = D(l + 0x14);
    first = D(l + 0x20);
    cm_fn_0053e140(first, second, third, name_a, 300, 1, person_a);
    person_b = D(l + 4);
    third_b = D(l);
    second_b = D(l + 8);
    first_b = D(l + 0x10);
    cm_fn_0053e140(first_b, second_b, third_b, name_b, 300, 1, person_b);
    amount = D(l + 0x1c);
    comparison = D(l + 0xc);
    if ((s32)amount > (s32)comparison) {
        D(l + 0x24) = amount;
        frame.state = 0;
        converted = cm_fn_0043f460(l + 0x24, 6);
        cm_fn_00441c10(money, 100, (u32)converted, (u32)(converted >> 32), 6);
        amount = D(l + 0x1c);
        comparison = D(l + 0xc);
        frame.state = 0xffffffffu;
    }
    if ((u16)category == 22)
        format = (s32)amount > (s32)comparison ? 0x00a997fc : 0x00a9971c;
    else
        format = (s32)amount > (s32)comparison ? 0x00a99608 : 0x00a99540;
    base = D(0x00ae23b4);
    offset_b = club_b * 0x245u;
    gender_b = (u32)(s32)(s8)B(offset_b + base + 0x52);
    offset_a = club_a * 0x245u;
    gender_a = (u32)(s32)(s8)B(offset_a + base + 0x52);
    if ((s32)amount > (s32)comparison)
        cm_fn_0066f4e0(0x00de1f64, format, gender_a, gender_a, gender_b, gender_b,
                       offset_a + base + 0x38, name_b, offset_b + base + 0x38, name_a, money);
    else
        cm_fn_0066f4e0(0x00de1f64, format, gender_a, gender_a, gender_b, gender_b,
                       offset_a + base + 0x38, name_b, offset_b + base + 0x38, name_a);
    receiver = text_object;
    cm_fn_004ae660(receiver, 0x00de1f64);
    base = D(0x00ae23b4);
    value = offset_a + base;
    cm_fn_004ae8a0(receiver, value + 0x38, 0x7d5, value);
    base = D(0x00ae23b4);
    value = offset_b + base;
    cm_fn_004ae8a0(receiver, value + 0x38, 0x7d5, value);
    third = D(l + 0x18);
    second = D(l + 0x14);
    first = D(l + 0x20);
    base = D(0x00ae23bc);
    offset = person_a * 0x6eu;
    result = cm_fn_0053e990(offset + base, first, second, third, 0);
    if (result != 0) {
        base = D(0x00ae23bc);
        cm_fn_004ae8a0(receiver, name_a, 0x7d7, offset + base);
    }
    person_b = D(l + 4);
    second_b = D(l + 8);
    third_b = D(l);
    first_b = D(l + 0x10);
    base = D(0x00ae23bc);
    offset = person_b * 0x6eu;
    result = cm_fn_0053e990(offset + base, first_b, second_b, third_b, 0);
    if (result != 0) {
        base = D(0x00ae23bc);
        result = cm_fn_004ae8a0(receiver, name_b, 0x7d7, offset + base);
    }
done:
    u32 predecessor = frame.prev;
    CM_FS(u32, 0) = predecessor;
    return result;
}
