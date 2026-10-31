/* Preferred-image VA 006a0990, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_007875e0(u32 self, u32 record);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 kind, u32 zero);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32 self, void *message, u32 record);

u32 CM_STDCALL cm_fn_006a0990(u32 first, u32 second, u32 third)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0xe8];
    u32 previous, result, receiver, low, high, pointer, value;
    u16 year;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095cdabu;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    result = previous;
    if (first == 0 || second == 0)
        goto done;
    receiver = D(0x00b7431cu);
    result = cm_fn_007875e0(receiver, second);
    if (result == 0)
        goto done;
    low = D(0x00dd6ec8u);
    high = D(0x00dd6eccu);
    year = W(0x00ae2c92u);
    D((u32)work) = low;
    D((u32)work + 4u) = high;
    result = low;
    if ((u16)(low >> 16) > year)
        goto done;
    if ((u16)(low >> 16) == year) {
        result = (s16)low > (s16)W(0x00ae2c90u);
        if (result != 0)
            goto done;
    }
    cm_fn_0077bbe0(work + 8, 0x1778u, 0);
    value = D(first);
    frame.state = 0;
    cm_fn_00785790(work + 8, 0, value);
    pointer = D(first + 4u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(work + 8, 1, value);
    pointer = D(first + 8u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(work + 8, 2, value);
    pointer = D(first + 12u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(work + 8, 3, value);
    value = D(second);
    cm_fn_00785790(work + 8, 4, value);
    value = (u32)(s32)(s16)third;
    cm_fn_00785790(work + 8, 5, value);
    receiver = D(0x00b7431cu);
    result = (u32)cm_fn_007861e0(receiver, work + 8, second);
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
