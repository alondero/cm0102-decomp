#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);

/* Sole direct caller immediately overwrites full EAX at VA008c399a. */
void CM_STDCALL cm_fn_008e4290(u32 person, u32 record)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0xe0];
    u32 local = (u32)scratch;
    u32 pointer, value, context, result, club, id, base;
    frame.state = 0xffffffffu;
    frame.handler = 0x0096486b;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0077bbe0(local, 0xbcc, 0);
    frame.state = 0;
    value = D(person);
    cm_fn_00785790(local, 0, value);
    pointer = D(person + 4);
    value = D(pointer + 0x33);
    cm_fn_00785790(local, 1, value);
    pointer = D(person + 8);
    value = D(pointer + 0x33);
    cm_fn_00785790(local, 2, value);
    pointer = D(person + 0xc);
    value = D(pointer + 0x33);
    cm_fn_00785790(local, 3, value);
    value = D(record + 4);
    cm_fn_00785790(local, 4, value);

    context = D(0x00ae1050);
    result = cm_fn_004cf5a0(context, person);
    D(0x00ae1080) = result;
    if (result != 0) {
        id = D(result + 4);
        base = D(0x00ae23b4);
        club = base + id * 581u;
        if (club != 0) {
            context = D(0x00ae1050);
            result = cm_fn_004cf5a0(context, person);
            D(0x00ae1080) = result;
            if (result != 0) {
                id = D(result + 4);
                base = D(0x00ae23b4);
                club = base + id * 581u;
            } else {
                club = 0;
            }
            context = D(0x00b7431c);
            cm_fn_007861e0(context, local, club);
        }
    }
    context = D(0x00ae1050);
    result = cm_fn_004cf5a0(context, person);
    D(0x00ae1080) = result;
    if (result != 0) {
        id = D(result + 4);
        base = D(0x00ae23b4);
        club = base + id * 581u;
        if (club != 0) {
            context = D(0x00ae1050);
            result = cm_fn_004cf5a0(context, person);
            D(0x00ae1080) = result;
            if (result != 0) {
                id = D(result + 4);
                base = D(0x00ae23b4);
                club = base + id * 581u;
            } else {
                club = 0;
            }
            /* The original dereferences this repeated lookup even if null. */
            value = D(club);
            if (value != D(record + 4)) {
                context = D(0x00ae1050);
                result = cm_fn_004cf5a0(context, person);
                D(0x00ae1080) = result;
                if (result != 0) {
                    id = D(result + 4);
                    base = D(0x00ae23b4);
                    club = base + id * 581u;
                } else {
                    club = 0;
                }
                context = D(0x00b7431c);
                cm_fn_007861e0(context, local, club);
            }
        }
    }
    CM_FS(u32, 0) = frame.prev;
}
