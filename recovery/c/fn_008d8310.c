#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32, u32, u32);
extern u8 CM_THISCALL cm_fn_008d1130(u32, u32, u32, u32, u32);

/* All direct callers discard EAX: full overwrites at preferred-image VAs
   008c0233, 008c06b2 and 008c65b6 precede any use of its returned value. */
void CM_THISCALL cm_fn_008d8310(u32 self, u32 record, u32 option)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x158];
    u32 local = (u32)scratch;
    u32 person, result, club, value, pointer, index, context;
    s8 count;
    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x009643eb;
    CM_FS(u32, 0) = (u32)&frame;
    index = D(record + 0xc);
    context = D(0x00ae1050);
    person = D(0x00ae23bc) + index * 110u;
    result = cm_fn_004cf5a0(context, person);
    D(0x00ae1080) = result;
    if (result != 0 && (B(result + 0x35) & 0x3f) != 5) {
        index = D(result + 4);
        club = D(0x00ae23b4) + index * 581u;
    } else {
        club = 0;
    }
    count = (s8)cm_fn_008d1130(self, record, 0x2f, local + 0xe0, 30);
    cm_fn_0077bbe0(local, 0xfae, 0);
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
    value = club != 0 ? D(club) : 0xffffffffu;
    cm_fn_00785790(local, 4, value);
    value = D(record + 8);
    cm_fn_00785790(local, 5, value);
    value = (u32)(s32)(s8)B(record + 0x1f);
    cm_fn_00785790(local, 6, value);
    cm_fn_00785790(local, 7, option);
    if (club != 0) {
        pointer = D(club + 0xcf);
        if (pointer != 0) {
            value = D(pointer);
            cm_fn_00785790(local, 8, value);
        }
    }
    index = D(record + 8);
    if ((s32)index >= 0) {
        pointer = D(0x00ae23b4);
        pointer = D(pointer + index * 581u + 0xcf);
        if (pointer != 0) {
            value = D(pointer);
            cm_fn_00785790(local, 9, value);
        }
    }
    if (count > 0) {
        pointer = local + 0xe0;
        index = (u32)(s32)count;
        do {
            value = D(pointer);
            context = D(0x00b7431c);
            cm_fn_007861e0(context, local, value);
            pointer += 4;
        } while (--index != 0);
    }
    CM_FS(u32, 0) = frame.prev;
}
