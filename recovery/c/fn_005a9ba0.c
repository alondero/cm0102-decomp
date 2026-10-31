#include "cmre.h"

extern short *CM_THISCALL cm_fn_00549f70(short *self, short a, char b,
                                          u16 c, u32 d);
extern char *CM_CDECL cm_fn_0066f4e0(u32 value, char *text);

struct cm_seh_005a9ba0 { void *prev; u32 handler; u32 state; };

u32 CM_STDCALL cm_fn_005a9ba0(s32 arg1, s32 arg2, u32 message_value,
                               volatile s16 *date, u32 unused_arg5)
{
    volatile u8 local[0x18];
    volatile struct cm_seh_005a9ba0 frame;
    u32 saved_eax;
    u32 argument;
    u32 result_address;
    u32 first;
    u32 second;
    u16 computed_high;
    u16 requested_high;
    s16 computed_low;
    s16 requested_low;
    u32 compare_result;
    u32 return_value;

    (void)unused_arg5;
    frame.state = 0xffffffffu;
    frame.handler = 0x00959568u;
    saved_eax = (u32)CM_FS(void *, 0);
    frame.prev = (void *)saved_eax;
    CM_FS(void *, 0) = (void *)&frame;

    requested_high = *(volatile u16 *)(date + 1);
    argument = (saved_eax & 0xffff0000u) | requested_high;
    result_address = (u32)cm_fn_00549f70((short *)(local + 8), 0x12, 7,
                                         (u16)argument, 0xffffffffu);
    first = *(volatile u32 *)result_address;
    second = *(volatile u32 *)(result_address + 4);
    *(volatile u32 *)(local + 0) = first;
    computed_high = *(volatile u16 *)(local + 2);
    *(volatile u32 *)(local + 4) = second;
    requested_high = *(volatile u16 *)(date + 1);
    computed_low = (s16)(u16)first;
    frame.state = 0;
    if (computed_high < requested_high) {
        compare_result = 1;
    } else if (computed_high > requested_high) {
        compare_result = 0;
    } else {
        requested_low = *date;
        compare_result = computed_low < requested_low;
    }
    if (compare_result != 0) {
        argument = (second & 0xffff0000u) | requested_high;
        result_address = (u32)cm_fn_00549f70((short *)(local + 0x10), 0x0f,
                                             10, (u16)argument, 0xffffffffu);
        first = *(volatile u32 *)result_address;
        second = *(volatile u32 *)(result_address + 4);
        *(volatile u32 *)(local + 0) = first;
        computed_high = *(volatile u16 *)(local + 2);
        *(volatile u32 *)(local + 4) = second;
        requested_high = *(volatile u16 *)(date + 1);
        computed_low = (s16)(u16)first;
        if (computed_high > requested_high) {
            compare_result = 1;
        } else if (computed_high < requested_high) {
            compare_result = 0;
        } else {
            requested_low = *date;
            compare_result = computed_low > requested_low;
        }
    }

    frame.state = 0xffffffffu;
    if (compare_result == 0) {
        return_value = 1;
    } else if (message_value == 0) {
        return_value = 0;
    } else if (arg1 == *(volatile s32 *)((volatile u8 *)arg2 + 0x39)) {
        cm_fn_0066f4e0(message_value, (char *)0x009cc1a4);
        return_value = 0;
    } else {
        cm_fn_0066f4e0(message_value, (char *)0x009cc170);
        return_value = 0;
    }

    CM_FS(void *, 0) = frame.prev;
    return return_value;
}
