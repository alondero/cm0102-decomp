#include "cmre.h"

/* Preferred-image VA00487c70, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/fullEAX; SEH955fbb, scratchec: savedBYTE L3, WORDsL4/L6/L8/La,
   messageLc. 48aff0 THIS4 RET16 initializes both WORD outputs;7bd9d0
   initializes its two WORD outputs before every returning branch.
   Only BL is later observed from the original partial-register counter. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_0054cdb0(u32, u32, u32);
extern u8 CM_STDCALL cm_fn_0048af30(u32);
extern void CM_THISCALL cm_fn_0048aff0(u32, u32, u32, u32, u32);
extern u8 CM_CDECL cm_fn_007bd9d0(u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00546ab0(u32, u32);
extern u32 CM_CDECL cm_fn_005470d0(u32, u32);
extern u16 CM_CDECL cm_fn_005b7d20(u32);
extern u32 CM_THISCALL cm_fn_008cca10(u32, u32, u32);
extern u8 CM_THISCALL cm_fn_00418f60(u32, u32, u32);

u32 CM_THISCALL cm_fn_00487c70(u32 self, u32 first,
                              volatile u32 second_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0xec];
    u32 l = (u32)scratch, message = l + 0xc;
    u32 eax, second, low, high, threshold, pointer, value, owner;
    u8 kind;
    frame.state = 0xffffffffu; frame.handler = 0x00955fbb;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    eax = frame.prev;
    if (first == 0) goto done;
    eax = D(first + 0x39);
    if (eax == 0) goto done;
    second = second_arg;
    if (second == 0) goto done;
    eax = D(second + 0x39);
    if (eax == 0) goto done;
    high = D(second + 0x42);
    low = D(second + 0x3e);
    eax = cm_fn_0054cdb0(0x00ae2c90, low, high);
    threshold = 21u - (u32)(s32)(s8)B(second + 0x56);
    if ((s32)eax > 1000 && (s8)B(second + 0x59) > 10) kind = 1;
    else {
        threshold = threshold * 5u + 90u;
        kind = (s32)eax >= (s32)threshold ? 4 : 2;
    }
    B(l + 3) = cm_fn_0048af30(second);
    cm_fn_0048aff0(self, first, second, l + 0xa, l + 4);
    cm_fn_007bd9d0(second, first, l + 6, l + 8);
    cm_fn_0077bbe0(message, 0x1391, 0);
    value = D(first);
    frame.state = 0;
    cm_fn_00785790(message, 0, value);
    pointer = D(first + 4); value = D(pointer + 0x33);
    cm_fn_00785790(message, 1, value);
    pointer = D(first + 8); value = D(pointer + 0x33);
    cm_fn_00785790(message, 2, value);
    pointer = D(first + 0xc); value = D(pointer + 0x33);
    cm_fn_00785790(message, 3, value);
    value = D(second);
    cm_fn_00785790(message, 4, value);
    pointer = D(second + 4); value = D(pointer + 0x33);
    cm_fn_00785790(message, 5, value);
    pointer = D(second + 8); value = D(pointer + 0x33);
    cm_fn_00785790(message, 6, value);
    pointer = D(second + 0xc); value = D(pointer + 0x33);
    cm_fn_00785790(message, 7, value);
    cm_fn_00785790(message, 8, (u32)(s32)(s8)kind);
    value = (u32)(s32)(s8)B(l + 3);
    cm_fn_00785790(message, 9, value);
    value = (u32)(s32)(s16)W(l + 0xa);
    cm_fn_00785790(message, 10, value);
    value = (u32)(s32)(s16)W(l + 4);
    cm_fn_00785790(message, 11, value);
    value = (u32)(s32)(s16)W(l + 6);
    cm_fn_00785790(message, 12, value);
    value = (u32)(s32)(s16)W(l + 8);
    cm_fn_00785790(message, 13, value);
    pointer = D(first + 0x39);
    value = cm_fn_00546ab0(second, pointer);
    cm_fn_00785790(message, 14, value);
    pointer = D(first + 0x39);
    value = cm_fn_005470d0(second, pointer);
    cm_fn_00785790(message, 15, value);
    pointer = D(second + 0x61);
    value = (u32)(s32)(s16)cm_fn_005b7d20(pointer);
    cm_fn_00785790(message, 16, value);
    value = (u32)(s32)(s8)B(second + 0x18);
    cm_fn_00785790(message, 17, value);
    pointer = D(second + 0x39);
    value = (u32)(s32)(s16)W(pointer + 0x80);
    cm_fn_00785790(message, 18, value);
    pointer = D(first + 0x39);
    value = D(pointer);
    cm_fn_00785790(message, 19, value);
    owner = D(0x00de1f54);
    value = cm_fn_008cca10(owner, second, 0);
    cm_fn_00785790(message, 20, value != 0);
    owner = D(0x00ad9c6c);
    value = cm_fn_00418f60(owner, second, 0);
    cm_fn_00785790(message, 21, value != 0);
    pointer = D(second + 0x69);
    cm_fn_00785790(message, 22, pointer != 0);
    pointer = D(first + 0x39);
    owner = D(0x00b7431c);
    eax = (u32)cm_fn_007861e0(owner, message, pointer);
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
