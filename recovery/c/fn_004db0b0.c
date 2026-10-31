#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 a, u32 b);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 a, u32 b);

static u32 cm_fn_004db0b0_select(volatile u8 *arg)
{
    if (arg == 0) return 0;
    s32 index = *(volatile s32 *)arg;
    if (index >= CM_GLOBAL(volatile s32, 0x00ae2364)) return 0;
    u32 table = CM_GLOBAL(volatile u32, 0x00ae2d0c);
    s32 row = *(volatile s32 *)(table + (u32)index * 79u);
    if (row < 0) return 0;
    u32 list = CM_GLOBAL(volatile u32, 0x00ae1050);
    return *(volatile u32 *)list + (u32)row * 80u;
}

static u32 cm_fn_004db0b0_secondary(u32 selected)
{
    u32 value = *(volatile u32 *)(selected + 4);
    u32 base = CM_GLOBAL(volatile u32, 0x00ae23b4);
    return base + value * 581u;
}

void CM_STDCALL cm_fn_004db0b0(volatile u8 *record, volatile u8 *other)
{
    volatile struct { u32 prev; u32 handler; s32 state; } frame;
    volatile u8 local[224];
    u32 previous = CM_FS(u32, 0);
    frame.state = -1;
    frame.handler = 0x0095702b;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;

    u32 selected = cm_fn_004db0b0_select(record);
    CM_GLOBAL(volatile u32, 0x00ae1080) = selected;
    if (selected == 0) goto done;
    u32 base = cm_fn_004db0b0_secondary(selected);
    if (base == 0) goto done;

    cm_fn_0077bbe0((void *)local, 0xfb8, 0);
    u32 v = *(volatile u32 *)record;
    frame.state = 0;
    cm_fn_00785790((void *)local, 0, v);
    u32 p = *(volatile u32 *)(record + 4);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 1, v);
    p = *(volatile u32 *)(record + 8);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 2, v);
    p = *(volatile u32 *)(record + 0xc);
    v = *(volatile u32 *)(p + 0x33);
    cm_fn_00785790((void *)local, 3, v);
    p = *(volatile u32 *)(record + 0x39);
    v = *(volatile u32 *)p;
    cm_fn_00785790((void *)local, 4, v);

    selected = cm_fn_004db0b0_select(record);
    CM_GLOBAL(volatile u32, 0x00ae1080) = selected;
    u32 p2 = selected == 0 ? 0 : cm_fn_004db0b0_secondary(selected);
    v = *(volatile u32 *)p2;
    cm_fn_00785790((void *)local, 5, v);
    u32 saved = (u32)other;
    v = *(volatile u32 *)saved;
    cm_fn_00785790((void *)local, 6, v);
    p = *(volatile u32 *)(record + 0x39);
    if (saved == p) {
        selected = cm_fn_004db0b0_select(record);
        CM_GLOBAL(volatile u32, 0x00ae1080) = selected;
        p2 = selected == 0 ? 0 : cm_fn_004db0b0_secondary(selected);
        cm_fn_007861e0((void *)CM_GLOBAL(volatile u32, 0x00b7431c),
                       (u32)local, p2);
    } else {
        cm_fn_007861e0((void *)CM_GLOBAL(volatile u32, 0x00b7431c),
                       (u32)local, p);
    }
done:
    CM_FS(u32, 0) = frame.prev;
}
