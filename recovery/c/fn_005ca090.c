#include "cmre.h"

/* Preferred-image VA 0x005ca090, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   005c6b70 replaces ECX before use/forwarding; its three stack args remain explicit. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_FASTCALL cm_fn_0068d020(u32);
extern void CM_FASTCALL cm_fn_0068d040(u32);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_005acbe0(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_STDCALL cm_fn_005c6b70(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005ca090(u32 self, volatile u32 first, volatile u32 second,
    volatile u32 third, volatile u32 fourth, volatile u32 fifth,
    volatile u32 sixth, volatile u32 seventh, volatile s16 eighth)
{
    u32 previous = CM_FS(u32, 0);
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x218];
    u32 local = (u32)scratch, message, date, club, number, other, id, manager;
    u32 value, high, result, text, row, format;
    u16 year, other_year;
    s32 length;
    (void)self;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095a35b;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    message = seventh;
    if (message != 0 && eighth < 100) {
        cm_fn_00944cff(0x009cd0a0, 0, 0, local + 0x118, local + 0x18);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x118, local + 0x18, 0x1527);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        result = 1; goto done;
    }
    date = sixth;
    if (date == 0) {
        if (message != 0) {
            cm_fn_0068d020(local);
            club = first;
            number = fifth;
            id = D(club);
            other = fourth;
            manager = D(0x00ae2a58);
            frame.state = 0;
            cm_fn_005acbe0(manager, other, number, other, number, id, local,
                          0xffffffffu, 0xfffffffeu, 0xffffffffu);
            value = D(local + 2);
            D(local + 0xa) = value;
            row = cm_fn_0068d1b0(local);
            format = 0x009cd148;
            if (row != 0 && D(row + 0x1c) == club) {
                value = D(row + 0x20);
                other = second;
                if (value == other) format = 0x009cd168;
            }
            cm_fn_0066f4e0(message, format);
            frame.state = 0xffffffffu;
            cm_fn_0068d040(local);
        }
        result = 0; goto done;
    }
    row = cm_fn_0054c5b0(0x00ae2c90, local + 0x10, 2);
    value = D(row);
    high = D(row + 4);
    D(local) = value;
    year = W(local + 2);
    D(local + 4) = high;
    other_year = W(date + 2);
    if (year > other_year || (year == other_year && (s16)value > (s16)W(date))) {
        if (message != 0) cm_fn_0066f4e0(message, 0x009cd148);
        result = 0; goto done;
    }
    club = third;
    value = cm_fn_005c6b70(date, club, 0);
    if (value != 0) { result = 1; goto done; }
    if (message != 0) {
        length = (s8)B(club + 0x52);
        cm_fn_0066f4e0(message, 0x009cd104, (u32)length, (u32)length, club + 0x38);
    }
    result = 0;
done:
    previous = frame.prev;
    CM_FS(u32, 0) = previous;
    return result;
}
