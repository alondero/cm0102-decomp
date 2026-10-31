/* Preferred-image VA 007e6a70, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller006b3c9e overwrites AL006b3ca3; both branches then kill EAX:
   006b3cb6 loads an argument;006b3fe8 loads a field.
   Wrapper005ae6a0 forwards the day byte;005abfcc masks it to8bits. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_FASTCALL cm_fn_0068d020(void *list);
extern void CM_FASTCALL cm_fn_0068d040(void *list);
extern u32 CM_FASTCALL cm_fn_0068d1b0(void *list);
extern u32 CM_THISCALL cm_fn_005ae6a0(u32 receiver, u32 date, u8 day, u32 kind);
extern u32 CM_STDCALL cm_fn_007e6ba0(u32 object, u32 mode);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(volatile u8 *self, const u8 *source, s16 count);
extern u8 CM_THISCALL cm_fn_005abc30(u32 self, u32 object);

void CM_THISCALL cm_fn_007e6a70(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x14];
    u32 predecessor, kind, receiver, list, head, node, pointer, id, expected;
    u8 limit, index = 0, day;
    (void)self; /* The007e6ba0 boundary uses only its two stack arguments. */
    frame.state = 0xffffffffu;
    frame.handler = 0x009607c8u;
    predecessor = CM_FS(volatile u32, 0);
    frame.prev = predecessor;
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0068d020(work + 4);
    limit = B(0x00b64738u);
    frame.state = 0;
    B((u32)work) = index;
    if ((s8)limit > 0) {
        do {
            kind = D((u32)work);
            day = B(0x00ae2c88u);
            receiver = D(0x00ae2a58u);
            list = cm_fn_005ae6a0(receiver, 0x00ae2c90u, day, kind);
            if (list != 0) {
                head = D(list + 2u);
                D(list + 0xau) = head;
                node = cm_fn_0068d1b0((void *)list);
                while (node != 0) {
                    pointer = D(node + 0x14u);
                    if (pointer != 0) {
                        id = D(pointer);
                        expected = D(0x009cf684u);
                        if (id != expected) {
                            expected = D(0x009cf5d8u);
                            if (id != expected) {
                                expected = D(0x009cf548u);
                                if (id != expected)
                                    goto next_node;
                            }
                        }
                        if (cm_fn_007e6ba0(node, 0) == 0)
                            cm_fn_0068d0d0(work + 4, (const u8 *)node, 0x4f);
                    }
next_node:
                    node = cm_fn_0068d1b0((void *)list);
                }
            }
            limit = B(0x00b64738u);
            index = (u8)(index + 1u);
            B((u32)work) = index;
        } while ((s8)index < (s8)limit);
    }
    head = D((u32)work + 6u);
    D((u32)work + 0xeu) = head;
    node = cm_fn_0068d1b0(work + 4);
    while (node != 0) {
        receiver = D(0x00ae2a58u);
        cm_fn_005abc30(receiver, node);
        node = cm_fn_0068d1b0(work + 4);
    }
    frame.state = 0xffffffffu;
    cm_fn_0068d040(work + 4);
    predecessor = frame.prev;
    CM_FS(volatile u32, 0) = predecessor;
}
