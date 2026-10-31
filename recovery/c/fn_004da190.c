#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
typedef u16 word_4da190 __attribute__((may_alias));
#define W(a) CM_GLOBAL(volatile word_4da190, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_THISCALL cm_fn_00549ef0(u32, u32, u32);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_00786d30(u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);

/* Preferred-image VA004da190/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12/full incidental EAX. Scratch150: flagBYTE L3,
   namekeysL4/L8/Lc, kindL10, personIDL14, savedselfL18,
   dateL1c, name300B L24. Original club annotation uses club+4.
   The zero-mode branch caches two name keys before callbacks. */
u32 CM_THISCALL cm_fn_004da190(u32 self, u32 event,
                              volatile u32 mode, volatile u32 output)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[0x150 / 4];
    u32 l = (u32)scratch, buffer = l + 0x24, previous, club, person, value;
    u32 year, day, expected, base, pointer, index, count, list, saved_self;
    u32 first, second, third, receiver, format, person_offset, eax;
    u8 check_flag, flag;
    int notify;

    frame.state = 0xffffffffu;
    frame.handler = 0x00956fcb;
    previous = CM_FS(u32, 0);
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    D(l + 0x18) = self;
    club = cm_fn_00785830(event, 0);
    person = cm_fn_00785830(event, 2);
    value = cm_fn_00785830(event, 3);
    D(l + 0xc) = value;
    value = cm_fn_00785830(event, 4);
    D(l + 8) = value;
    value = cm_fn_00785830(event, 5);
    D(l + 4) = value;
    value = cm_fn_00785830(event, 6);
    D(l + 0x10) = value;
    value = cm_fn_00785830(event, 7);
    check_flag = (u8)value;
    value = cm_fn_00785830(event, 11);
    B(l + 3) = (u8)value;
    value = mode;
    if (value != 0) {
        if (check_flag == 0) {
            year = cm_fn_00785830(event, 9);
            day = cm_fn_00785830(event, 8);
            cm_fn_00549ef0(l + 0x1c, day, year);
            frame.state = 0;
            expected = cm_fn_00785830(event, 10);
            base = D(0x00ae23bc);
            pointer = base + person * 110u;
            notify = 1;
            if (pointer != 0) {
                index = D(pointer);
                count = D(0x00ae2364);
                D(l + 0x14) = index;
                if ((s32)index < (s32)count) {
                    base = D(0x00ae2d0c);
                    index = D(base + index * 79u);
                    if ((s32)index >= 0) {
                        base = D(0x00ae1050);
                        pointer = D(base) + index * 80u;
                        if (pointer != 0) {
                            value = D(pointer + 0xc);
                            if (value == expected) {
                                value = W(l + 0x1e);
                                year = W(pointer + 0x2f);
                                if (value == year) {
                                    value = W(l + 0x1c);
                                    day = W(pointer + 0x2d);
                                    if (value == day) {
                                        notify = 0;
                                        saved_self = D(l + 0x18);
                                        list = D(saved_self + 0x34);
                                        if (list != 0) {
                                            value = D(list + 2);
                                            D(list + 0xa) = value;
                                            list = D(saved_self + 0x34);
                                            pointer = cm_fn_0068d1b0(list);
                                            while (pointer != 0) {
                                                index = D(l + 0x14);
                                                value = D(pointer);
                                                if (value == index) {
                                                    value = D(pointer + 4);
                                                    if (value == club) {
                                                        notify = 1;
                                                        break;
                                                    }
                                                }
                                                list = D(saved_self + 0x34);
                                                pointer = cm_fn_0068d1b0(list);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (notify) {
                index = (u32)(s32)(s16)W(0x00b77c7a);
                value = D(event + 0xd6);
                pointer = D(0x00b74c26 + index * 768u);
                receiver = D(0x00b7431c);
                cm_fn_00786d30(receiver, pointer, value, 7, 1, event);
            }
            frame.state = 0xffffffffu;
        }
        third = D(l + 4);
        second = D(l + 8);
        first = D(l + 0xc);
        cm_fn_0053e140(first, second, third, buffer, 300, 1, person);
        flag = B(l + 3);
        value = D(l + 0x10);
        if (flag != 0) {
            if (value == 1) format = 0x009ae934;
            else if (value == 2) format = 0x009aea00;
            else if (value == 3) format = 0x009aead0;
            else format = 0x009aeba0;
            cm_fn_0066f4e0(0x00de1f64, format, 100, buffer);
        } else {
            if (value == 1) format = 0x009ae690;
            else if (value == 2) format = 0x009ae740;
            else if (value == 3) format = 0x009ae7f4;
            else format = 0x009ae8a8;
            cm_fn_0066f4e0(0x00de1f64, format, buffer);
        }
        receiver = output;
        cm_fn_004ae660(receiver, 0x00de1f64);
        base = D(0x00ae23b4);
        pointer = base + club * 581u;
        cm_fn_004ae8a0(receiver, pointer + 4, 0x7d5, pointer);
        second = D(l + 8);
        first = D(l + 0xc);
        third = D(l + 4);
        base = D(0x00ae23bc);
        person_offset = person * 110u;
        eax = cm_fn_0053e990(base + person_offset, first, second, third, 0);
        if (eax != 0) {
            base = D(0x00ae23bc);
            eax = cm_fn_004ae8a0(receiver, buffer, 0x7d7, base + person_offset);
        }
    } else {
        third = D(l + 4);
        second = D(l + 8);
        first = D(l + 0xc);
        cm_fn_0053e140(first, second, third, buffer, 300, 5, person);
        cm_fn_0066f4e0(0x00de1f64, 0x009ae660, buffer);
        receiver = output;
        cm_fn_004ae660(receiver, 0x00de1f64);
        third = D(l + 4);
        person_offset = person * 110u;
        base = D(0x00ae23bc);
        eax = cm_fn_0053e990(base + person_offset, first, second, third, 0);
        if (eax != 0) {
            base = D(0x00ae23bc);
            receiver = output;
            eax = cm_fn_004ae8a0(receiver, buffer, 0x7d7, base + person_offset);
        }
    }
    previous = frame.prev;
    CM_FS(u32, 0) = previous;
    return eax;
}
