/* Preferred-image VA, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8: incoming ECX is forwarded to 008d1130.
   Original callees remain explicit execution boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
#define SW(a) CM_GLOBAL(volatile s16, (u32)(a))
#define UW(a) CM_GLOBAL(volatile u16, (u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern s8 CM_THISCALL cm_fn_008d1130(u32, u32, u32, u32, s8);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);

extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Sole indexed caller008be8eb kills EAX at008be8fd. */
void CM_THISCALL cm_fn_008e1780(u32 self, u32 offer, u32 related)
{
    volatile struct { u32 prev, handler, state; } frame;
    /* P+C record, P+EC selected, P+164/P+264 report buffers.
       P is after saving EBX/ESI/EDI; frame.prev is P+364. */
    u8 scratch[0x358];
    u32 record = (u32)scratch, selected = record + 0xe0u;
    u32 index, person, pointer, value, mapped, owner, base, manager;
    s8 recipients;
    s32 remaining;
    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0096470bu;
    CM_FS(u32, 0) = (u32)&frame;
    index = D(offer + 0xcu);
    person = D(0x00ae23bcu) + index * 110u;
    recipients = cm_fn_008d1130(self, offer, 0x2e, selected, 30);
    if (recipients != 0) {
        (void)cm_fn_0077bbe0((void *)record, 0xfb5, 0);
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
        owner = 0;
        if (mapped != 0 && (B(mapped + 0x35u) & 0x3fu) != 5u) {
            index = D(mapped + 4u);
            base = D(0x00ae23b4u);
            owner = base + index * 581u;
        }
        value = D(owner); /* Original008e187e includes null. */
        (void)cm_fn_00785790((void *)record, 5, value);
        value = D(offer + 0x20u);
        (void)cm_fn_00785790((void *)record, 6, value);
        value = D(offer + 0x24u);
        (void)cm_fn_00785790((void *)record, 7, value);
        value = D(related + 0xcu);
        (void)cm_fn_00785790((void *)record, 8, value);
        value = (u32)(s32)(s8)B(offer + 0x2cu);
        (void)cm_fn_00785790((void *)record, 9, value);
        value = (u32)(s32)SW(related + 0x2du);
        (void)cm_fn_00785790((void *)record, 10, value);
        value = UW(related + 0x2fu);
        (void)cm_fn_00785790((void *)record, 11, value);
        manager = D(0x00ae1050u);
        mapped = cm_fn_004cf5a0(manager, person);
        if (mapped == 0) {
            (void)cm_fn_00944cff(0x00a8d514u, 0, 0,
                scratch + 0x258, scratch + 0x158);
            /* Earlier pushes belong to 944c9f, not to void-argument90d130. */
            value = cm_fn_0090d130();
            (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, value,
                scratch + 0x258, scratch + 0x158, 0x5e4cu);
            cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
            D(0x00b67a34u) = 0;
        } else {
            value = D(mapped + 0xcu);
            (void)cm_fn_00785790((void *)record, 12, value);
            if (recipients > 0) {
                pointer = selected;
                remaining = recipients;
                do {
                    value = D(pointer);
                    manager = D(0x00b7431cu);
                    (void)cm_fn_007861e0((void *)manager, record, value);
                    pointer += 4u;
                    --remaining;
                } while (remaining != 0);
            }
        }
    }
    CM_FS(u32, 0) = frame.prev;
}
