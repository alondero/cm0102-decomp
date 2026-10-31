#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x25c one-byte mode selector. The mode selects one of four fixed call
   sequences. Each sequence makes eleven calls to 0x005b2ba0 with the stack
   argument as the receiver (ECX). Every call pushes three values: a zero, a
   per-row flag/size word and a row index (the index is the first stack
   argument). */
#define OBJ_U8(off) (*(volatile u8 *)(self + (off)))

extern void CM_THISCALL cm_fn_005b2ba0(u32 self, u32 arg1, u32 arg2, u32 arg3);

static void cm_row(u32 target, u32 index, u32 flags)
{
    cm_fn_005b2ba0(target, index, flags, 0u);
}

void CM_THISCALL cm_fn_008b0730(u32 self, u32 target)
{
    u8 mode;

    mode = OBJ_U8(0x25c);
    if (mode == 0) {
        cm_row(target, 0u, 1u);
        cm_row(target, 1u, 0x804u);
        cm_row(target, 2u, 0x84u);
        cm_row(target, 3u, 0x104u);
        cm_row(target, 4u, 0x404u);
        cm_row(target, 5u, 0x110u);
        cm_row(target, 6u, 0x810u);
        cm_row(target, 7u, 0x410u);
        cm_row(target, 8u, 0x140u);
        cm_row(target, 9u, 0x440u);
        cm_row(target, 10u, 0x90u);
        return;
    }
    if (mode == 2) {
        cm_row(target, 0u, 1u);
        cm_row(target, 1u, 0x808u);
        cm_row(target, 2u, 0x88u);
        cm_row(target, 3u, 0x104u);
        cm_row(target, 4u, 0x404u);
        cm_row(target, 5u, 0x202u);
        cm_row(target, 6u, 0x410u);
        cm_row(target, 7u, 0x210u);
        cm_row(target, 8u, 0x140u);
        cm_row(target, 9u, 0x440u);
        cm_row(target, 10u, 0x110u);
        return;
    }
    if (mode == 1) {
        cm_row(target, 0u, 1u);
        cm_row(target, 1u, 0x804u);
        cm_row(target, 2u, 0x84u);
        cm_row(target, 3u, 0x104u);
        cm_row(target, 4u, 0x404u);
        cm_row(target, 5u, 0x110u);
        cm_row(target, 6u, 0x210u);
        cm_row(target, 7u, 0x410u);
        cm_row(target, 8u, 0x140u);
        cm_row(target, 9u, 0x240u);
        cm_row(target, 10u, 0x440u);
        return;
    }
    cm_row(target, 0u, 1u);
    cm_row(target, 1u, 0x804u);
    cm_row(target, 2u, 0x84u);
    cm_row(target, 3u, 0x202u);
    cm_row(target, 4u, 0x204u);
    cm_row(target, 5u, 0x208u);
    cm_row(target, 6u, 0x90u);
    cm_row(target, 7u, 0x810u);
    cm_row(target, 8u, 0x220u);
    cm_row(target, 9u, 0x140u);
    cm_row(target, 10u, 0x440u);
}
