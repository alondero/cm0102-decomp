#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(u32 *self, u32 *output, u16 *source);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(
    void *self, u32 first, u32 second);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);
extern u32 CM_CDECL cm_fn_00946464(
    u32 output, u32 first, u32 second, s32 size, const void *text);

void CM_THISCALL cm_fn_004a22f0(volatile u8 *owner, volatile u8 *destination)
{
    volatile u8 scratch[0x2c];
    u32 result;
    u32 date_result;
    u32 first;
    u32 second;
    u32 count;
    u32 destination_initial;
    s32 converted;
    u16 first_word;
    u16 second_word;

    first = *(volatile u32 *)owner;
    if (first == 0)
        return;
    second = *(volatile u32 *)(owner + 8);
    destination_initial = *(volatile u32 *)destination;
    *(volatile u32 *)(scratch + 0x10) = destination_initial;
    result = cm_fn_00946464((u32)(unsigned long)(scratch + 0x10), first,
                            second, 0x1c, (const void *)0x008b4e00);
    if (result == 0)
        return;

    converted = (s32)*(volatile s16 *)(result + 4);
    *(volatile s32 *)(destination + 0x19) = converted;

    second_word = *(volatile u16 *)(result + 8);
    first_word = *(volatile u16 *)(result + 6);
    date_result = (u32)cm_fn_00549ef0((void *)scratch, first_word, second_word);
    cm_fn_00417c10((u32 *)(destination + 0x1d), (u32 *)(scratch + 8),
                   (u16 *)date_result);
    cm_fn_00417c10((u32 *)(destination + 0x25), (u32 *)(scratch + 8),
                   (u16 *)0x00ae2c90);

    converted = (s32)*(volatile s16 *)(result + 0x0a);
    *(volatile s32 *)(destination + 0x2d) = converted;
    first_word = *(volatile u16 *)(result + 0x0e);
    second_word = *(volatile u16 *)(result + 0x0c);
    date_result = (u32)cm_fn_00549ef0((void *)(scratch + 8),
                                      second_word, first_word);
    cm_fn_00417c10((u32 *)(destination + 0x31), (u32 *)scratch,
                   (u16 *)date_result);
    cm_fn_00417c10((u32 *)(destination + 0x39), (u32 *)(scratch + 8),
                   (u16 *)0x00ae2c90);

    converted = (s32)*(volatile s16 *)(result + 0x10);
    *(volatile s32 *)(destination + 0x41) = converted;
    first_word = *(volatile u16 *)(result + 0x14);
    second_word = *(volatile u16 *)(result + 0x12);
    date_result = (u32)cm_fn_00549ef0((void *)(scratch + 8),
                                      second_word, first_word);
    cm_fn_00417c10((u32 *)(destination + 0x45), (u32 *)scratch,
                   (u16 *)date_result);
    cm_fn_00417c10((u32 *)(destination + 0x4d), (u32 *)(scratch + 8),
                   (u16 *)0x00ae2c90);

    converted = (s32)*(volatile s16 *)(result + 0x16);
    *(volatile s32 *)(destination + 0x55) = converted;
    first_word = *(volatile u16 *)(result + 0x1a);
    second_word = *(volatile u16 *)(result + 0x18);
    date_result = (u32)cm_fn_00549ef0((void *)(scratch + 8),
                                      second_word, first_word);
    cm_fn_00417c10((u32 *)(destination + 0x59), (u32 *)scratch,
                   (u16 *)date_result);
    cm_fn_00417c10((u32 *)(destination + 0x61), (u32 *)(scratch + 8),
                   (u16 *)0x00ae2c90);

    count = *(volatile u32 *)(owner + 8);
    count -= 1u;
    *(volatile u32 *)(owner + 8) = count;
    if (count == 0) {
        first = *(volatile u32 *)owner;
        (void)cm_fn_009452ca(first);
        *(volatile u32 *)owner = 0;
        *(volatile u32 *)(owner + 4) = 0;
    }
}
