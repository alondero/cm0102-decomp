/* Preferred-image VA008e1150, base00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve inherited ECX into the first collector call and the native null
   DWORD read on the second-lookup failure. Original helpers remain boundaries. */
#include "cmre.h"
#define R8(a) CM_GLOBAL(volatile u8, (u32)(a))
#define R32(a) CM_GLOBAL(volatile u32, (u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);
/* Root's no-interest return forwards full native EAX. The recovered collector
   declares s8; reconcile that return contract before selected C binding. */
extern u32 CM_THISCALL cm_fn_008d1130(u32, u32, u32, u32, s8);
extern u32 CM_THISCALL cm_fn_008e7a70(u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_008e1150(u32 inherited_receiver, volatile u32 offer_arg,
                             volatile u32 other_arg)
{
    struct {
        u8 news[224];
        u32 interested[30];
        volatile struct { u32 prev, handler, state; } seh;
    } local;
    u32 previous = CM_FS(u32, 0);
    local.seh.state = 0xffffffffu;
    local.seh.handler = 0x009646ebu;
    local.seh.prev = previous;
    CM_FS(u32, 0) = (u32)&local.seh;
    u32 offer = offer_arg;
    u32 index = R32(offer + 0x0cu);
    u32 base = R32(0x00ae23bcu);
    u32 person = base + index * 110u;
    u32 eax = cm_fn_008d1130(inherited_receiver, offer, 15u,
                            (u32)local.interested, 30);
    s8 count = (s8)eax;
    u32 value, receiver, id;
    if (count == 0)
        goto done;
    cm_fn_0077bbe0(local.news, 0xfb4u, 0);
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
    value = R32(offer + 8u);
    cm_fn_00785790(local.news, 4u, value);

    receiver = R32(0x00ae1050u);
    eax = cm_fn_004cf5a0(receiver, person);
    R32(0x00ae1080u) = eax;
    if (eax != 0 && (R8(eax + 0x35u) & 0x3fu) != 5u) {
        id = R32(eax + 4u);
        base = R32(0x00ae23b4u);
        eax = base + id * 581u;
        if (eax != 0) {
            receiver = R32(0x00ae1050u);
            eax = cm_fn_004cf5a0(receiver, person);
            R32(0x00ae1080u) = eax;
            if (eax != 0 && (R8(eax + 0x35u) & 0x3fu) != 5u) {
                id = R32(eax + 4u);
                base = R32(0x00ae23b4u);
                eax = base + id * 581u;
                value = R32(eax);
            } else {
                eax = 0;
                /* Retain the native continuation even if address zero is mapped. */
                volatile u32 null_address = eax;
                value = R32(null_address); /* Native008e1289/008e128b. */
            }
        } else {
            value = 0xffffffffu;
        }
    } else {
        value = 0xffffffffu;
    }
    cm_fn_00785790(local.news, 5u, value);
    value = R32(offer + 0x1bu);
    cm_fn_00785790(local.news, 6u, value);
    value = (u32)(s32)(s8)R8(offer + 0x2fu);
    cm_fn_00785790(local.news, 7u, value);
    value = cm_fn_008e7a70(offer, 0, 0, 0, 1u);
    cm_fn_00785790(local.news, 8u, value);
    value = other_arg;
    value = R32(value);
    cm_fn_00785790(local.news, 9u, value);
    value = (u32)(s32)(s8)R8(offer + 0x2cu);
    cm_fn_00785790(local.news, 10u, value);
    value = R32(offer + 0x20u);
    cm_fn_00785790(local.news, 11u, value);
    value = R32(offer + 0x24u);
    cm_fn_00785790(local.news, 12u, value);
    value = (u32)(s32)(s8)R8(offer + 0x1fu);
    eax = (u32)cm_fn_00785790(local.news, 13u, value);
    if (count > 0) {
        /* Native signed count is not clamped to the thirty-word input buffer. */
        for (u32 n = 0; n < (u32)(s32)count; ++n) {
            value = R32((u32)local.interested + n * 4u);
            receiver = R32(0x00b7431cu);
            eax = (u32)cm_fn_007861e0((void *)receiver, (u32)local.news, value);
        }
    }
done:
    CM_FS(u32, 0) = local.seh.prev;
    return eax;
}
