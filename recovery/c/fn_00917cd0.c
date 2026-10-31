#include "cmre.h"

extern u32 CM_CDECL cm_fn_00945ce6(void *destination, u32 element_size,
                                   u32 count, u32 parameter);

u32 CM_THISCALL cm_fn_00917cd0(void *self, u32 parameter)
{
    u32 count_address = (u32)self + 0xb6u;
    u32 count;
    u32 index;
    u32 list;
    u32 slot;
    u32 value;

    (void)cm_fn_00945ce6((void *)((u32)self + 0xbau), 1u, 1u, parameter);
    (void)cm_fn_00945ce6((void *)((u32)self + 0xbbu), 1u, 1u, parameter);
    (void)cm_fn_00945ce6((void *)((u32)self + 0xbcu), 1u, 1u, parameter);
    (void)cm_fn_00945ce6((void *)((u32)self + 0xb6u), 4u, 1u, parameter);

    count = *(volatile u32 *)count_address;
    index = 0u;
    if ((s32)count > 0) {
        do {
            list = *(volatile u32 *)((u32)self + 0xb2u);
            slot = list + index * 4u;
            value = *(volatile u32 *)slot;
            if (value != 0u) {
                value = *(volatile u32 *)value;
                *(volatile u32 *)slot = value;
            } else {
                *(volatile u32 *)slot = 0xffffffffu;
            }
            count = *(volatile u32 *)count_address;
            ++index;
        } while ((s32)index < (s32)count);
    }

    count = *(volatile u32 *)count_address;
    list = *(volatile u32 *)((u32)self + 0xb2u);
    (void)cm_fn_00945ce6((void *)list, 4u, count, parameter);

    count = *(volatile u32 *)count_address;
    index = 0u;
    if ((s32)count > 0) {
        do {
            list = *(volatile u32 *)((u32)self + 0xb2u);
            slot = list + index * 4u;
            value = *(volatile u32 *)slot;
            if (value != 0xffffffffu) {
                u32 base = CM_GLOBAL(volatile u32, 0x00ae23b4);
                u32 scaled = value * 0x245u;
                *(volatile u32 *)slot = base + scaled;
            } else {
                *(volatile u32 *)slot = 0u;
            }
            count = *(volatile u32 *)count_address;
            ++index;
        } while ((s32)index < (s32)count);
    }
    return count;
}
