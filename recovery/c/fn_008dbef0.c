/* Preferred-image VA008dbef0, base00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original callees remain boundaries. Signed counters wrap without clamping. */
#include "cmre.h"
#define R8(a) CM_GLOBAL(volatile u8, (u32)(a))
#define R16(a) CM_GLOBAL(volatile u16, (u32)(a))
#define R32(a) CM_GLOBAL(volatile u32, (u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_CDECL cm_fn_00546ab0(u32, u32);
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
    cm_fn_005b7d20(u32, u32, const u8 *);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
/* Original785790 consumes its index only as signed BYTE, including diagnostics;
   it never forwards the argument address. Full native EBX upper24 are dead at
   the final setter, whose popped index is overwritten by the next CALL word.
   Arbitrary pointers into native outgoing frames remain outside this proof. */
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00786780(void *, u32);
extern u32 CM_THISCALL cm_fn_007875e0(u32, u32);
/* Full native EAX is forwarded when collector AL is zero. */
extern u32 CM_THISCALL cm_fn_008d1130(u32, u32, u32, u32, s8);
extern u32 CM_THISCALL cm_fn_008e7a70(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008e7ef0(u32);
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
    cm_fn_0090cfc0(u32, u32, u32);

u32 CM_THISCALL cm_fn_008dbef0(u32 inherited_receiver, volatile u32 offer_arg)
{
    struct {
        union { volatile u8 bytes[16]; volatile u32 words[4]; } work;
        u8 news[224];
        u32 interested[30];
        volatile struct { u32 prev, handler, state; } seh;
    } local;
    local.seh.state = 0xffffffffu;
    local.seh.handler = 0x0096455bu;
    local.seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&local.seh;
    u32 offer = offer_arg;
    local.work.bytes[2] = 0;
    u32 index = R32(offer + 0x0cu);
    u32 base = R32(0x00ae23bcu);
    u32 person = base + index * 110u;
    u32 eax = cm_fn_008d1130(inherited_receiver, offer, 0x5fu,
                            (u32)local.interested, 30);
    local.work.bytes[3] = (u8)eax;
    u32 receiver, id, value, record, club, item;
    unsigned long long wide;
    if ((u8)eax == 0)
        goto done;
    cm_fn_0077bbe0(local.news, 0xfa8u, 0);
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
    value = 0xffffffffu;
    if (eax != 0 && (R8(eax + 0x35u) & 0x3fu) != 5u) {
        id = R32(eax + 4u);
        base = R32(0x00ae23b4u);
        club = base + id * 581u;
        if (club != 0) {
            receiver = R32(0x00ae1050u);
            eax = cm_fn_004cf5a0(receiver, person);
            R32(0x00ae1080u) = eax;
            if (eax != 0 && (R8(eax + 0x35u) & 0x3fu) != 5u) {
                id = R32(eax + 4u);
                base = R32(0x00ae23b4u);
                club = base + id * 581u;
                value = R32(club);
            } else {
                volatile u32 null_address = 0;
                value = R32(null_address); /* Native8dc031/8dc033 continues if mapped. */
            }
        }
    }
    cm_fn_00785790(local.news, 5u, value);
    value = R32(offer + 0x1bu);
    cm_fn_00785790(local.news, 6u, value);
    cm_fn_00785790(local.news, 7u, 0);
    value = cm_fn_008e7a70(offer, 1u, 1u, 0, 1u);
    cm_fn_00785790(local.news, 8u, value);
    value = (u32)(s32)(s8)R8(offer + 0x2fu);
    cm_fn_00785790(local.news, 9u, value);
    value = (u32)(s32)(s8)R8(offer + 0x11u);
    cm_fn_00785790(local.news, 12u, value);
    value = cm_fn_008e7ef0(offer);
    cm_fn_00785790(local.news, 13u, value);

    if (R8(offer + 0x2fu) != 0) {
        receiver = R32(0x00ae1050u);
        eax = cm_fn_004cf5a0(receiver, person);
        R32(0x00ae1080u) = eax;
        if (eax != 0 && (R8(eax + 0x35u) & 0x3fu) != 5u) {
            id = R32(eax + 4u);
            base = R32(0x00ae23b4u);
            club = base + id * 581u;
            if (club != 0 && (R8(offer + 0x30u) & 0x0fu) == 3u) {
                receiver = R32(0x00ae1050u);
                record = cm_fn_004cf5a0(receiver, person);
                u32 has_future = 0;
                if (record != 0) {
                    u32 low = R32(0x00ae2c90u);
                    u32 high = R32(0x00ae2c94u);
                    local.work.words[2] = low;
                    u16 year = R16((u32)&local.work + 10u);
                    local.work.words[3] = high;
                    u16 target_year = R16(record + 0x2fu);
                    if (year < target_year)
                        has_future = 1u;
                    else if (year == target_year && (s16)low < (s16)R16(record + 0x2du))
                        has_future = 1u;
                }
                if (has_future) {
                    value = (u32)(s32)(s16)R16(record + 0x2du);
                    cm_fn_00785790(local.news, 10u, value);
                    value = R16(record + 0x2fu);
                } else {
                    cm_fn_00785790(local.news, 10u, 0);
                    value = 0;
                }
                cm_fn_00785790(local.news, 11u, value);
            }
        }
    }
    u8 slot = 15;
    receiver = R32(offer + 4u);
    if (receiver != 0) {
        value = R32(receiver + 2u);
        R32(receiver + 10u) = value;
        receiver = R32(offer + 4u);
        item = cm_fn_0068d1b0(receiver);
        while (item != 0) {
            index = R32(item);
            base = R32(0x00ae23bcu);
            u32 other_person = base + index * 110u;
            if ((s8)local.work.bytes[2] < 3) {
                value = R32(other_person);
                cm_fn_00785790(local.news, slot, value);
                ++slot;
                value = R32(other_person + 4u);
                value = R32(value + 0x33u);
                cm_fn_00785790(local.news, slot, value);
                ++slot;
                value = R32(other_person + 8u);
                value = R32(value + 0x33u);
                cm_fn_00785790(local.news, slot, value);
                ++slot;
                value = R32(other_person + 0x0cu);
                value = R32(value + 0x33u);
                cm_fn_00785790(local.news, slot, value);
                ++slot;
            }
            u8 count = local.work.bytes[2];
            receiver = R32(offer + 4u);
            local.work.bytes[2] = (u8)(count + 1u);
            item = cm_fn_0068d1b0(receiver);
        }
    }
    value = (u32)(s32)(s8)local.work.bytes[2];
    wide = cm_fn_00785790(local.news, 14u, value);
    local.work.bytes[8] = slot;
    ++slot;
    local.work.bytes[4] = slot;
    wide = cm_fn_0090cfc0((u32)wide, (u32)(wide >> 32), 3u);
    value = local.work.words[2];
    cm_fn_00785790(local.news, value, (u32)wide);
    eax = R32(person + 0x61u);
    if (eax != 0) {
        value = (eax & 0xffffff00u) | slot;
        ++slot;
        local.work.bytes[4] = slot;
        cm_fn_00785790(local.news, value, 1u);
        receiver = R32(0x00ae1050u);
        eax = cm_fn_004cf5a0(receiver, person);
        R32(0x00ae1080u) = eax;
        club = 0;
        if (eax != 0 && (R8(eax + 0x35u) & 0x3fu) != 5u) {
            id = R32(eax + 4u);
            base = R32(0x00ae23b4u);
            club = base + id * 581u;
        }
        value = cm_fn_00546ab0(person, club);
        index = local.work.words[1];
        wide = cm_fn_00785790(local.news, index, value);
        receiver = R32(person + 0x61u);
        ++slot;
        wide = cm_fn_005b7d20((u32)wide, (u32)(wide >> 32), (const u8 *)receiver);
        value = (u32)(s32)(s16)wide;
        cm_fn_00785790(local.news, slot, value);
    } else {
        value = local.work.words[1];
        cm_fn_00785790(local.news, value, 0);
    }
    id = R32(offer + 8u);
    base = R32(0x00ae23b4u);
    club = base + id * 581u;
    receiver = R32(0x00b7431cu);
    eax = cm_fn_007875e0(receiver, club);
    if (eax != 0) {
        receiver = R32(0x00b7431cu);
        eax = (u32)cm_fn_00786780((void *)receiver, (u32)local.news);
    } else {
        u8 count = local.work.bytes[3];
        eax = (eax & 0xffffff00u) | count;
        if ((s8)count > 0) {
            for (u32 n = 0; n < (u32)(s32)(s8)count; ++n) {
                value = R32((u32)local.interested + n * 4u);
                receiver = R32(0x00b7431cu);
                eax = (u32)cm_fn_007861e0((void *)receiver, (u32)local.news, value);
            }
        }
    }
done:
    CM_FS(u32, 0) = local.seh.prev;
    return eax;
}
