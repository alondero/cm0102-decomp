#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);

u8 CM_STDCALL cm_fn_00729bd0(u32 record, u32 related)
{
    u32 limit = CM_GLOBAL(volatile u32, 0x00ae2364u);
    u32 lower = limit - 0x10u;
    u32 first_group = *(volatile u32 *)(record + 0x1cu);
    u32 candidate = *(volatile u32 *)(first_group + 0xcfu);
    if (candidate != 0) {
        u32 id = *(volatile u32 *)candidate;
        if ((s32)id >= (s32)lower && (s32)id < (s32)limit &&
            *(volatile u8 *)(candidate + 0x5fu) != 0)
            return 0;
    }

    u32 second_group = *(volatile u32 *)(record + 0x20u);
    candidate = *(volatile u32 *)(second_group + 0xcfu);
    if (candidate != 0) {
        u32 id = *(volatile u32 *)candidate;
        if ((s32)id >= (s32)lower && (s32)id < (s32)limit &&
            *(volatile u8 *)(candidate + 0x5fu) != 0)
            return 0;
    }

    if (*(volatile u8 *)(related + 0x4cu) != 0 &&
        CM_GLOBAL(volatile u8, 0x00a5a390u) != 0)
        return 3;

    if (*(volatile u8 *)(record + 0x40u) == 0)
        return (*(volatile u8 *)(record + 0x3au) == 1u) ? 3 : 2;

    if (cm_fn_0053b900((u32 *)first_group) != 0) {
        if (*(volatile u8 *)(record + 0x40u) == 0)
            return 2;
        u32 current_group = *(volatile u32 *)(record + 0x1cu);
        s16 value = *(volatile s16 *)(current_group + 0x80u);
        s32 doubled = (s32)value * 2;
        if (doubled >= 0x2710 || value >= 0x157c)
            return 0;
        return 2;
    }

    s32 selector = (s32)*(volatile s8 *)(record + 0x3au) - 1;
    if ((u32)selector > 8u) {
        u32 info = *(volatile u32 *)(record + 0x14u);
        s8 value = *(volatile s8 *)(info + 0x69u);
        if (value > 0x0d)
            return 1;
        return value <= 6 ? 3 : 2;
    }

    u32 target = CM_GLOBAL(volatile u32, 0x00729d38u + (u32)selector * 4u);
    if (target == 0x00729c3bu)
        return 3;
    if (target == 0x00729d0eu) {
        u32 info = *(volatile u32 *)(record + 0x14u);
        s8 value = *(volatile s8 *)(info + 0x69u);
        if (value > 0x0d)
            return 1;
        return value <= 6 ? 3 : 2;
    }
    if (target == 0x00729cdb)
        return 1;

    if (target == 0x00729ce3) {
        s32 index = (s32)*(volatile s16 *)(record + 0x30u);
        if ((u32)index > 0x9bu)
            return 1;
        u8 route = CM_GLOBAL(volatile u8, 0x00729d94u + (u32)index);
        u32 secondary_target = CM_GLOBAL(volatile u32,
                                         0x00729d88u + (u32)route * 4u);
        if (secondary_target == 0x00729d2eu)
            return 0;
        if (secondary_target == 0x00729cdbu)
            return 1;
        u32 info = *(volatile u32 *)(record + 0x14u);
        s16 value = *(volatile s16 *)(info + 0x69u);
        return value <= 0x0c ? 1 : 0;
    }

    if (target == 0x00729caeu) {
        s32 threshold = (s32)*(volatile s16 *)(record + 0x32u);
        if (threshold >= 0x41f && threshold <= 0x426)
            return 0;
        s32 index = (s32)*(volatile s16 *)(record + 0x30u) - 0x78;
        if ((u32)index > 0x23u)
            return 1;
        u8 route = CM_GLOBAL(volatile u8, 0x00729d64u + (u32)index);
        u32 secondary_target = CM_GLOBAL(volatile u32,
                                         0x00729d5cu + (u32)route * 4u);
        return secondary_target == 0x00729cdbu ? 1 : 0;
    }

    return 0;
}
