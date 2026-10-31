#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
extern u32 CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);

/* Preferred-image VA00587e90/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL3/full incidental EAX, not the draft's two-argument void function.
   Scratch338: two400-byte names atL18/L1a8 with saved fieldsL0..L14.
   Every fixed getter index passes the original BYTE bound. The original
   name helpers initialize the first output BYTE under the nonnull/400
   size guards. Original callees remain boundaries. */
u32 CM_CDECL cm_fn_00587e90(volatile u32 event, volatile u32 mode,
                           volatile u32 text_object)
{
    u32 scratch[0x338 / 4];
    u32 l = (u32)scratch, selected_mode = mode, news = event;
    u32 person, other, first, second, third, kind, detail;
    u32 value, format, receiver, offset, base, eax;
    if (selected_mode != 0) {
        person = cm_fn_00785830(news, 0);
        value = cm_fn_00785830(news, 1);
        D(l + 0xc) = value;
        value = cm_fn_00785830(news, 2);
        D(l + 0x14) = value;
        value = cm_fn_00785830(news, 3);
        D(l + 0x10) = value;
        other = cm_fn_00785830(news, 4);
        value = cm_fn_00785830(news, 5);
        D(l) = value;
        value = cm_fn_00785830(news, 6);
        D(l + 4) = value;
        value = cm_fn_00785830(news, 7);
        D(l + 8) = value;
        kind = cm_fn_00785830(news, 8);
        detail = cm_fn_00785830(news, 9);
        third = D(l + 0x10);
        second = D(l + 0x14);
        first = D(l + 0xc);
        cm_fn_0053e140(first, second, third, l + 0x18, 400, 1, person);
        third = D(l + 8);
        second = D(l + 4);
        first = D(l);
        cm_fn_0053e140(first, second, third, l + 0x1a8, 400, 1, other);
        if (kind == 0) {
            if (detail == 0) format = 0x009c5678;
            else if (detail == 1) format = 0x009c55f0;
            else if (detail == 2) format = 0x009c5574;
            else format = 0x009c550c;
            cm_fn_0066f4e0(0x00de1f64, format, l + 0x18);
        } else if (kind == 1) {
            cm_fn_0066f4e0(0x00de1f64, 0x009c5708, l + 0x18, l + 0x1a8);
        } else if (detail == 0) {
            cm_fn_0066f4e0(0x00de1f64, 0x009c5a3c, 100, l + 0x18, l + 0x1a8);
        } else {
            if (detail == 1) format = 0x009c59a4;
            else if (detail == 2) format = 0x009c592c;
            else if (detail == 3) format = 0x009c58a8;
            else if (detail == 4) format = 0x009c5800;
            else format = 0x009c577c;
            cm_fn_0066f4e0(0x00de1f64, format, l + 0x18, l + 0x1a8);
        }
        receiver = text_object;
        cm_fn_004ae660(receiver, 0x00de1f64);
        third = D(l + 0x10);
        second = D(l + 0x14);
        offset = person * 110u;
        first = D(l + 0xc);
        base = D(0x00ae23bc);
        eax = cm_fn_0053e990(offset + base, first, second, third, 0);
        if (eax != 0) {
            base = D(0x00ae23bc);
            cm_fn_004ae8a0(receiver, l + 0x18, 0x7da, offset + base);
        }
        third = D(l + 8);
        second = D(l + 4);
        offset = other * 110u;
        first = D(l);
        base = D(0x00ae23bc);
        eax = cm_fn_0053e990(offset + base, first, second, third, 0);
        if (eax == 0) return eax;
        base = D(0x00ae23bc);
        return cm_fn_004ae8a0(receiver, l + 0x1a8, 0x7da, offset + base);
    }
    person = cm_fn_00785830(news, 0);
    first = cm_fn_00785830(news, 1);
    second = cm_fn_00785830(news, 2);
    value = cm_fn_00785830(news, 3);
    D(l) = value;
    value = cm_fn_00785830(news, 8);
    D(l + 4) = value;
    detail = cm_fn_00785830(news, 9);
    third = D(l);
    cm_fn_0053e140(first, second, third, l + 0x18, 400, 5, person);
    kind = D(l + 4);
    if (kind == 0) {
        format = detail == 0 || detail == 1 ? 0x009c5408u : 0x009c5448u;
        cm_fn_0066f4e0(0x00de1f64, format, l + 0x18);
    } else if (kind == 1) {
        cm_fn_0066f4e0(0x00de1f64, 0x009c5480, l + 0x18);
    } else if (detail == 1 || detail == 2 || detail == 3) {
        cm_fn_0066f4e0(0x00de1f64, 0x009c54b8, l + 0x18);
    } else {
        cm_fn_0066f4e0(0x00de1f64, 0x009c54ec);
    }
    receiver = text_object;
    cm_fn_004ae660(receiver, 0x00de1f64);
    third = D(l);
    offset = person * 110u;
    base = D(0x00ae23bc);
    eax = cm_fn_0053e990(offset + base, first, second, third, 0);
    if (eax == 0) return eax;
    base = D(0x00ae23bc);
    receiver = text_object;
    return cm_fn_004ae8a0(receiver, l + 0x18, 0x7da, offset + base);
}
