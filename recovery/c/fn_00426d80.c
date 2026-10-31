#include "cmre.h"

/* Preferred-image VA00426D80/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/RET0; sole caller discards EAX. All ten name comparisons and
   the later association mapping follow their own original sites. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945501(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

static u32 same_426d80(u32 left, u32 right)
{
    u8 a, b;
    for (;;) {
        a = B(left); b = B(right);
        if (a != b) return 0;
        if (a == 0) return 1;
        a = B(left + 1); b = B(right + 1);
        if (a != b) return 0;
        left += 2; right += 2;
        if (a == 0) return 1;
    }
}

void CM_THISCALL cm_fn_00426d80(u32 self)
{
    static const struct { u32 name, offset; } names[10] = {
        {0x98a398, 0}, {0x98a380, 0}, {0x98a364, 6}, {0x98a344, 12},
        {0x98a328, 18}, {0x98a304, 24}, {0x98a2e4, 30}, {0x98a2c8, 36},
        {0x98a2a8, 42}, {0x98a294, 48}
    };
    u8 scratch[0x52c];
    u32 L = (u32)scratch, total, club, i, j, association, first, second;
    u32 p, value, count, text, offset, table;
    total = D(0xae235c);
    club = D(0xae23b4);
    D(L + 4) = self;
    D(L) = 0;
    i = 0;
    if ((s32)total > 0) {
        first = D(0x9cf9c0);
        second = D(0x9cf998);
        do {
            if ((s16)W(L) >= 200) break;
            p = D(club + 0x60);
            if (p != 0) {
                association = D(p);
                if (association == first || association == second) goto collect;
                if (association == D(0x9cf9c8) || association == D(0x9cf9b4) ||
                    association == D(0x9cf9d4) || association == D(0x9cf994) ||
                    association == D(0x9cf9a4) || association == D(0x9cf9d0) ||
                    association == D(0x9cf9bc) || association == D(0x9cf9dc) ||
                    association == D(0x9cf9b0) || association == D(0x9cf9e4)) {
                    self = D(L + 4);
collect:
                    value = (u32)(s32)(s16)W(L);
                    D(L + 0x20c + value * 4) = club;
                    value = D(L);
                    D(L) = value + 1;
                } else self = D(L + 4);
            }
            ++i;
            club += 0x245;
        } while ((s32)(s16)(u16)i < (s32)total);
    }
    count = (u32)(s32)(s16)W(L);
    D(L + 8) = count;
    cm_fn_00945333(L + 0x20c, count, 4, 0x4b07d0);
    B(self + 0x4b) = 0xff;
    W(self + 0x38) = 9;
    W(self + 0x36) = 9;
    value = cm_fn_00945501(6, 9);
    D(self + 0x14) = value;
    if (value == 0) {
        cm_fn_00944cff(0x98a1e8, 0, 0, (void *)(L + 0xc), (void *)(L + 0x10c));
        value = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, value, L + 0xc, L + 0x10c, 0x3d7);
        cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
        D(0xb67a34) = 0;
        return;
    }
    total = D(0xae235c);
    club = D(0xae23b4);
    D(L + 4) = 0;
    if ((s32)total > 0) {
        text = club + 4;
        do {
            p = D(text + 0x4f);
            if (p != 0) {
                association = D(p);
                value = D(0x9cf260);
                if (association == value) {
                    for (j = 0; j != 10; ++j) {
                        if (same_426d80(text, names[j].name)) {
                            p = D(self + 0x14);
                            D(p + names[j].offset) = club;
                        }
                    }
                }
            }
            value = D(L + 4);
            club += 0x245;
            ++value;
            text += 0x245;
            D(L + 4) = value;
            total = D(0xae235c);
        } while ((s32)(s16)(u16)value < (s32)total);
        count = D(L + 8);
    }
    table = D(self + 0x14);
    i = 0;
    do {
        p = (u32)(s32)(s16)(u16)i * 6u;
        if (D(table + p) == 0) break;
        ++i;
    } while ((s16)(u16)i < 9);
    if ((u16)i == 9 || (s16)W(L) <= 0) return;
    p = L + 0x20c;
    do {
        value = D(p);
        first = D(0x9cf9c0);
        association = D(value + 0x60);
        association = D(association);
        if (association == first) offset = 48;
        else if (association == D(0x9cf998)) offset = 0;
        else if (association == D(0x9cf9c8)) offset = 36;
        else if (association == D(0x9cf9b4)) offset = 6;
        else if (association == D(0x9cf9d4)) offset = 30;
        else if (association == D(0x9cf994)) offset = 0;
        else if (association == D(0x9cf9a4)) offset = 0;
        else if (association == D(0x9cf9d0)) offset = 42;
        else if (association == D(0x9cf9bc)) offset = 12;
        else if (association == D(0x9cf9dc)) offset = 18;
        else if (association == D(0x9cf9b0)) offset = 6;
        else if (association == D(0x9cf9e4)) offset = 24;
        else goto advance;
        table = D(self + 0x14);
        if (D(table + offset) == 0) D(table + offset) = value;
advance:
        p += 4;
        --count;
    } while (count != 0);
}
