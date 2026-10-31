#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);
extern int CM_FASTCALL cm_fn_0054cfb0(short *param_1);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_00945667(u32 arg8, u32 argC);

typedef struct {
    u32 prev;
    u32 handler;
    u32 state;
} cm_fn_0058dea0_seh;

void CM_THISCALL cm_fn_0058dea0(void *self, volatile u32 *index_pointer, u16 value, u8 *owner)
{
    volatile cm_fn_0058dea0_seh frame;
    volatile u8 *object = (volatile u8 *)self;
    u32 entry_prev = CM_FS(u32, 0);
    u32 index;
    u32 array;
    u32 entry;
    u32 old_pointer;
    u16 count;
    s32 old_count;
    u32 scale;
    u32 buffer_pointer;
    u32 restore_prev;
    u8 helper_byte;
    u8 local_object[8];

    frame.state = 0xffffffffu;
    frame.handler = 0x00958e28;
    frame.prev = entry_prev;
    CM_FS(u32, 0) = (u32)&frame;

    index = *index_pointer;
    array = *(volatile u32 *)object;
    entry = array + index * 0x2c;
    count = *(volatile u16 *)(entry + 8);
    if (*(volatile u16 *)(entry + 0x0a) == count) {
        *(volatile u16 *)(entry + 8) = (u16)(count + 5);

        index = *index_pointer;
        array = *(volatile u32 *)object;
        entry = array + index * 0x2c;
        old_pointer = *(volatile u32 *)(entry + 4);
        count = *(volatile u16 *)(entry + 8);
        old_count = (s16)count;
        if (old_pointer != 0) {
            scale = (u32)(old_count * 5);
            buffer_pointer = (u32)cm_fn_00945667(old_pointer, scale);
        } else {
            scale = (u32)(old_count * 5);
            buffer_pointer = (u32)cm_fn_00944e46(scale);
        }

        index = *index_pointer;
        array = *(volatile u32 *)object;
        entry = array + index * 0x2c;
        *(volatile u32 *)(entry + 4) = buffer_pointer;
    }

    index = *index_pointer;
    array = *(volatile u32 *)object;
    entry = array + index * 0x2c;
    count = *(volatile u16 *)(entry + 0x0a);
    *(volatile u16 *)(entry + 0x0a) = (u16)(count + 1);
    old_count = (s16)count;
    scale = (u32)(old_count * 5);

    index = *index_pointer;
    array = *(volatile u32 *)object;
    entry = array + index * 0x2c;
    buffer_pointer = *(volatile u32 *)(entry + 4);
    *(volatile u16 *)(buffer_pointer + scale) = value;

    {
        u16 owner_28 = *(volatile u16 *)(owner + 0x28);
        u16 owner_2a = *(volatile u16 *)(owner + 0x2a);
        buffer_pointer = (u32)cm_fn_00549ef0((void *)local_object, owner_2a, owner_28);
    }
    frame.state = 0;
    helper_byte = (u8)cm_fn_0054cfb0((short *)buffer_pointer);

    index = *index_pointer;
    array = *(volatile u32 *)object;
    entry = array + index * 0x2c;
    buffer_pointer = *(volatile u32 *)(entry + 4);
    *(volatile u8 *)(buffer_pointer + scale + 2) = helper_byte;

    index = *index_pointer;
    array = *(volatile u32 *)object;
    entry = array + index * 0x2c;
    buffer_pointer = *(volatile u32 *)(entry + 4);
    {
        u16 owner_value = *(volatile u16 *)(owner + 0x28);
        restore_prev = frame.prev;
        *(volatile u16 *)(buffer_pointer + scale + 3) = owner_value;
    }

    CM_FS(u32, 0) = restore_prev;
}
