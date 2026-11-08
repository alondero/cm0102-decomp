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

extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_THISCALL cm_fn_00786780(void *, u32);
extern u32 CM_STDCALL cm_fn_007875e0(u32);
extern s32 CM_THISCALL cm_fn_008e7a70(u32, u32, u32, u32, u32);

/* Sole indexed caller008c3493 kills EAX at008c3498. */
void CM_THISCALL cm_fn_008e05d0(u32 self, u32 offer, u32 date)
{
    volatile struct { u32 prev, handler, state; } frame;
    /* P+C record, P+EC selected; P is after saving ESI/EDI.
       Keep their adjacency; no scratch initialization is inferred. */
    u8 scratch[0x158];
    u32 record = (u32)scratch, selected = record + 0xe0u;
    u32 index, person, pointer, value, mapped, owner, base, manager, node;
    u8 count = 0, slot = 13;
    s8 recipients;
    s32 remaining;
    frame.state = 0xffffffffu;
    frame.handler = 0x0096469bu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    index = D(offer + 0xcu);
    person = D(0x00ae23bcu) + index * 110u;
    recipients = cm_fn_008d1130(self, offer, 0xf, selected, 30);
    if (recipients != 0) {
        (void)cm_fn_0077bbe0((void *)record, 0xfb3, 0);
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
        value = 0xffffffffu;
        if (owner != 0) {
            manager = D(0x00ae1050u);
            mapped = cm_fn_004cf5a0(manager, person);
            D(0x00ae1080u) = mapped;
            owner = 0;
            if (mapped != 0 && (B(mapped + 0x35u) & 0x3fu) != 5u) {
                index = D(mapped + 4u);
                base = D(0x00ae23b4u);
                owner = base + index * 581u;
            }
            value = D(owner); /* Includes the original null read at008e0711. */
        }
        (void)cm_fn_00785790((void *)record, 5, value);
        value = D(offer + 0x1bu);
        (void)cm_fn_00785790((void *)record, 6, value);
        value = (u32)(s32)(s8)B(offer + 0x2fu);
        (void)cm_fn_00785790((void *)record, 7, value);
        value = (u32)cm_fn_008e7a70(offer, 0, 1, 0, 1);
        (void)cm_fn_00785790((void *)record, 8, value);
        value = (u32)((s32)(s8)(u8)(B(offer + 0x30u) << 4) >> 4);
        (void)cm_fn_00785790((void *)record, 9, value);
        value = (u32)(s32)SW(date);
        (void)cm_fn_00785790((void *)record, 10, value);
        value = UW(date + 2u);
        (void)cm_fn_00785790((void *)record, 11, value);
        pointer = D(offer + 4u);
        if (pointer != 0) {
            value = D(pointer + 2u);
            D(pointer + 0xau) = value;
            pointer = D(offer + 4u);
            node = cm_fn_0068d1b0(pointer);
            while (node != 0) {
                index = D(node);
                base = D(0x00ae23bcu);
                person = base + index * 110u;
                if ((s8)count < 3) {
                    /* 00785790 consumes only signed low-byte index. */
                    value = D(person);
                    (void)cm_fn_00785790((void *)record, slot++, value);
                    pointer = D(person + 4u); value = D(pointer + 0x33u);
                    (void)cm_fn_00785790((void *)record, slot++, value);
                    pointer = D(person + 8u); value = D(pointer + 0x33u);
                    (void)cm_fn_00785790((void *)record, slot++, value);
                    pointer = D(person + 0xcu); value = D(pointer + 0x33u);
                    (void)cm_fn_00785790((void *)record, slot++, value);
                }
                pointer = D(offer + 4u);
                ++count;
                node = cm_fn_0068d1b0(pointer);
            }
        }
        value = (u32)(s32)(s8)count;
        (void)cm_fn_00785790((void *)record, 12, value);
        index = D(offer + 8u);
        base = D(0x00ae23b4u);
        owner = base + index * 581u;
        manager = D(0x00b7431cu); /* Original read precedes stdcall. */
        (void)manager;
        if (cm_fn_007875e0(owner) != 0) {
            manager = D(0x00b7431cu);
            (void)cm_fn_00786780((void *)manager, record);
        } else if (recipients > 0) {
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
    CM_FS(u32, 0) = frame.prev;
}
