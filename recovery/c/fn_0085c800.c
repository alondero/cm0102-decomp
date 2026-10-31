#include "cmre.h"
/* Preferred-image VA0085c800/base00400000, pinned 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS7/RET28/full EAX. Scratch18: comparatorL0, scoreL4, keyL8..L17.
   Original comparators85e2f0/85e320 read only key+0/+4 respectively and
   initialized key+c=0. Other key bytes stay unwritten; bsearch forwards
   the pointer without copying the key. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
typedef u32 (CM_CDECL *comparison)(u32,u32);
extern void CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00946464(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_0085ddb0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0085de20(u32,u32,u32);
/* RET20, no local ECX input read; retain ECX set at original85c9fb. */
extern u32 CM_THISCALL cm_fn_0085da00(u32,u32,u32,u32,u32,u32);

u32 CM_THISCALL cm_fn_0085c800(u32 self, volatile u32 flags_arg,
    volatile u32 key4_arg, volatile u32 key0_arg, volatile u32 filter_arg,
    volatile u32 club_a_arg, volatile u32 club_b_arg, volatile u32 nation_arg)
{
    u8 scratch[0x18];
    u32 l = (u32)scratch, key, count, table, comparator, current, flags;
    u32 value, pointer, excluded, first, second, nation, filter, score;
    u32 counter = 0, remainder, category;
    u8 mode;
    key = key0_arg;
    D(l + 4) = 0;
    if (key) {
        if (B(self + 0x18) != 1) {
            count = D(self + 0x10);
            table = D(self + 0xc);
            cm_fn_00945333(table, count, 16, 0x0085e2f0);
            B(self + 0x18) = 1;
        }
        value = D(key);
        comparator = 0x0085e2f0;
        D(l + 8) = value;
        D(l + 0x14) = 0;
        D(l) = comparator;
        goto search;
    }
    key = key4_arg;
    mode = B(self + 0x18);
    if (key) {
        if (mode != 2) {
            count = D(self + 0x10);
            table = D(self + 0xc);
            cm_fn_00945333(table, count, 16, 0x0085e320);
            B(self + 0x18) = 2;
        }
        value = D(key);
        D(l) = 0x0085e320;
        comparator = D(l);
        D(l + 0xc) = value;
        D(l + 0x14) = 0;
        goto search;
    }
    if (mode != 3) {
        count = D(self + 0x10);
        table = D(self + 0xc);
        cm_fn_00945333(table, count, 16, 0x0085e350);
        B(self + 0x18) = 3;
    }
    current = D(self + 0xc);
    goto selected;
search:
    count = D(self + 0x10);
    table = D(self + 0xc);
    current = cm_fn_00946464(l + 8, table, count, 16, comparator);
    if (current) {
        for (;;) {
            table = D(self + 0xc);
            if (table >= current) break;
            current -= 16u;
            value = ((comparison)comparator)(current, l + 8);
            if (value) { current += 16u; break; }
            if (!current) break;
        }
    }
selected:
    flags = flags_arg;
    if (!current) goto fallback;
    for (;;) {
        if ((s16)counter >= 30) break;
        count = D(self + 0x10);
        table = D(self + 0xc);
        if (current == table + (count + 0x0fffffffu) * 16u) break;
        pointer = D(current + 0xc);
        excluded = D(0x00a835a4);
        value = D(pointer);
        if (value == excluded) goto next_uncounted;
        first = club_a_arg;
        if (first) {
            second = club_b_arg;
            if (second && cm_fn_0053b900(first) == 0 && (s32)flags < 64) {
                first = club_a_arg;
                pointer = D(current + 0xc);
                if (pointer == D(first + 0x69)) goto next_counted;
                second = club_b_arg;
                if (pointer == D(second + 0x69)) goto next_counted;
            }
        }
        value = key0_arg;
        if (!value) value = key4_arg;
        if (value) {
            comparator = D(l);
            if (((comparison)comparator)(current, l + 8) != 0) break;
        }
        if ((flags & 0x7fc0u) == 0) {
            if ((flags & 4u) && D(current + 8) == 0) goto next_uncounted;
            if ((flags & 2u) && D(current + 8) != 0) goto next_uncounted;
            filter = filter_arg;
            pointer = D(current + 0xc);
            if (cm_fn_0085ddb0(self, pointer, filter) == 0) goto next_counted;
            nation = nation_arg;
            second = club_b_arg;
            first = club_a_arg;
            pointer = D(current + 0xc);
            score = cm_fn_0085da00(self, pointer, flags, first, second, nation);
            value = D(l + 4);
            if ((s32)score > (s32)value) {
                D(l + 4) = score;
                flags_arg = current;
            }
            goto next_counted;
        }
        if (D(current + 8) != 0) goto next_uncounted;
        remainder = (u32)((s32)(s16)counter % 9);
        for (category = 0; category < 9; ++category) {
            if ((flags & (0x40u << category)) && remainder == category) break;
        }
        if (category == 9) goto next_counted;
        nation = nation_arg;
        if (nation) {
            value = D(nation);
            excluded = D(0x009cf76c);
            if (value == excluded) goto claim;
        }
        filter = filter_arg;
        pointer = D(current + 0xc);
        if (cm_fn_0085ddb0(self, pointer, filter)) goto claim;
next_counted:
        counter++;
next_uncounted:
        current += 16u;
        if (!current) break;
    }
    if ((s32)D(l + 4) > 0) {
        nation = nation_arg;
        if (nation) {
            pointer = flags_arg;
            filter = filter_arg;
            value = D(pointer + 0xc);
            cm_fn_0085de20(self, value, filter);
        }
        pointer = flags_arg;
        return D(pointer + 0xc);
    }
fallback:
    pointer = key0_arg;
    if (pointer && !(flags & 2u) && (flags & 0x2cu)) return D(pointer + 0x80);
    return 0;
claim:
    filter = filter_arg;
    pointer = D(current + 0xc);
    cm_fn_0085de20(self, pointer, filter);
    return D(current + 0xc);
}
