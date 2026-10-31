#include "cmre.h"

typedef struct {
    void *prev;
    void *handler;
    s32 state;
} cm_seh_record_008cdc90;

extern void CM_FASTCALL cm_fn_0068d020(u16 *iterator);
extern void CM_FASTCALL cm_fn_0068d040(void *iterator);
extern void *CM_FASTCALL cm_fn_0068d1b0(void *iterator);
extern void CM_THISCALL cm_fn_008c5cf0(void *self, const volatile u32 *first,
                                       void *iterator, u32 zero);

u32 CM_THISCALL cm_fn_008cdc90(volatile u8 *self,
                               const volatile u32 *first,
                               const volatile u32 *second)
{
    volatile cm_seh_record_008cdc90 seh;
    volatile u8 iterator[0x10];
    void *entry_prev;
    void *restore_prev;
    void *item;
    u32 eligible = 1u;

    seh.state = -1;
    seh.handler = (void *)0x00963f18u;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;
    cm_fn_0068d020((u16 *)iterator);
    seh.state = 0;

    if (first != 0) {
        u32 first_index = *first;
        s32 limit = CM_GLOBAL(volatile s32, 0x00ae2364u);
        if ((s32)first_index >= limit)
            eligible = 0u;
    }

    if (eligible != 0u) {
        u32 index = *first;
        u32 table = *(volatile u32 *)(self + 0x86eu);
        if ((*(volatile u8 *)(table + index * 8u) & 1u) != 0u) {
            cm_fn_008c5cf0((void *)self, first, (void *)iterator, 0u);
            *(volatile u32 *)(iterator + 0x0au) =
                *(volatile u32 *)(iterator + 2u);
            item = cm_fn_0068d1b0((void *)iterator);
            while (item != 0) {
                u32 item_index = *(volatile u32 *)item;
                u32 array = *(volatile u32 *)self;
                u32 record = *(volatile u32 *)(array + item_index * 4u);
                u32 expected = *second;
                u32 actual = *(volatile u32 *)(record + 8u);
                if (actual == expected) {
                    seh.state = -1;
                    cm_fn_0068d040((void *)iterator);
                    restore_prev = seh.prev;
                    CM_FS(void *, 0) = restore_prev;
                    return 1u;
                }
                item = cm_fn_0068d1b0((void *)iterator);
            }
        }
    }

    /* The original reaches common cleanup when the signed range guard fails. */
    seh.state = -1;
    cm_fn_0068d040((void *)iterator);
    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
    return 0u;
}
