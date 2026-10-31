#include "cmre.h"

/* Thiscall, three stack arguments (ret 0xc). ECX is the object: fields 0x09 (byte),
   0x0a (pointer, dereferenced when non-null), 0x0e (pointer, used when 0x0a is
   null). arg_object is the object for 0x0040a7d0 and the byte base for +1 reads;
   arg_byte is read only through its low byte; arg_dword is passed on. The 0xe8-byte
   scratch area is the frame below the SEH record; scratch+8 is the object passed
   as ECX to the 0x00785790 family, and the SEH state is written as 0, then 1. */

extern u32 CM_THISCALL cm_fn_0040a7d0(u32 self, u32 out, u32 value);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32 self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32 self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_00786520(u32 self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00786640(u32 self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00786780(u32 self, u32 arg1);

u32 CM_THISCALL cm_fn_008e9080(u32 self, u32 arg_object, u32 arg_byte, u32 arg_dword)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0xe8];
    u32 global_object;
    u32 value;
    u32 pointer;
    u32 callee_result;

    seh.state = -1;
    seh.handler = 0x00964a46u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    cm_fn_0040a7d0(arg_object, (u32)scratch, arg_dword);

    seh.state = 0;
    cm_fn_0077bbe0((u32)(scratch + 8), 0x0fb2u, 0u);

    value = (u32)(s32)(s8)*(volatile u8 *)(self + 9u);
    *(volatile u8 *)&seh.state = 1;
    cm_fn_00785790((u32)(scratch + 8), 0u, value);

    value = (u32)(s32)(s8)(u8)arg_byte;
    cm_fn_00785790((u32)(scratch + 8), 1u, value);

    value = (u32)(s32)(s8)*(volatile u8 *)(arg_object + 1u);
    cm_fn_00785790((u32)(scratch + 8), 2u, value);

    value = (u32)(s32)(s16)*(volatile u16 *)scratch;
    cm_fn_00785790((u32)(scratch + 8), 3u, value);

    value = *(volatile u32 *)(scratch + 2) & 0xffffu;
    cm_fn_00785790((u32)(scratch + 8), 4u, value);

    pointer = *(volatile u32 *)(self + 0x0au);
    if (pointer != 0) {
        value = *(volatile u32 *)pointer;
    } else {
        value = 0xffffffffu;
    }
    cm_fn_00785790((u32)(scratch + 8), 5u, value);

    pointer = *(volatile u32 *)(self + 0x0au);
    global_object = CM_GLOBAL(volatile u32, 0x00b7431cu);
    if (pointer != 0) {
        callee_result = (u32)cm_fn_00786520(global_object, (u32)(scratch + 8), pointer);
    } else {
        pointer = *(volatile u32 *)(self + 0x0eu);
        if (pointer != 0) {
            callee_result = (u32)cm_fn_00786640(global_object, (u32)(scratch + 8), pointer);
        } else {
            callee_result = (u32)cm_fn_00786780(global_object, (u32)(scratch + 8));
        }
    }

    CM_FS(u32, 0) = seh.prev;
    return callee_result;
}
