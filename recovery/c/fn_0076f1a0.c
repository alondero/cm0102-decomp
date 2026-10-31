#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
/* MOV AL at 00785790 consumes only the signed low index byte. */
extern unsigned long long CM_THISCALL cm_fn_00785790(u32, u8, u32);
extern unsigned long long CM_THISCALL cm_fn_00785d40(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
/* Preserve the explicit incoming ECX set from saved self at 0076f244. */
extern u32 CM_THISCALL cm_fn_0076d560(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);

unsigned long long CM_THISCALL cm_fn_0076f1a0(u32 self, u32 club,
                                             u32 option, u32 record)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0xe4];
    u32 local = (u32)scratch;
    u32 pointer, id, target, special, list, value, node, context;
    u16 count;
    u8 index;
    unsigned long long result;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095de1b;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(local) = self;
    special = 0;
    list = 0;
    count = 0;
    cm_fn_0077bbe0(local + 4, 30, 0);
    frame.state = 0;
    pointer = D(record + 0x14);
    if (pointer != 0) {
        id = D(pointer);
        target = D(0x009cf79c);
        if (id == target || id == D(0x009cf7a0) || id == D(0x009cf7a4) ||
            id == D(0x009cf7a8) || id == D(0x009cf7ac) || id == D(0x009cf790) ||
            id == D(0x009cf8a0) || id == D(0x009cf78c) || id == D(0x009cf964))
            special = 1;
    }
    if (special == 0) {
        context = D(local);
        list = cm_fn_0076d560(context, club, record);
        count = W(list);
    }
    value = D(club);
    cm_fn_00785790(local + 4, 0, value);
    cm_fn_00785790(local + 4, 1, option);
    cm_fn_00785790(local + 4, 2, 0);
    cm_fn_00785790(local + 4, 3, special);
    cm_fn_00785790(local + 4, 4, (u32)(s32)(s16)count);
    pointer = D(record + 0x14);
    value = D(pointer);
    cm_fn_00785790(local + 4, 5, value);
    value = W(record + 0x2a);
    cm_fn_00785790(local + 4, 6, value);
    value = W(record + 0x28);
    cm_fn_00785790(local + 4, 7, value);
    if ((s16)count < 4 && list != 0 && special == 0) {
        value = D(list + 2);
        index = 8;
        D(list + 10) = value;
        node = cm_fn_0068d1b0(list);
        while (node != 0) {
            pointer = D(node);
            value = D(pointer);
            cm_fn_00785790(local + 4, index, value);
            ++index;
            node = cm_fn_0068d1b0(list);
        }
    }
    pointer = D(club + 0xcf);
    if (pointer == 0) {
        value = D(club + 0x53);
        pointer = cm_fn_0053b3d0(value);
        pointer = D(pointer + 0xcf);
    }
    context = D(0x00b7431c);
    result = cm_fn_00785d40(context, local + 4, pointer, 0);
    CM_FS(u32, 0) = frame.prev;
    return result;
}
