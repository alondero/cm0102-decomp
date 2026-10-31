#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);

/* Sole direct caller overwrites full EAX at VA008cb265. */
void CM_STDCALL cm_fn_008db3a0(u32 record, volatile u32 argument,
                               volatile u32 other, volatile u32 flag)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0xe0];
    u32 local = (u32)scratch;
    u32 id, base, person, value, pointer, mode, context, result, club;
    frame.state = 0xffffffffu;
    frame.handler = 0x009644db;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    id = D(record + 9);
    base = D(0x00ae23bc);
    person = base + id * 110u;
    cm_fn_0077bbe0(local, 0xfa7, 0);
    value = D(person);
    frame.state = 0;
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
    value = (u32)(s32)(s8)B(record);
    cm_fn_00785790(local, 4, value);
    value = D(record + 5);
    cm_fn_00785790(local, 5, value);
    value = D(record + 1);
    cm_fn_00785790(local, 6, value);
    value = argument;
    cm_fn_00785790(local, 7, value);
    mode = flag;
    cm_fn_00785790(local, 8, mode);
    if (mode != 0) {
        pointer = other;
        value = D(pointer + 8);
    } else {
        context = D(0x00ae1050);
        result = cm_fn_004cf5a0(context, person);
        D(0x00ae1080) = result;
        if (result == 0)
            goto done;
        id = D(result + 4);
        base = D(0x00ae23b4);
        club = base + id * 581u;
        if (club == 0)
            goto done;
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
        value = D(club);
    }
    cm_fn_00785790(local, 9, value);
    id = D(record + 5);
    base = D(0x00ae23b4);
    club = base + id * 581u;
    context = D(0x00b7431c);
    cm_fn_007861e0(context, local, club);
    id = D(record + 1);
    base = D(0x00ae23b4);
    club = base + id * 581u;
    context = D(0x00b7431c);
    cm_fn_007861e0(context, local, club);
done:
    CM_FS(u32, 0) = frame.prev;
}
