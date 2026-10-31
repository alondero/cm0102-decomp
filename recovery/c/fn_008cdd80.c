#include "cmre.h"

extern void CM_THISCALL cm_fn_0068d020(u8 *self);
extern u32 CM_THISCALL cm_fn_008c5cf0(u32 self, u32 a1, u32 a2, u32 a3);
extern u32 CM_THISCALL cm_fn_0068d1b0(u8 *self);
extern void CM_THISCALL cm_fn_0068d040(u8 *self);

/* Thiscall with three stack arguments (ret 0xc). The object's fields are used by offset:
   +0x86e table pointer (byte flags at 8 * index). The 16-byte block cursor_block is
   initialized by 0x68d020 (word at +0, dwords at +2, +6, +0xa); +0x2 holds a list head
   copied to +0xa, which 0x68d1b0 pops as a cursor. The SEH frame {prev, handler, state}
   is linked at fs:[0] for the call and unlinked on every return path. */
u32 CM_THISCALL cm_fn_008cdd80(u8 *self, u32 arg1, u32 arg2, u32 arg3)
{
    volatile struct {
        u32 prev;
        u32 handler;
        s32 state;
    } frame;
    u8 cursor_block[0x10];
    u32 prev_head;
    u32 index;
    u32 table;
    u32 flags;
    u32 limit;
    u32 signed_index;
    u32 value;
    u32 node;
    u32 entry;
    u32 key;
    u32 entry_key;

    frame.state = -1;
    frame.handler = 0x00963f38U;
    prev_head = CM_FS(u32, 0);
    frame.prev = prev_head;
    CM_FS(u32, 0) = (u32)&frame;

    cm_fn_0068d020(cursor_block);
    frame.state = 0;
    if (arg1 != 0) {
        signed_index = *(volatile u32 *)arg1;
        limit = CM_GLOBAL(volatile u32, 0x00ae2364);
        if ((s32)signed_index >= (s32)limit) {
            goto unlink_fail;
        }
    }
    if (arg3 == 0) {
        index = *(volatile u32 *)arg1;
        table = *(volatile u32 *)(self + 0x86e);
        flags = *(volatile u8 *)(table + index * 8U);
        if ((flags & 1U) == 0) {
            goto unlink_fail;
        }
    }

    cm_fn_008c5cf0((u32)self, arg1, (u32)cursor_block, arg3);
    value = *(volatile u32 *)(cursor_block + 2);
    *(volatile u32 *)(cursor_block + 0xa) = value;

    node = cm_fn_0068d1b0(cursor_block);
    while (node != 0) {
        index = *(volatile u32 *)node;
        table = *(volatile u32 *)self;
        entry = *(volatile u32 *)(table + index * 4U);
        key = *(volatile u32 *)arg2;
        entry_key = *(volatile u32 *)(entry + 8U);
        if (entry_key == key) {
            goto found;
        }
        node = cm_fn_0068d1b0(cursor_block);
    }

unlink_fail:
    frame.state = -1;
    cm_fn_0068d040(cursor_block);
    CM_FS(u32, 0) = frame.prev;
    return 0;

found:
    frame.state = -1;
    cm_fn_0068d040(cursor_block);
    CM_FS(u32, 0) = frame.prev;
    return entry;
}
