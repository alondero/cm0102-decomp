#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern unsigned long long CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);

/* Preferred-image VA0041a8e0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL3/RET12; full EAX retained although the indexed caller overwrites
   it. Scratch33C: localsL0..18, adjacent 400-byte buffersL1C/L1AC.
   Getter6 in the nonzero branch is called even though its result is unused.
   Every original callee remains an explicit boundary. */
u32 CM_STDCALL cm_fn_0041a8e0(u32 news, u32 mode, volatile u32 result_arg)
{
    u32 scratch[0x33c / 4];
    u32 l = (u32)scratch, first_person, second_person, club, last;
    u32 first, second, third, receiver, p, base, offset, format, kind, result;
    s32 gender;
    if (mode == 0) {
        first_person = (u32)cm_fn_00785830(news, 0);
        first = (u32)cm_fn_00785830(news, 1);
        second = (u32)cm_fn_00785830(news, 2);
        third = (u32)cm_fn_00785830(news, 3);
        D(l) = third;
        D(l + 4) = (u32)cm_fn_00785830(news, 5);
        last = (u32)cm_fn_00785830(news, 6);
        third = D(l);
        cm_fn_0053e140(first, second, third, l + 0x1c, 400, 5, first_person);
        if (last != 0)
            format = 0x00988e98;
        else if (D(l + 4) == 0)
            format = 0x00988e54;
        else
            format = 0x00988e20;
        cm_fn_0066f4e0(0x00de1f64, format, l + 0x1c);
        receiver = result_arg;
        cm_fn_004ae660(receiver, 0x00de1f64);
        offset = first_person * 110u;
        third = D(l);
        base = D(0x00ae23bc);
        result = cm_fn_0053e990(base + offset, first, second, third, 0);
        if (result != 0) {
            base = D(0x00ae23bc);
            receiver = result_arg;
            result = cm_fn_004ae8a0(receiver, l + 0x1c, 0x7da, base + offset);
        }
        return result;
    }

    first_person = (u32)cm_fn_00785830(news, 0);
    D(l + 0x18) = (u32)cm_fn_00785830(news, 1);
    D(l + 0x10) = (u32)cm_fn_00785830(news, 2);
    D(l + 0xc) = (u32)cm_fn_00785830(news, 3);
    club = (u32)cm_fn_00785830(news, 4);
    D(l + 0x14) = (u32)cm_fn_00785830(news, 5);
    cm_fn_00785830(news, 6);
    second_person = (u32)cm_fn_00785830(news, 7);
    D(l) = (u32)cm_fn_00785830(news, 8);
    D(l + 8) = (u32)cm_fn_00785830(news, 9);
    last = (u32)cm_fn_00785830(news, 10);
    third = D(l + 0xc);
    second = D(l + 0x10);
    first = D(l + 0x18);
    D(l + 4) = last;
    cm_fn_0053e140(first, second, third, l + 0x1c, 400, 1, first_person);
    if (second_person != 0xffffffffu) {
        second = D(l + 8);
        first = D(l);
        cm_fn_0053e140(first, second, last, l + 0x1ac, 400, 1, second_person);
    } else {
        B(l + 0x1ac) = 0;
    }
    offset = (u32)(s32)(s16)W(0x00b77c7a) * 0x300u;
    p = D(0x00b74c26 + offset);
    if (p != 0) {
        p = D(p + 0x39);
        if (p != 0) {
            p = D(p);
            if (club == p) {
                kind = D(l + 0x14);
                if (kind == 0)
                    format = second_person == 0xffffffffu ? 0x00989250 : 0x009891d0;
                else if (kind == 1)
                    format = second_person == 0xffffffffu ? 0x0098933c : 0x009892c0;
                else
                    format = second_person == 0xffffffffu ? 0x0098942c : 0x009893a8;
                if (second_person == 0xffffffffu)
                    cm_fn_0066f4e0(0x00de1f64, format, l + 0x1c);
                else
                    cm_fn_0066f4e0(0x00de1f64, format, l + 0x1ac, l + 0x1c);
                goto formatted;
            }
        }
    }
    if (second_person == 0xffffffffu) {
        base = D(0x00ae23b4);
        p = base + club * 0x245u;
        gender = (s32)(s8)B(p + 0x52);
        cm_fn_0066f4e0(0x00de1f64, 0x00988ed0, gender, gender, 100, p + 0x38, l + 0x1c);
    } else {
        kind = D(l + 0x14);
        base = D(0x00ae23b4);
        p = base + club * 0x245u;
        gender = (s32)(s8)B(p + 0x52);
        if (kind == 0)
            cm_fn_0066f4e0(0x00de1f64, 0x00988f6c, gender, gender, 100, p + 0x38, l + 0x1ac, l + 0x1c);
        else
            cm_fn_0066f4e0(0x00de1f64, kind == 1 ? 0x00989030 : 0x00989100,
                gender, gender, p + 0x38, l + 0x1ac, l + 0x1c);
    }
formatted:
    receiver = result_arg;
    cm_fn_004ae660(receiver, 0x00de1f64);
    third = D(l + 0xc);
    second = D(l + 0x10);
    first = D(l + 0x18);
    base = D(0x00ae23bc);
    offset = first_person * 110u;
    result = cm_fn_0053e990(base + offset, first, second, third, 0);
    if (result != 0) {
        base = D(0x00ae23bc);
        receiver = result_arg;
        cm_fn_004ae8a0(receiver, l + 0x1c, 0x7da, base + offset);
    } else {
        receiver = result_arg;
    }
    if (second_person != 0xffffffffu) {
        third = D(l + 4);
        second = D(l + 8);
        first = D(l);
        base = D(0x00ae23bc);
        offset = second_person * 110u;
        result = cm_fn_0053e990(base + offset, first, second, third, 0);
        if (result != 0) {
            base = D(0x00ae23bc);
            cm_fn_004ae8a0(receiver, l + 0x1ac, 0x7da, base + offset);
        }
    }
    base = D(0x00ae23b4);
    p = base + club * 0x245u;
    return cm_fn_004ae8a0(receiver, p + 4, 0x7d5, p);
}
