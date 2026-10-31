#include "cmre.h"

void CM_THISCALL cm_fn_006c2a40(volatile u8 *self, volatile u8 *record,
    volatile u8 *out_low, volatile u8 *out_high)
{
    u8 mode = *(volatile u8 *)(self + 0x8eae);
    s8 row;
    u32 table;
    u32 index;
    u8 packed;

    if (mode == 0) {
        s8 palette = *(volatile s8 *)(record + 0x27);
        if (palette == 0) {
            row = *(volatile s8 *)(record + 0x19);
            table = *(volatile u32 *)(self + 0xf56e);
            index = (u32)(s32)row * 0xd8u;
            packed = *(volatile u8 *)(table + index + 0x9f);
            *(volatile u8 *)out_low = packed & 0x0f;
            palette = *(volatile s8 *)(record + 0x27);
            row = *(volatile s8 *)(record + 0x19);
            table = *(volatile u32 *)(self + (s32)palette * 4 + 0xf56e);
            index = (u32)(s32)row * 0xd8u;
            packed = *(volatile u8 *)(table + index + 0x9f);
            *(volatile u8 *)out_high = packed >> 4;
            return;
        }
        row = *(volatile s8 *)(record + 0x19);
        table = *(volatile u32 *)(self + (s32)palette * 4 + 0xf56e);
        index = (u32)(s32)row * 0xd8u;
        packed = *(volatile u8 *)(table + index + 0x30);
        *(volatile u8 *)out_low = packed & 0x0f;
        palette = *(volatile s8 *)(record + 0x27);
        row = *(volatile s8 *)(record + 0x19);
        table = *(volatile u32 *)(self + (s32)palette * 4 + 0xf56e);
        index = (u32)(s32)row * 0xd8u;
        packed = *(volatile u8 *)(table + index + 0x30);
        *(volatile u8 *)out_high = packed >> 4;
        return;
    }

    if (mode != 1) {
        row = *(volatile s8 *)(record + 0x19);
        s8 palette = *(volatile s8 *)(record + 0x27);
        table = *(volatile u32 *)(self + (s32)palette * 4 + 0xf56e);
        index = (u32)(s32)row * 0xd8u;
        packed = *(volatile u8 *)(table + index + 0x35);
        *(volatile u8 *)out_low = packed & 0x0f;
        palette = *(volatile s8 *)(record + 0x27);
        row = *(volatile s8 *)(record + 0x19);
        table = *(volatile u32 *)(self + (s32)palette * 4 + 0xf56e);
        index = (u32)(s32)row * 0xd8u;
        packed = *(volatile u8 *)(table + index + 0x35);
        *(volatile u8 *)out_high = packed >> 4;
        return;
    }

    s8 palette = *(volatile s8 *)(record + 0x27);
    if (palette == 1) {
        row = *(volatile s8 *)(record + 0x19);
        table = *(volatile u32 *)(self + 0xf572);
        index = (u32)(s32)row * 0xd8u;
        packed = *(volatile u8 *)(table + index + 0xa4);
        *(volatile u8 *)out_low = packed & 0x0f;
        palette = *(volatile s8 *)(record + 0x27);
        row = *(volatile s8 *)(record + 0x19);
        table = *(volatile u32 *)(self + (s32)palette * 4 + 0xf56e);
        index = (u32)(s32)row * 0xd8u;
        packed = *(volatile u8 *)(table + index + 0xa4);
        *(volatile u8 *)out_high = packed >> 4;
        return;
    }
    row = *(volatile s8 *)(record + 0x19);
    table = *(volatile u32 *)(self + (s32)palette * 4 + 0xf56e);
    index = (u32)(s32)row * 0xd8u;
    packed = *(volatile u8 *)(table + index + 0x3b);
    *(volatile u8 *)out_low = packed & 0x0f;
    palette = *(volatile s8 *)(record + 0x27);
    row = *(volatile s8 *)(record + 0x19);
    table = *(volatile u32 *)(self + (s32)palette * 4 + 0xf56e);
    index = (u32)(s32)row * 0xd8u;
    packed = *(volatile u8 *)(table + index + 0x3b);
    *(volatile u8 *)out_high = packed >> 4;
}
