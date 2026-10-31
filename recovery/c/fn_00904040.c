#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_009059e0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00904040(u32 self, u32 output)
{
    u8 scratch[0x208];
    u32 local = (u32)scratch;
    u32 count, allocation, selected = 0, club, pointer, id, value, row;
    u32 index, context = self, result, text, sort_count;
    s32 quotient;
    count = D(0x00ae235c);
    D(local + 4) = self;
    allocation = (u32)cm_fn_00944e46(count * 12u);
    D(output) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x00aa1a1c, 0, 0, local + 0x108, local + 8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       local + 0x108, local + 8, 0x2ed);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result & 0xffff0000u;
    }
    count = D(0x00ae235c);
    club = D(0x00ae23b4);
    D(local) = 0;
    if ((s32)count > 0) {
        do {
            pointer = D(club + 0x53);
            if (pointer != 0 && cm_fn_0053b900(club) == 0) {
                pointer = D(club + 0x53);
                pointer = D(pointer + 0x71);
                if (pointer != 0) {
                    id = D(pointer);
                    value = D(0x009cfa10);
                    if (id == value) {
                        id = D(club);
                        D(club + 0x1db) = 0xffffffffu;
                        pointer = D(0x00ae2d14);
                        D(pointer + id * 6u) = 0xffffffffu;
                        row = allocation + (u32)(s32)(s16)selected * 12u;
                        D(row) = club;
                        pointer = D(club + 0x53);
                        value = D(pointer);
                        D(row + 4) = value;
                        /* Signed magic multiply00904159..171 is truncating
                           division by500 over the sign-extended WORD input. */
                        quotient = (s16)W(club + 0x80) / 500;
                        B(row + 8) = (u8)quotient;
                        pointer = D(club + 0x5b);
                        if (pointer != 0) {
                            value = W(pointer + 0x69);
                            W(row + 9) = (u16)value;
                            value = B(club + 0x5f);
                            B(row + 0xb) = (u8)value;
                            pointer = D(club + 0x5b);
                            pointer = D(pointer + 0x5d);
                            if (pointer == 0 || B(pointer + 0x11c) == 0) {
                                D(club + 0x5b) = 0;
                                B(club + 0x5f) = 0;
                            }
                        } else {
                            W(row + 9) = 0xffff;
                            B(row + 0xb) = 0x7f;
                        }
                        ++selected;
                    }
                }
            }
            index = D(local);
            club += 581;
            ++index;
            D(local) = index;
            count = D(0x00ae235c);
        } while ((s32)(s16)index < (s32)count);
        context = D(local + 4);
    }
    sort_count = (u32)(s32)(s16)selected;
    result = cm_fn_00945333(allocation, sort_count, 12, 0x00905ce0);
    /* 00945342/345 returns through00945482..486 for unsigned count<2
       without reading or writing EAX. Incoming EAX is the last club-total
       load009040df or009041c7. Other sorter paths overwrite EAX first. */
    if (sort_count < 2u)
        result = count;
    if (W(context + 0x439) == 2001)
        result = cm_fn_009059e0(context, allocation, selected);
    return (result & 0xffff0000u) | (u16)selected;
}
