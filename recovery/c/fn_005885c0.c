/* Preferred-image VA 005885c0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve incidental EAX, including the initial FS predecessor on null input. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_007875e0(u32 self, u32 club);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 first, u32 second);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32 self, u32 report, u32 club);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 limit);

u32 CM_CDECL cm_fn_005885c0(u32 input)
{
    u32 predecessor = CM_FS(volatile u32, 0);
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0xe0];
    u32 result = predecessor, receiver, pointer, value, club;
    frame.state = 0xffffffffu;
    frame.handler = 0x00958bbbu;
    frame.prev = predecessor;
    CM_FS(volatile u32, 0) = (u32)&frame;
    if (input == 0)
        goto done;
    result = D(input + 0x61u);
    if (result == 0)
        goto done;
    result = D(input + 0x39u);
    if (result == 0)
        goto done;
    receiver = D(0x00b7431cu);
    result = cm_fn_007875e0(receiver, result);
    if (result == 0)
        goto done;
    cm_fn_0077bbe0(work, 0xbcdu, 0);
    value = D(input);
    frame.state = 0;
    cm_fn_00785790(work, 0, value);
    pointer = D(input + 4u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(work, 1, value);
    pointer = D(input + 8u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(work, 2, value);
    pointer = D(input + 0xcu);
    value = D(pointer + 0x33u);
    cm_fn_00785790(work, 3, value);
    pointer = D(input + 0x39u);
    value = D(pointer);
    cm_fn_00785790(work, 4, value);
    value = (u32)cm_fn_0090cfc0(3);
    cm_fn_00785790(work, 5, value);
    club = D(input + 0x39u);
    receiver = D(0x00b7431cu);
    result = (u32)cm_fn_007861e0(receiver, (u32)work, club);
done:
    predecessor = frame.prev;
    CM_FS(volatile u32, 0) = predecessor;
    return result;
}
