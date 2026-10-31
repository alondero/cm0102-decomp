#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_0068b100(u32, u32, u32, u32, u32, u32,
    u32, u32, u32, u32, u32, u32, u32, u32, u32, u32,
    u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_004204d0(u32 self, u32 option)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 previous, count, clubs, index, table, value, enumeration, limit, allocated;
    u32 year, c4, c3, c2, competition, child, holder, result, text;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095405e;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    B((u32)scratch) = 0;
    count = (u32)(s32)(s16)W(self + 0x3e);
    B((u32)scratch + 1) = 1;
    B((u32)scratch + 2) = 0;
    B((u32)scratch + 3) = 0;
    clubs = (u32)cm_fn_00944e46(count * 4u);
    if (clubs == 0) {
        cm_fn_00944cff(0x00989b54, 0, 0, (u32)scratch + 0x108, (u32)scratch + 8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x108, (u32)scratch + 8, 0x346);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    index = 0;
    if ((s16)W(self + 0x3e) > 0) {
        do {
            count = (u32)(s32)(s16)index;
            table = D(self + 0xb1);
            ++index;
            value = D(table + count * 59u);
            D(clubs + count * 4u) = value;
        } while ((s16)index < (s16)W(self + 0x3e));
    }
    if ((u8)option == 0) {
        enumeration = 0x439;
        limit = 10;
    } else {
        enumeration = (u8)option == 1 ? 0x43a : 0x43b;
        limit = 12;
    }
    allocated = (u32)cm_fn_00944cf1(0xee);
    D((u32)scratch + 4) = allocated;
    frame.state = 0;
    child = 0;
    if (allocated != 0) {
        year = W(self + 0x40);
        c4 = B(self + 0xc4);
        c3 = B(self + 0xc3);
        c2 = B(self + 0xc2);
        competition = D(self + 4);
        count = W(self + 0x3e);
        /* 0068b100 consumes the partial-register inputs at their actual
           WORD/BYTE widths; its upper register fragments are dead. */
        child = cm_fn_0068b100(allocated, self, count, clubs, 1, competition,
            0, 0, c2, c3, c4, self + 0xc5, (u32)scratch, year, option,
            enumeration, 0, 1, 0, 2, limit, 0, 2);
    }
    index = (u32)(s32)(s8)option;
    holder = D(self + 0xc);
    D(holder + index * 4u) = child;
    D(self + 0x30) = index;
    result = (u32)cm_fn_009452ca(clubs);
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
