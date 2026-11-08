/* Preferred-image VA008de790/base00400000; primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12. Plain C; actual external helpers remain original boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern s8 CM_THISCALL cm_fn_008d1130(u32, u32, u32, u32, s8);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);

void CM_THISCALL cm_fn_008de790(u32 self, u32 offer, u32 other, u32 reason)
{
    volatile struct { u32 prev, handler, state; } frame;
    /* Original P+18 record immediately precedes thirty DWORDs at P+f8. */
    u8 scratch[0xe0 + 30 * 4];
    u32 record = (u32)scratch, selected = record + 0xe0u;
    u32 index, manager, person, mapped, base, owner, club, value, pointer;
    s16 code;
    s8 count;
    s32 remaining;
    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0096461bu;
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
    count = cm_fn_008d1130(self, offer, 0x2f, selected, 30);
    if (count != 0) {
        if (owner != 0) {
            manager = D(0x00ae1050u);
            mapped = cm_fn_004cf5a0(manager, person);
            if ((B(mapped + 0x35u) & 0x3fu) == 5u)
                owner = 0;
        }
        (void)cm_fn_0077bbe0((void *)record, 0xfb0u, 0);
        frame.state = 0;
        value = owner != 0 ? D(owner) : 0xffffffffu;
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
        value = other != 0 ? D(other) : 0xffffffffu;
        (void)cm_fn_00785790((void *)record, 6, value);
        code = (s16)reason;
        (void)cm_fn_00785790((void *)record, 7, (u32)(s32)code);
        value = (u32)(s32)(s8)B(offer + 0x1fu);
        (void)cm_fn_00785790((void *)record, 8, value);
        if (count > 0) {
            pointer = selected;
            remaining = (s32)count;
            do {
                value = D(pointer);
                if (value != other || code != 11) {
                    manager = D(0x00b7431cu);
                    (void)cm_fn_007861e0((void *)manager, record, value);
                }
                pointer += 4u;
                --remaining;
            } while (remaining != 0);
        }
    }
    CM_FS(u32, 0) = frame.prev;
}
