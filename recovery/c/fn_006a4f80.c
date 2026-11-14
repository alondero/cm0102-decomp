/* Preferred-image VA006a4f80, base00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original callees and native exception/unwind delivery remain boundaries. */
#include "cmre.h"
#define R8(a) CM_GLOBAL(volatile u8, (u32)(a))
#define R16(a) CM_GLOBAL(volatile u16, (u32)(a))
#define R32(a) CM_GLOBAL(volatile u32, (u32)(a))
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern long long CM_THISCALL cm_fn_00598800(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_005a0590(u32, u32);
extern u32 CM_THISCALL cm_fn_006a3770(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785d40(void *, u32, u32, u32);
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
    cm_fn_0090cfc0(u32 incoming_eax, u32 incoming_edx, u32 bound);

u32 CM_THISCALL cm_fn_006a4f80(u32 self, volatile u32 person_arg,
                             volatile u32 replacement_arg)
{
    /* Scratch L..L+df and the linked frame at L+e0 preserve adjacency.
       Constructor0077bbe0 initializes the scratch; no C initialization is added. */
    struct {
        u8 scratch[224];
        volatile struct { u32 prev, handler, state; } seh;
    } local;
    u32 person, id, receiver, table, offset, old_value, value, result;
    u8 saved_bit, current;
    unsigned long long wide;

    local.seh.state = 0xffffffffu;
    local.seh.handler = 0x0095ce8bu;
    local.seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&local.seh;
    cm_fn_0077bbe0(local.scratch, 0x1774u, 0);
    person = person_arg;
    local.seh.state = 0;
    id = R32(person);
    cm_fn_00785790(local.scratch, 0, id);
    wide = cm_fn_00785790(local.scratch, 1u, 0);
    result = (u32)wide;
    if (R32(person + 0xcfu) == 0) {
        if (cm_fn_0053b900((u32 *)person) == 0) {
            receiver = R32(0x00ae2a38u);
            receiver = cm_fn_005a0590(receiver, person);
            id = R32(person);
            table = R32(self);
            offset = id * 73u;
            R32(table + offset + 0x38u) = 0;
            R32(table + offset + 0x3cu) = 0;
            value = (u32)cm_fn_00598800(receiver, 0, 0, 0, 1u, 0);
            cm_fn_00785790(local.scratch, 2u, value);

            id = R32(person);
            table = R32(self);
            old_value = R32(person + 0xcfu);
            value = replacement_arg;
            offset = id * 73u;
            saved_bit = R8(table + offset + 2u);
            R32(person + 0xcfu) = value;
            table = R32(self);
            current = R8(table + offset + 2u);
            R8(table + offset + 2u) = current | 1u;
            cm_fn_006a3770(self, person, 0);

            id = R32(person);
            R32(person + 0xcfu) = old_value;
            table = R32(self);
            offset = id * 73u;
            current = R8(table + offset + 2u);
            R8(table + offset + 2u) = (current & 0xfeu) | (saved_bit & 1u);

            id = R32(person);
            table = R32(self);
            value = (u32)(s32)(s16)R16(table + id * 73u + 0x0eu);
            wide = cm_fn_00785790(local.scratch, 3u, value);
            /* Original0090cfc0 consumes the actual EDX:EAX returned by setter
               00785790, plus one stack word. Capture both physical halves. */
            wide = cm_fn_0090cfc0((u32)wide, (u32)(wide >> 32), 2u);
            cm_fn_00785790(local.scratch, 4u, (u32)wide);
        } else {
            cm_fn_00785790(local.scratch, 2u, 0);
            cm_fn_00785790(local.scratch, 3u, 0xffffffffu);
            cm_fn_00785790(local.scratch, 4u, 0);
        }
        value = replacement_arg;
        receiver = R32(0x00b7431cu);
        wide = cm_fn_00785d40((void *)receiver, (u32)local.scratch, value, 1u);
        result = (u32)wide;
    }
    CM_FS(u32, 0) = local.seh.prev;
    return result;
}
