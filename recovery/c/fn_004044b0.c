#include "cmre.h"

extern void CM_THISCALL cm_fn_00687970(u8 *self, u8 *buffer);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern void CM_FASTCALL cm_fn_0049f450(int *param_1);
extern void CM_FASTCALL cm_fn_00682300(u32 *param_1);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

/* Slot 0 of an object's table, called with the object in ECX and one stack argument. */
typedef void (CM_THISCALL *cm_slot0_fn)(u8 *self, s32 flag);

/* Thiscall with no stack arguments (ret 0). The object's fields are used by offset:
   +0x00 table pointer, +0x08 block pointer, +0x0c array base, +0x30 signed count,
   +0xba pending block pointer. The function links an SEH frame {prev, handler, state} at
   fs:[0] for its duration and unlinks it on return. */
void CM_THISCALL cm_fn_004044b0(u8 *param_1)
{
    volatile struct {
        u32 prev;
        u32 handler;
        s32 state;
    } frame;
    u32 prev_head;
    u32 element;
    u32 vtable;
    s32 index;
    s32 count;
    u32 block;

    frame.state = -1;
    frame.handler = 0x00953588U;
    prev_head = CM_FS(u32, 0);
    frame.prev = prev_head;
    CM_FS(u32, 0) = (u32)&frame;

    *(u32 *)param_1 = 0x00967324U;
    frame.state = 0;
    cm_fn_00687970(param_1, (u8 *)0);

    block = *(u32 *)(param_1 + 0xba);
    if (block != 0) {
        cm_fn_009452ca(block);
        *(u32 *)(param_1 + 0xba) = 0;
    }

    count = *(s32 *)(param_1 + 0x30);
    index = 0;
    if (count >= 0) {
        do {
            element = *(u32 *)(*(u32 *)(param_1 + 0xc) + (u32)index * 4U);
            if (element != 0) {
                vtable = *(u32 *)element;
                ((cm_slot0_fn)(*(u32 *)vtable))((u8 *)element, 1);
            }
            count = *(s32 *)(param_1 + 0x30);
            index++;
        } while (index <= count);
    }

    block = *(u32 *)(param_1 + 0xc);
    *(s32 *)(param_1 + 0x30) = -1;
    cm_fn_009452ca(block);

    block = *(u32 *)(param_1 + 8);
    if (block != 0) {
        cm_fn_0049f450((int *)block);
        cm_fn_00944c94((void *)block);
    }

    frame.state = -1;
    cm_fn_00682300((u32 *)param_1);
    CM_FS(u32, 0) = frame.prev;
}
