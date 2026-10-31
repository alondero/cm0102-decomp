/* Preferred-image VA 005abb10, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole direct caller 0077795c overwrites EAX at 00777965. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern void CM_FASTCALL cm_fn_0068d020(void *list);
extern void CM_FASTCALL cm_fn_0068d040(void *list);
extern u32 CM_FASTCALL cm_fn_0068d1b0(void *list);
extern void CM_THISCALL cm_fn_0054c8e0(u32 source, void *out, u32 flag);
extern void CM_THISCALL cm_fn_0054c5b0(u32 source, void *out, u32 flag);
extern u32 CM_THISCALL cm_fn_005acbe0(u32 self, u32 a, u32 b, u32 c, u32 d, u32 id, void *list, u32 g, u32 h, u32 i);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 day, u32 year);
extern u32 CM_THISCALL cm_fn_005abc30(u32 self, u32 node);
extern void CM_THISCALL cm_fn_005ab510(u32 self, u32 node);
extern u8 CM_THISCALL cm_fn_005aa680(u32 self, u32 node, u32 flag);
extern void CM_THISCALL cm_fn_005aac70(u32 self, u32 node, u32 zero, void *date);

void CM_THISCALL cm_fn_005abb10(u32 self, u32 argument, u32 source)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x28];
    u32 predecessor, id, a, b, c, d, node, year, day, head;
    frame.state = 0xffffffffu;
    frame.handler = 0x00959740u;
    predecessor = CM_FS(volatile u32, 0);
    frame.prev = predecessor;
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0068d020(work + 0x18);
    frame.state = 0;
    cm_fn_0054c8e0(source, work + 8, 1);
    CM_GLOBAL(volatile u8, (u32)&frame.state) = 1;
    cm_fn_0054c5b0(source, work, 1);
    id = D(argument);
    c = D((u32)work);
    d = D((u32)work + 2u);
    b = D((u32)work + 0xau);
    a = D((u32)work + 8u);
    CM_GLOBAL(volatile u8, (u32)&frame.state) = 2;
    cm_fn_005acbe0(self, a, b, c, d, id, work + 0x18, 0xffffffffu, 0xfffffffeu, 1);
    head = D((u32)work + 0x1au);
    D((u32)work + 0x22u) = head;
    node = cm_fn_0068d1b0(work + 0x18);
    while (node != 0) {
        year = W(node + 0x28u);
        day = W(node + 0x2au);
        cm_fn_00549ef0(work + 0x10, day, year);
        CM_GLOBAL(volatile u8, (u32)&frame.state) = 3;
        if (cm_fn_005abc30(self, node) != 0) {
            cm_fn_005ab510(self, node);
            cm_fn_005aa680(self, node, 1);
            cm_fn_005aac70(self, node, 0, work + 0x10);
        }
        CM_GLOBAL(volatile u8, (u32)&frame.state) = 2;
        node = cm_fn_0068d1b0(work + 0x18);
    }
    frame.state = 0xffffffffu;
    cm_fn_0068d040(work + 0x18);
    predecessor = frame.prev;
    CM_FS(volatile u32, 0) = predecessor;
}
