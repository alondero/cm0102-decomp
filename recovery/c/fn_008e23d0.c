/* Preferred-image VA008e23d0/base00400000; primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX forwards unchanged to8d1130: explicit THIS2/RET8 boundary.
   Actual original numeric helpers remain external dependencies. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern s8 CM_THISCALL cm_fn_008d1130(u32, u32, u32, u32, s8);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);

void CM_THISCALL cm_fn_008e23d0(u32 self, u32 offer, u32 amount)
{
    volatile struct { u32 prev, handler, state; } frame;
    /* Original P+10 record immediately precedes thirty DWORDs at P+f0. */
    u8 scratch[0xe0 + 30 * 4];
    u32 record = (u32)scratch, selected = record + 0xe0u;
    u32 base, index, person, club, pointer, manager, mapped, owner, value;
    s8 count;
    s32 remaining;
    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0096478bu;
    CM_FS(u32, 0) = (u32)&frame;
    base = D(0x00ae23b4u);
    index = D(offer + 0xcu);
    person = D(0x00ae23bcu) + index * 110u;
    club = base + D(offer + 8u) * 581u;
    count = cm_fn_008d1130(self, offer, 0xf, selected, 30);
    if (count != 0) {
        (void)cm_fn_0077bbe0((void *)record, 0xfb7u, 0);
        value = D(person);
        frame.state = 0;
        (void)cm_fn_00785790((void *)record, 0, value);
        pointer = D(person + 4u); value = D(pointer + 0x33u);
        (void)cm_fn_00785790((void *)record, 1, value);
        pointer = D(person + 8u); value = D(pointer + 0x33u);
        (void)cm_fn_00785790((void *)record, 2, value);
        pointer = D(person + 0xcu); value = D(pointer + 0x33u);
        (void)cm_fn_00785790((void *)record, 3, value);
        value = D(offer + 8u);
        (void)cm_fn_00785790((void *)record, 4, value);
        manager = D(0x00ae1050u);
        mapped = cm_fn_004cf5a0(manager, person);
        D(0x00ae1080u) = mapped;
        if (mapped != 0 && (B(mapped + 0x35u) & 0x3fu) != 5u) {
            index = D(mapped + 4u);
            base = D(0x00ae23b4u);
            owner = base + index * 581u;
        } else {
            owner = 0;
        }
        /* Original unconditionally dereferences the zero owner on this path. */
        value = D(owner);
        (void)cm_fn_00785790((void *)record, 5, value);
        value = (u32)(s32)(s16)amount;
        (void)cm_fn_00785790((void *)record, 6, value);
        pointer = D(club + 0x57u);
        value = pointer != 0 ? D(pointer) : 0xffffffffu;
        (void)cm_fn_00785790((void *)record, 7, value);
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
