#include "cmre.h"

extern void CM_THISCALL cm_fn_0077bbe0(void *self, u32 a, u32 b);
extern void CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern void CM_THISCALL cm_fn_007861e0(void *self, u32 a, u32 b);

void CM_CDECL cm_fn_00589300(volatile u32 *object, volatile u32 *other)
{
    struct seh_frame { u32 prev, handler, state; } volatile frame;
    volatile u8 local[0xe0];
    u32 entry_prev = CM_FS(u32, 0);
    frame.state = (u32)-1;
    frame.handler = 0x00958c1bu;
    frame.prev = entry_prev;
    CM_FS(u32, 0) = (u32)(unsigned long)&frame;

    if (object != 0) {
        u32 record = *(volatile u32 *)((volatile u8 *)object + 0x61);
        if (record != 0) {
            if (*(volatile u32 *)((volatile u8 *)object + 0x1a) != 0) {
                if (other != 0) {
                    u32 relation = *(volatile u32 *)((volatile u8 *)other + 0x53);
                    if (relation != 0) {
                        u32 other_state = *(volatile u32 *)((volatile u8 *)other + 0x69);
                        if (other_state != 0) {
                            if (*(volatile u32 *)(other_state + 0x38) != 0) {
                                u32 fields = *(volatile u32 *)((volatile u8 *)other + 0xcf);
                                if (fields != 0) {
                                    cm_fn_0077bbe0((void *)local, 0xbde, 0);
                                    u32 value = object[0];
                                    frame.state = 0;
                                    cm_fn_00785790((void *)local, 0, value);
                                    u32 p = object[1];
                                    cm_fn_00785790((void *)local, 1,
                                        *(volatile u32 *)(p + 0x33));
                                    p = object[2];
                                    cm_fn_00785790((void *)local, 2,
                                        *(volatile u32 *)(p + 0x33));
                                    p = object[3];
                                    cm_fn_00785790((void *)local, 3,
                                        *(volatile u32 *)(p + 0x33));
                                    cm_fn_00785790((void *)local, 4, other[0]);

                                    record = *(volatile u32 *)((volatile u8 *)object + 0x61);
                                    s16 raw = *(volatile s16 *)(record + 0x0b);
                                    cm_fn_00785790((void *)local, 5,
                                        raw > 0x1c52 ? 2u : 1u);

                                    fields = *(volatile u32 *)((volatile u8 *)other + 0xcf);
                                    cm_fn_00785790((void *)local, 6,
                                        *(volatile u32 *)fields);
                                    p = *(volatile u32 *)(fields + 4);
                                    cm_fn_00785790((void *)local, 7,
                                        *(volatile u32 *)(p + 0x33));
                                    p = *(volatile u32 *)(fields + 8);
                                    cm_fn_00785790((void *)local, 8,
                                        *(volatile u32 *)(p + 0x33));
                                    p = *(volatile u32 *)(fields + 0x0c);
                                    cm_fn_00785790((void *)local, 9,
                                        *(volatile u32 *)(p + 0x33));
                                    u32 global = CM_GLOBAL(volatile u32, 0x00b7431cu);
                                    cm_fn_007861e0((void *)global,
                                        (u32)(unsigned long)local,
                                        (u32)(unsigned long)other);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    CM_FS(u32, 0) = frame.prev;
}
