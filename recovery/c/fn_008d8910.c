/* Preferred-image VA008d8910, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12. Original callees remain explicit execution boundaries.
   Sole indexed caller008c04cf next forwards to68d1f0 without observing EAX;
   return-liveness proof is retained in the owner evidence bundle. */
#include "cmre.h"
#define D(address) CM_GLOBAL(volatile u32, (u32)(address))
#define B(address) CM_GLOBAL(volatile u8, (u32)(address))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern s8 CM_THISCALL cm_fn_008d1130(u32, u32, u32, u32, s8);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);

void CM_THISCALL cm_fn_008d8910(u32 self, u32 offer, u32 other, u32 amount)
{
    volatile struct { u32 prev, handler, state; } frame;
    /* Original P+14 two output DWORDs immediately precede P+1c record.
       No record or output initialization is inferred. */
    u8 scratch[8 + 0xe0];
    u32 selected = (u32)scratch;
    u32 record = selected + 8u;
    u32 index, manager, person, mapped, base, club, owner, value, pointer;
    s8 count;
    s32 remaining;

    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0096440bu;
    CM_FS(u32, 0) = (u32)&frame;
    index = D(offer + 0xcu);
    manager = D(0x00ae1050u);
    person = D(0x00ae23bcu) + index * 110u;
    mapped = cm_fn_004cf5a0(manager, person);
    base = D(0x00ae23b4u);
    D(0x00ae1080u) = mapped;
    if (mapped != 0 && (B(mapped + 0x35u) & 0x3fu) != 5u)
        owner = base + D(mapped + 4u) * 581u;
    else
        owner = 0;
    club = base + D(offer + 8u) * 581u;
    count = cm_fn_008d1130(self, offer, 3, selected, 2);
    if (count != 0) {
        (void)cm_fn_0077bbe0((void *)record, 0xfafu, 0);
        frame.state = 0;
        value = D(owner);
        (void)cm_fn_00785790((void *)record, 0, value);
        value = D(club);
        (void)cm_fn_00785790((void *)record, 1, value);
        value = D(person);
        (void)cm_fn_00785790((void *)record, 2, value);
        pointer = D(person + 4u); value = D(pointer + 0x33u);
        (void)cm_fn_00785790((void *)record, 3, value);
        pointer = D(person + 8u); value = D(pointer + 0x33u);
        (void)cm_fn_00785790((void *)record, 4, value);
        pointer = D(person + 0xcu); value = D(pointer + 0x33u);
        (void)cm_fn_00785790((void *)record, 5, value);
        value = D(other);
        (void)cm_fn_00785790((void *)record, 6, value);
        pointer = D(other + 4u); value = D(pointer + 0x33u);
        (void)cm_fn_00785790((void *)record, 7, value);
        pointer = D(other + 8u); value = D(pointer + 0x33u);
        (void)cm_fn_00785790((void *)record, 8, value);
        pointer = D(other + 0xcu); value = D(pointer + 0x33u);
        (void)cm_fn_00785790((void *)record, 9, value);
        (void)cm_fn_00785790((void *)record, 10, amount);
        value = D(offer + 0x1bu);
        (void)cm_fn_00785790((void *)record, 11, value);
        if (count > 0) {
            pointer = selected;
            remaining = (s32)count;
            do {
                value = D(pointer);
                manager = D(0x00b7431cu);
                (void)cm_fn_007861e0((void *)manager, record, value);
                pointer += 4u;
                --remaining;
            } while (remaining != 0);
        }
    }
    CM_FS(u32, 0) = frame.prev;
}
