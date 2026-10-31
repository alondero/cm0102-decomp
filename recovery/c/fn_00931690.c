#include "cmre.h"

/* Preferred-image VA 0x00931690, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The direct caller discards EAX. The original leaves scratch[3] uninitialized
   when the second argument's +0x14 field is null; retain that read. Signed
   constant divisions below follow the original multiply-high corrections. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00945667(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define S(a) (*(volatile s8 *)(u32)(a))

void CM_THISCALL cm_fn_00931690(u32 self, u32 stats, u32 player, u32 team)
{
    volatile u8 scratch[0x210];
    u32 id, lookup = 0, table, record, count, index = 0, capacity;
    u32 source, value, offset, pointer;
    u8 factor, a, b, c, appearances;
    s32 rating, product, score, reputation;
    char *version;

    id = D(player);
    if ((s32)id >= 0) {
        table = D(0x00adadfcu);
        lookup = D(table + id * 4u);
    }
    id = D(stats);
    source = D(0x00ae23bcu) + id * 110u;
    table = D(self);
    count = D(self + 4u);
    record = table;
    if ((s32)count > 0) {
        do {
            if (D(record) == id) goto update;
            record += 21u;
            ++index;
        } while ((s32)index < (s32)count);
    }
    capacity = D(self + 8u);
    if (count == capacity) {
        capacity += 400u;
        D(self + 8u) = capacity;
        if (table == 0)
            table = (u32)cm_fn_00944e46(capacity * 21u);
        else
            table = (u32)cm_fn_00945667(table, capacity * 21u);
        D(self) = table;
        if (table == 0) {
            cm_fn_00944cff(0x00ad6a38u, 0, 0,
                          (void *)(scratch + 0x110), (void *)(scratch + 0x10));
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                          scratch + 0x110, scratch + 0x10, 0x113);
            cm_fn_005e8290((const char *)0x009870e0u,
                          (const char *)0x00ae24d0u, 0);
            D(0x00b67a34u) = 0;
            return;
        }
    }
    count = D(self + 4u);
    id = D(stats);
    D(table + count * 21u) = id;
    pointer = D(source + 0x1au);
    value = pointer != 0 ? D(pointer) : 0xffffffffu;
    count = D(self + 4u);
    table = D(self);
    D(table + count * 21u + 4u) = value;
    count = D(self + 4u); table = D(self);
    W(table + count * 21u + 8u) = 0;
    count = D(self + 4u); table = D(self);
    W(table + count * 21u + 10u) = 0;
    count = D(self + 4u); table = D(self);
    W(table + count * 21u + 12u) = 0;
    count = D(self + 4u); table = D(self);
    W(table + count * 21u + 15u) = 0;
    count = D(self + 4u); table = D(self);
    W(table + count * 21u + 17u) = 0;
    count = D(self + 4u); table = D(self);
    W(table + count * 21u + 19u) = 0;
    count = D(self + 4u); table = D(self);
    B(table + count * 21u + 14u) = 0;
    count = D(self + 4u) + 1u;
    D(self + 4u) = count;
    table = D(self);
    record = table + count * 21u - 21u;
update:
    pointer = D(player + 0x14u);
    if (pointer != 0) {
        rating = (s16)W(team + 0x80u);
        reputation = (s16)W(pointer + 0x69u);
        product = reputation * (rating / 500);
        factor = (u8)(product / 20 + (rating - 5000) / 1700);
        if ((s8)factor < 1) factor = 1;
        else if ((s8)factor > 20) factor = 20;
    } else {
        factor = scratch[3];
    }
    if (lookup == 0) {
        rating = (s16)W(team + 0x80u);
        score = rating / 500;
        pointer = D(team + 0x53u);
        reputation = (s16)W(pointer + 0x8eu);
        score = (s8)(u8)score;
        if (reputation > 7000) score *= 16;
        else if (reputation > 4000) score *= 8;
        else score *= 4;
        score /= 20;
        value = B(stats + 0x35u);
        product = (s32)value * (s8)(u8)score;
        goto ordinary;
    }
    value = (u32)(s32)S(lookup + 0x42u);
    if (value == 2u) {
        value = B(stats + 0x35u);
        product = (s32)value * (s8)factor;
        a = (u8)(product / 20);
        b = a;
        c = (u8)(product / 40);
    } else if (value == 4u) {
        value = B(stats + 0x35u);
        product = (s32)value * (s8)factor;
        a = (u8)(product / 20);
        b = a;
        c = a;
    } else {
        value = B(stats + 0x35u);
        product = (s32)value * (s8)factor;
ordinary:
        a = (u8)(product / 20);
        b = (u8)(product / 60);
        c = (u8)(product / 80);
    }
    W(record + 8u) = (u16)(W(record + 8u) + (s16)(s8)a);
    appearances = B(record + 14u);
    W(record + 10u) = (u16)(W(record + 10u) + (s16)(s8)b);
    W(record + 12u) = (u16)(W(record + 12u) + (s16)(s8)c);
    B(record + 14u) = (u8)(appearances + 1u);
    value = B(stats + 0x21u);
    W(record + 15u) = (u16)(W(record + 15u) + value);
    value = B(stats + 0x27u);
    W(record + 17u) = (u16)(W(record + 17u) + value);
    value = B(stats + 0x2du);
    offset = B(stats + 0x2cu);
    W(record + 19u) = (u16)(W(record + 19u) + offset + value * 2u);
}
