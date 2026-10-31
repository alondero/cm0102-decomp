#include "cmre.h"

static void cm_scan_part(volatile u8 *self, u32 pointer_off, u32 records_off)
{
    u32 current = *(volatile u32 *)(self + pointer_off);
    if (current != 0 && *(volatile s8 *)(current + 0x19) < 0)
        *(volatile u32 *)(self + pointer_off) = 0;
    if (*(volatile u32 *)(self + pointer_off) != 0) return;

    volatile u8 *base = self + records_off;
    volatile u8 *candidate = 0;
    for (s32 i = 0; i < 20; ++i) {
        volatile u8 *r = base + (u32)i * 0x1be;
        u32 selected = *(volatile u32 *)(self + pointer_off);
        if (selected == 0) {
            s8 day = *(volatile s8 *)(r + 0x19);
            if (day >= 0 && day <= 10) {
                s8 region = *(volatile s8 *)(r + 0x27);
                u32 table = *(volatile u32 *)(self + 0x8ebc + (s32)region * 4);
                if (*(volatile s16 *)(table + (s32)day * 2) == 1)
                    *(volatile u32 *)(self + pointer_off) = (u32)r;
            }
        }
        s8 other_day = *(volatile s8 *)(r + 0x1a);
        if (other_day >= 0 && other_day <= 10) {
            s8 region = *(volatile s8 *)(r + 0x27);
            u32 table = *(volatile u32 *)(self + 0x8ebc + (s32)region * 4);
            if (*(volatile s16 *)(table + (s32)other_day * 2) == 1 &&
                *(volatile s8 *)(r + 0x19) == -2 &&
                *(volatile s8 *)(r + 0x20) == -1)
                candidate = r;
        }
    }
    if (*(volatile u32 *)(self + pointer_off) != 0) return;

    volatile u8 *r = base;
    for (s32 i = 0; i < 20; ++i, r += 0x1be) {
        if (*(volatile s8 *)(r + 0x19) >= 0) {
            s8 saved = 0;
            if (candidate != 0) {
                u8 replacement = *(volatile u8 *)(r + 0x19);
                saved = *(volatile s8 *)(candidate + 0x1a);
                *(volatile u8 *)(candidate + 0x1a) = replacement;
            }
            *(volatile u32 *)(self + pointer_off) = (u32)r;
            s8 region = *(volatile s8 *)(r + 0x27);
            u32 table = *(volatile u32 *)(self + 0x8ebc + (s32)region * 4);
            if (*(volatile s16 *)(table + (s32)saved * 2) == 1)
                *(volatile u8 *)(r + 0x19) = (u8)saved;
            break;
        }
    }
}

u32 CM_THISCALL cm_fn_006ce110(volatile u8 *self, s8 selector)
{
    cm_scan_part(self, 0xf59e, 0x4796);
    cm_scan_part(self, 0xf5a2, 0x6a6e);
    if (selector == 1) return *(volatile u32 *)(self + 0xf5a2);
    return *(volatile u32 *)(self + 0xf59e);
}
