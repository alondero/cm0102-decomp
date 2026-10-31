#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_007863f0(void *self, u32 arg1, u32 arg2);

void CM_CDECL cm_fn_0058a4d0(volatile u8 *record, s8 byte_arg, u32 value_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local_area[0xe0];
    u32 entry_prev = CM_FS(u32, 0);
    u32 child;
    u32 nested;
    u32 global_object;
    u8 *buffer = (u8 *)local_area;

    frame.state = 0xffffffffu;
    frame.handler = 0x00958c9bu;
    frame.prev = entry_prev;
    CM_FS(u32, 0) = (u32)&frame;

    if (record != 0 && *(volatile u32 *)(record + 0x53) != 0 &&
        *(volatile u32 *)(record + 0xcf) != 0) {
        (void)cm_fn_0077bbe0(buffer, 0x178c, 0);
        child = *(volatile u32 *)record;
        frame.state = 0;
        (void)cm_fn_00785790(buffer, 0, child);

        nested = *(volatile u32 *)(record + 0xcf);
        if (nested != 0) {
            child = *(volatile u32 *)nested;
            (void)cm_fn_00785790(buffer, 1, child);
            child = *(volatile u32 *)(nested + 4);
            child = *(volatile u32 *)(child + 0x33);
            (void)cm_fn_00785790(buffer, 2, child);
            child = *(volatile u32 *)(nested + 8);
            child = *(volatile u32 *)(child + 0x33);
            (void)cm_fn_00785790(buffer, 3, child);
            child = *(volatile u32 *)(nested + 0x0c);
            child = *(volatile u32 *)(child + 0x33);
        } else {
            (void)cm_fn_00785790(buffer, 1, 0xffffffffu);
            (void)cm_fn_00785790(buffer, 2, 0xffffffffu);
            (void)cm_fn_00785790(buffer, 3, 0xffffffffu);
            child = 0xffffffffu;
        }
        (void)cm_fn_00785790(buffer, 4, child);
        nested = *(volatile u32 *)(record + 0x53);
        child = *(volatile u32 *)nested;
        (void)cm_fn_00785790(buffer, 5, child);
        (void)cm_fn_00785790(buffer, 6, (u32)(s32)byte_arg);
        (void)cm_fn_00785790(buffer, 7, value_arg);

        if (byte_arg != 0 && *(volatile s16 *)(record + 0x80) > 0x128e) {
            nested = *(volatile u32 *)(record + 0x53);
            global_object = CM_GLOBAL(volatile u32, 0x00b7431c);
            (void)cm_fn_007863f0((void *)global_object, (u32)buffer, nested);
        } else {
            global_object = CM_GLOBAL(volatile u32, 0x00b7431c);
            (void)cm_fn_007861e0((void *)global_object, (u32)buffer, (u32)record);
        }
    }

    {
        u32 restore_prev = frame.prev;
        CM_FS(u32, 0) = restore_prev;
    }
}
