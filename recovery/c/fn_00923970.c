#include "cmre.h"

u8 CM_STDCALL cm_fn_00923970(volatile u8 *date, volatile u8 *outer)
{
    u32 total;
    u32 first_part;
    u32 second_part;
    u32 fourth_part;
    u16 third_part;
    s32 remainder;
    s32 measure;
    s8 code;
    s8 month;
    u8 flag;
    volatile u8 *entity;

    if (outer == 0)
        return 0;
    entity = (volatile u8 *)*(volatile u32 *)(outer + 0x18);
    if (entity == 0)
        return 0;
    if (*(volatile u8 *)(entity + 0x4c) != 0)
        return 0;

    first_part = *(volatile u32 *)(outer + 0x10);
    second_part = *(volatile u32 *)(outer + 0x0c);
    third_part = *(volatile u16 *)(outer + 0x2a);
    total = (u32)third_part + first_part;
    fourth_part = *(volatile u32 *)outer;
    total += second_part;
    total += fourth_part;
    remainder = (s32)total % 20;

    code = *(volatile s8 *)(date + 1);
    if (code == 5) {
        month = *(volatile s8 *)(date + 2);
        if (month < 1 || month >= 0x1d)
            goto fallback;
        flag = *(volatile u8 *)(entity + 0x4d);
        measure = *(volatile s32 *)(entity + 0x3c);
        if (flag != 0)
            return measure > 0x6b6c ? 2 : 1;
        return measure <= 0x445c ? 8 : 1;
    }
    if (code == 1) {
        month = *(volatile s8 *)(date + 2);
        if (month < 1 || month >= 0x1d)
            goto fallback;
        flag = *(volatile u8 *)(entity + 0x4d);
        if (flag == 0)
            return 2;
        measure = *(volatile s32 *)(entity + 0x3c);
        if (measure <= 0x6b6c)
            return remainder >= 6 ? 0 : 2;
        return remainder >= 4 ? 0 : 2;
    }
    if (code == 2 || code == 3) {
        month = *(volatile s8 *)(date + 2);
        if (month < 1 || month >= 0x1d)
            goto fallback;
        flag = *(volatile u8 *)(entity + 0x4d);
        if (flag != 0) {
            measure = *(volatile s32 *)(entity + 0x3c);
            if (measure > 0x6b6c)
                return remainder != 0 ? 0 : 2;
            return remainder >= 4 ? 0 : 2;
        }
        if (remainder >= 3)
            return 0;
        return 2;
    }
    if (code == 4) {
        month = *(volatile s8 *)(date + 2);
        if (month < 1 || month >= 0x1d)
            goto fallback;
        flag = *(volatile u8 *)(entity + 0x4d);
        if (flag != 0) {
            measure = *(volatile s32 *)(entity + 0x3c);
            if (measure <= 0x6b6c)
                return 2;
            return remainder >= 3 ? 0 : 2;
        }
        measure = *(volatile s32 *)(entity + 0x3c);
        return measure > 0x445c ? 2 : 1;
    }

fallback:
    month = *(volatile s8 *)(date + 2);
    if (month < 1 && (code == 1 || code == 8)) {
        flag = *(volatile u8 *)(entity + 0x4d);
        if (flag != 0)
            return 2;
        measure = *(volatile s32 *)(entity + 0x3c);
        if (measure > 0xafc8)
            return 2;
        return measure <= 0x30d4 ? 0x10 : 1;
    }
    if (code == 9) {
        flag = *(volatile u8 *)(entity + 0x4d);
        if (flag != 0) {
            measure = *(volatile s32 *)(entity + 0x3c);
            if (measure > 0x7530)
                return 2;
            return 1;
        }
        measure = *(volatile s32 *)(entity + 0x3c);
        return measure <= 0x4e20 ? 0x20 : 1;
    }
    if (code == 0 && month >= 0x1d) {
        measure = *(volatile s32 *)(entity + 0x3c);
        return measure <= 0x88b8 ? 0x40 : 4;
    }
    return 0;
}
