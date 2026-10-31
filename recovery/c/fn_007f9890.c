#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern void CM_THISCALL cm_fn_007f9a60(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* EAX is dead at both direct callers: full overwrite007f8b76, or the
   0068d040 call at007f8d44 whose first EAX access is MOV at0068d043. */
void CM_THISCALL cm_fn_007f9890(u32 self, u32 list)
{
    u8 scratch[0x210];
    u32 local = (u32)scratch;
    u32 count, allocation, value, text, node, cursor, source, index, row;
    u32 base, id, club, person, next, next_row, start, context, group;
    u16 raw_count;
    D(local + 0xc) = self;
    raw_count = W(list);
    if (raw_count == 0)
        return;
    count = (u32)(s32)(s16)raw_count;
    allocation = (u32)cm_fn_00944e46(count * 103u);
    if (allocation == 0 && count != 0) {
        cm_fn_00944cff(0x00a7b094, 0, 0, local + 0x110, local + 0x10);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
            local + 0x110, local + 0x10, 0x7dd);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return;
    }
    value = D(list + 2);
    D(list + 10) = value;
    node = cm_fn_0068d1b0(list);
    if (node != 0) {
        cursor = allocation;
        do {
            D(local) = cursor + 103;
            source = node;
            for (index = 0; index != 25; ++index) {
                value = D(source);
                D(cursor) = value;
                source += 4;
                cursor += 4;
            }
            value = W(source);
            W(cursor) = (u16)value;
            value = B(source + 2);
            B(cursor + 2) = (u8)value;
            node = cm_fn_0068d1b0(list);
            if (node != 0)
                cursor = D(local);
        } while (node != 0);
    }
    cm_fn_00945333(allocation, count, 103, 0x007fc2a0);
    index = 0;
    if ((s32)count > 0) {
        do {
            row = allocation + index * 103u;
            base = D(0x00ae23b4);
            B(local + 4) = 1;
            id = D(row + 0x50);
            club = base + id * 581u;
            id = D(row + 0x2b);
            start = index;
            D(local) = start;
            base = D(0x00ae23bc);
            person = base + id * 110u;
            next = index + 1u;
            if ((s32)next < (s32)count) {
                value = D(club);
                D(local + 8) = value;
                next_row = allocation + next * 103u + 0x2b;
                do {
                    value = D(local + 8);
                    if (D(next_row + 0x25) != value)
                        break;
                    id = D(next_row);
                    if (id != D(person))
                        break;
                    group = B(local + 4);
                    next_row += 103;
                    ++group;
                    ++next;
                    ++index;
                    B(local + 4) = (u8)group;
                } while ((s32)next < (s32)count);
                start = D(local);
            }
            /* Only BYTE L+4 was initialized; the pushed slot is a DWORD. */
            group = D(local + 4);
            row = allocation + start * 103u;
            context = D(local + 0xc);
            cm_fn_007f9a60(context, row, group);
            ++index;
        } while ((s32)index < (s32)count);
    }
    cm_fn_009452ca(allocation);
}
