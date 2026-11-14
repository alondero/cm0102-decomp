/* Preferred-image VA 0x008bfa30, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3 RET12. Keep the incidental full EAX from the final search/notification. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_004cca70(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004ccb30(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_00843e10(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00849ce0(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0084aeb0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008c6220(u32,u32,s8,u8);
extern void CM_THISCALL cm_fn_008d5170(u32,u32,s16);

u32 CM_THISCALL cm_fn_008bfa30(u32 self, u32 offer_cell, u32 mark,
                              u32 reason)
{
    u32 offer = D(offer_cell);
    u32 index, manager, base, person, club, mapped, owner;
    u32 header, link, payload, receiver, table, related, flag_address;
    u32 value, selector, result;
    u8 status, flag;

    if (mark) B(offer + 0x2du) = 1;
    index = D(offer + 0xcu);
    B(offer + 0x2cu) = 2;
    manager = D(0xae1050u);
    base = D(0xae23bcu);
    person = base + index * 110u;
    mapped = cm_fn_004cf5a0(manager, person);
    base = D(0xae23b4u);
    D(0xae1080u) = mapped;
    if (mapped) {
        index = D(mapped + 4u);
        owner = base + index * 581u;
    } else {
        owner = 0;
    }
    index = D(offer + 8u);
    club = base + index * 581u;
    receiver = D(self + 0x856u);
    cm_fn_0084aeb0(receiver, club, person);

    header = D(offer + 4u);
    if (header) {
        link = D(header + 2u);
        D(header + 0xau) = link;
        receiver = D(offer + 4u);
        payload = cm_fn_0068d1b0(receiver);
        while (payload) {
            index = D(payload);
            base = D(0xae23bcu);
            person = base + index * 110u;
            receiver = D(self + 0x856u);
            cm_fn_0084aeb0(receiver, owner, person);
            receiver = D(offer + 4u);
            payload = cm_fn_0068d1b0(receiver);
        }
    }

    cm_fn_008d5170(self, offer, (s16)reason);
    cm_fn_008c6220(self, offer, 10, 0);
    status = B(offer + 0x2cu);
    if (status == 2 || status == 3 || status == 0x14 || status == 0xf ||
        status == 0x10 || status == 4) {
        if (status == 0xf && B(offer + 0x2eu) < 3) goto lists_done;
        header = D(offer);
        if (header) {
            link = D(header + 2u);
            D(header + 0xau) = link;
            receiver = D(offer);
            payload = cm_fn_0068d1b0(receiver);
            while (payload) {
                index = D(payload);
                table = D(self);
                related = D(table + index * 4u);
                status = B(related + 0x2cu);
                if (!(status == 2 || status == 3 || status == 0x14 ||
                      status == 0xf || status == 0x10 || status == 4))
                    goto lists_done;
                if (status == 0xf && B(related + 0x2eu) < 3)
                    goto lists_done;
                receiver = D(offer);
                payload = cm_fn_0068d1b0(receiver);
            }
        }
        index = D(offer + 0xcu);
        table = D(self + 0x86eu);
        flag_address = table + index * 8u;
        flag = B(flag_address);
        if (flag & 1u) B(flag_address) = flag & 0xfeu;
    }

lists_done:
    index = D(offer + 8u);
    base = D(0xae23b4u);
    club = base + index * 581u;
    index = D(offer + 0xcu);
    base = D(0xae23bcu);
    person = base + index * 110u;
    receiver = D(0xae1050u);
    cm_fn_004cca70(receiver, person, club);
    header = D(offer + 4u);
    B(offer + 0x10u) = 0;
    if (header) {
        link = D(header + 2u);
        D(header + 0xau) = link;
        receiver = D(offer + 4u);
        payload = cm_fn_0068d1b0(receiver);
        while (payload) {
            value = D(offer + 0x20u);
            index = D(payload);
            base = D(0xae23bcu);
            person = base + index * 110u;
            receiver = D(0xae1050u);
            cm_fn_004ccb30(receiver, person, value);
            B(payload + 4u) = 0;
            receiver = D(offer + 4u);
            payload = cm_fn_0068d1b0(receiver);
        }
    }

    index = D(offer + 0xcu);
    status = B(offer + 0x11u);
    selector = status != 0xbu;
    base = D(0xae23bcu);
    person = base + index * 110u;
    index = D(offer + 8u);
    base = D(0xae23b4u);
    club = base + index * 581u;
    receiver = D(self + 0x856u);
    result = cm_fn_00849ce0(receiver, club, person, selector, 0);
    if (result) {
        index = D(offer + 8u);
        base = D(0xae23b4u);
        value = D(offer + 0xcu);
        club = base + index * 581u;
        base = D(0xae23bcu);
        person = base + value * 110u;
        receiver = D(self + 0x856u);
        result = cm_fn_00843e10(receiver, person, club, 1, 0);
    }
    return result;
}
