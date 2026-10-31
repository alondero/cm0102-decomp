#include "cmre.h"

/* Preferred-image VA 0x005c4380, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   54c5b0/54c8e0 consume the offset's low WORD (including their mutual
   negative forwarding). The incoming ESI high WORD is not consumed.
   The original second/third argument slots become retry/allocation and
   success locals; their object pointers stay cached in EDI/EBP. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_005c45a0(u32, u32);
extern u32 CM_THISCALL cm_fn_005c4b60(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32, u32, s16);

static void ensure_list(u32 field, volatile u32 *state, u8 phase)
{
    u32 value = D(field);
    if (value == 0) {
        value = cm_fn_00944cf1(14);
        *(volatile u8 *)state = phase;
        if (value != 0) value = cm_fn_0068d020(value);
        *(volatile u8 *)state = 0;
        D(field) = value;
    }
}

u32 CM_THISCALL cm_fn_005c4380(u32 self, u32 first, u32 second, u32 third)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[8];
    u32 local = (u32)scratch, previous, result, value, random, success = 0;
    u16 start, end, other;
    u8 attempts = 0, byte;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00959fe4;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    other = W(third + 14);
    start = W(second + 14);
    end = W(second + 16);
    if (other != 0xffffu) {
        if ((s16)other > (s16)start) start = other;
        other = W(third + 16);
        if ((s16)other < (s16)end) end = other;
    }
    if ((s16)start >= (s16)end) { result = 0; goto done; }
    random = cm_fn_0090cfc0((u32)((s32)(s16)end - (s32)(s16)start));
    value = (u16)(start + random);
    cm_fn_0054c5b0(self + 0xa7, local, value);
    frame.state = 0;
    for (;;) {
        if (attempts == 5) {
            if (success != 0) break;
            result = 0; goto done;
        }
        value = cm_fn_005c45a0(second, local);
        if (value == 0) {
            value = cm_fn_005c45a0(third, local);
            if (value == 0) success = 1;
        }
        attempts = (u8)(attempts + 1u);
        if (success != 0) break;
    }
    cm_fn_005c4b60(self, first, second, third, local);
    ensure_list(second + 0x12, &frame.state, 1);
    ensure_list(third + 0x12, &frame.state, 2);
    value = D(second + 0x12);
    cm_fn_0068d0d0(value, local, 8);
    value = D(third + 0x12);
    cm_fn_0068d0d0(value, local, 8);
    ensure_list(second + 10, &frame.state, 3);
    value = D(second + 10);
    cm_fn_0068d0d0(value, third, 4);
    ensure_list(third + 10, &frame.state, 4);
    value = D(third + 10);
    cm_fn_0068d0d0(value, second, 4);
    byte = B(second + 4);
    B(second + 4) = (u8)(byte + 1u);
    byte = B(third + 4);
    B(third + 4) = (u8)(byte + 1u);
    result = 1;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
