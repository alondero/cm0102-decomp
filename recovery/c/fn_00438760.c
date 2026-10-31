/* Preferred-image VA 00438760, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   0068b100 reads the partial-register arguments at their actual widths:
   count WORD0068b12a, flags BYTE0068b151/15f/176,
   year WORD0068b1ce, option BYTE0068b1d6. No wider forwarding occurs. */
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
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00438760(u32 self, u32 option)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095497e;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    B((u32)scratch) = 1;
    u32 count = (u32)(s32)(s16)W(self + 0x3e);
    B((u32)scratch + 1) = 0;
    B((u32)scratch + 2) = 0;
    B((u32)scratch + 3) = 0;
    u32 clubs = (u32)cm_fn_00944e46(count * 4);
    u32 result;
    if (!clubs) {
        cm_fn_00944cff(0x0098af98, 0, 0, scratch + 0x108, scratch + 8);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x108, scratch + 8, 0x3d5);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    } else {
        u32 copied = 0;
        if ((s16)W(self + 0x3e) > 0) {
            do {
                u32 index = (u32)(s32)(s16)copied;
                u32 table = D(self + 0xb1);
                ++copied;
                u32 club = D(table + index * 59u);
                D(clubs + index * 4) = club;
            } while ((s16)copied < (s16)W(self + 0x3e));
        }
        u32 enum_value = 0x439u + ((u8)option != 0);
        u32 allocation = (u32)cm_fn_00944cf1(0xee);
        D((u32)scratch + 4) = allocation;
        frame.state = 0;
        u32 child = 0;
        if (allocation) {
            u32 year = W(self + 0x40);
            u32 flag_c4 = B(self + 0xc4);
            u32 flag_c3 = B(self + 0xc3);
            u32 flag_c2 = B(self + 0xc2);
            u32 competition = D(self + 4);
            u32 size = W(self + 0x3e);
            child = cm_fn_0068b100(allocation, self, size, clubs, 1, competition,
                0, 0, flag_c2, flag_c3, flag_c4, self + 0xc5, (u32)scratch,
                year, (u8)option, enum_value, 0, 1, 0, 8, 11, 0, 2);
        }
        u32 holder = D(self + 0xc);
        u32 index = (u32)(s32)(s8)option;
        D(holder + index * 4) = child;
        D(self + 0x30) = index;
        result = (u32)cm_fn_009452ca(clubs);
    }
    CM_FS(u32, 0) = frame.prev;
    return result;
}
