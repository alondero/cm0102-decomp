/* Preferred-image VA005a74b0, base00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve native lookup, notification and EDX:EAX RNG boundaries. */
#include "cmre.h"
#define R8(a) CM_GLOBAL(volatile u8, (u32)(a))
#define R16(a) CM_GLOBAL(volatile u16, (u32)(a))
#define R32(a) CM_GLOBAL(volatile u32, (u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);
extern u32 CM_THISCALL cm_fn_007875e0(u32 physical_receiver, u32 club);
extern u32 CM_THISCALL cm_fn_008cca10(u32, u32, u32);
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
    cm_fn_0090cfc0(u32 incoming_eax, u32 incoming_edx, u32 bound);

u32 CM_STDCALL cm_fn_005a74b0(volatile u32 person_arg, volatile u32 second_arg,
                            volatile u32 third_arg, volatile u32 fourth_arg,
                            volatile u32 fifth_arg)
{
    struct {
        u8 news[224];
        volatile struct { u32 prev, handler, state; } seh;
    } local;
    u32 previous = CM_FS(u32, 0);
    local.seh.state = 0xffffffffu;
    local.seh.handler = 0x009593ebu;
    local.seh.prev = previous;
    CM_FS(u32, 0) = (u32)&local.seh;
    u32 eax = previous;
    u32 person = person_arg;
    u32 receiver, id, base, club, record, value;
    if (person == 0)
        goto done;
    receiver = R32(0x00ae1050u);
    eax = cm_fn_004cf5a0(receiver, person);
    R32(0x00ae1080u) = eax;
    if (eax == 0)
        goto done;
    eax = R32(eax + 4u);
    id = eax;
    base = R32(0x00ae23b4u);
    club = base + id * 581u;
    if (club == 0)
        goto done;
    receiver = R32(0x00ae1050u);
    eax = cm_fn_004cf5f0(receiver, person, club);
    record = eax;
    if (record == 0)
        goto done;
    eax = R32(person + 0x39u);
    if (eax != 0) {
        receiver = R32(0x00b7431cu);
        eax = cm_fn_007875e0(receiver, eax);
        if (eax == 0)
            goto done;
    }
    cm_fn_0077bbe0(local.news, 0xbc6u, 0);
    value = R32(person);
    local.seh.state = 0;
    cm_fn_00785790(local.news, 0, value);
    value = R32(person + 4u);
    value = R32(value + 0x33u);
    cm_fn_00785790(local.news, 1u, value);
    value = R32(person + 8u);
    value = R32(value + 0x33u);
    cm_fn_00785790(local.news, 2u, value);
    value = R32(person + 0x0cu);
    value = R32(value + 0x33u);
    cm_fn_00785790(local.news, 3u, value);
    value = (u32)(s32)*(volatile s8 *)&second_arg;
    cm_fn_00785790(local.news, 4u, value);
    value = (u32)(s32)*(volatile s8 *)&third_arg;
    unsigned long long wide = cm_fn_00785790(local.news, 5u, value);
    wide = cm_fn_0090cfc0((u32)wide, (u32)(wide >> 32), 2u);
    cm_fn_00785790(local.news, 6u, (u32)wide);
    s32 sum = (s8)R8(person + 0x5du);
    sum += (s8)R8(person + 0x5bu);
    value = 0;
    if (sum < 12 && (s8)R8(person + 0x59u) < 6) {
        id = R32(person + 0x61u);
        if ((s16)R16(id + 0x0bu) > 0x1482)
            value = 1u;
    }
    cm_fn_00785790(local.news, 7u, value);
    value = *(volatile u8 *)&fourth_arg == 1u;
    cm_fn_00785790(local.news, 8u, value);
    value = (R8(record + 0x4fu) & 0xf0u) == 0x70u;
    cm_fn_00785790(local.news, 9u, value);
    receiver = R32(0x00de1f54u);
    eax = cm_fn_008cca10(receiver, person, 0);
    cm_fn_00785790(local.news, 10u, eax != 0);
    value = (u32)(s32)*(volatile s8 *)&fifth_arg;
    cm_fn_00785790(local.news, 11u, value);
    value = 0;
    if ((s8)R8(person + 0x18u) > 23 && (R8(record + 0x4fu) & 0xf0u) == 0x10u)
        value = 1u;
    else if ((R8(record + 0x4fu) & 0xf0u) == 0x20u)
        value = 1u;
    cm_fn_00785790(local.news, 12u, value);
    receiver = R32(0x00b7431cu);
    eax = (u32)cm_fn_007861e0((void *)receiver, (u32)local.news, club);
done:
    CM_FS(u32, 0) = local.seh.prev;
    return eax;
}
