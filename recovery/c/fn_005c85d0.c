#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_FASTCALL cm_fn_0068d020(u32);
extern void CM_FASTCALL cm_fn_0068d040(u32);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32, u32);
extern u32 CM_THISCALL cm_fn_005acbe0(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_005abc30(u32, u32);
extern u32 CM_THISCALL cm_fn_005c88f0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_005c87b0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005c85d0(u32 self, u32 record)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x218];
    u32 local = (u32)scratch;
    u32 pointer, offset, value, first, mixed, club, context, node;
    u32 year, node_year, day, current_day, result, text;
    u8 index, count;
    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095a2a6;
    CM_FS(u32, 0) = (u32)&frame;
    if (record == 0 || D(self + 0x34cc) == 0) {
        cm_fn_00944cff(0x009cd0a0, 0, 0, local + 0x118, local + 0x18);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       local + 0x118, local + 0x18, 0x112c);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    count = B(record + 8);
    index = 0;
    if ((s8)count > 0) {
        do {
            offset = (u32)(s32)(s8)index * 25u;
            value = D(record + offset + 9);
            D(local) = value;
            value = D(record + offset + 0xd);
            D(local + 4) = value;
            frame.state = 0;
            cm_fn_0068d020(local + 8);
            pointer = D(record);
            club = D(pointer);
            first = D(local);
            mixed = D(local + 2);
            context = D(0x00ae2a58);
            *(volatile u8 *)&frame.state = 1;
            cm_fn_005acbe0(context, first, mixed, first, mixed, club,
                local + 8, 0xffffffffu, 0xfffffffeu, 0xffffffffu);
            value = D(local + 0xa);
            D(local + 0x12) = value;
            node = cm_fn_0068d1b0(local + 8);
            while (node != 0) {
                year = (u32)(s32)(s16)W(0x00ae2c90);
                node_year = W(node + 0x2a);
                if (node_year == year) {
                    day = W(node + 0x28);
                    current_day = W(0x00ae2c92);
                    if (day == current_day)
                        goto next_node;
                }
                if (B(node + 0x43) == 0xff) {
                    value = D(node);
                    club = D(0x009cf668);
                    if (value == club) {
                        context = D(0x00ae2a58);
                        cm_fn_005abc30(context, node);
                    }
                }
next_node:
                node = cm_fn_0068d1b0(local + 8);
            }
            club = D(record);
            value = cm_fn_005c88f0(self, local, club);
            if (value != 0)
                cm_fn_005c87b0(self, value, 0);
            *(volatile u8 *)&frame.state = 0;
            cm_fn_0068d040(local + 8);
            count = B(record + 8);
            index = (u8)(index + 1u);
            frame.state = 0xffffffffu;
        } while ((s8)index < (s8)count);
    }
    pointer = D(self + 0x34cc);
    result = cm_fn_0068d1f0(pointer, record);
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
