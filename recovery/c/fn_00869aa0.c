#include "cmre.h"
/* Preferred-image VA 0x00869aa0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Stack arguments are player, club, mode (EBP, EBX, low mode byte).
   Keep the separate record buffers, shared date output, linked SEH state,
   and full EAX from the final append or diagnostic boundary. */
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_0086f5b0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern s32 CM_THISCALL cm_fn_0054cdb0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00549f70(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_0058e340(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_00869aa0(u32 self, u32 player, u32 club, u32 mode)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 scratch[0x4b4];
    u32 local = (u32)scratch;
    u32 record, date0, date1, value, previous, start, date, attribute;
    u32 id, pointer, base, row, receiver, first, second, line, text, result;
    u16 year, checkpoint_year;
    s32 elapsed;
    frame.state = 0xffffffffu;
    frame.handler = 0x00962bc1;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(local + 8u) = self;
    if (club == 0 || player == 0) {
        first = local + 0x3b4u;
        second = local + 0x2b4u;
        line = 0x3b1;
        goto final_error;
    }
    if (cm_fn_0053b900(club) == 0) {
        record = cm_fn_0086f5b0(self, player, 1);
        if (record != 0) {
            date0 = D(0x00dd6ec8);
            date1 = D(0x00dd6ecc);
            year = W(0x00ae2c92);
            D(local) = date0;
            D(local + 4u) = date1;
            checkpoint_year = W(local + 2u);
            if (checkpoint_year <= year &&
                (checkpoint_year != year || (s16)(u16)date0 <= (s16)W(0x00ae2c90))) {
                value = D(record + 0x72u);
                if (value != 0xffffffffu) {
                    previous = D(record + 0x4au);
                    if (previous != 0xffffffffu) {
                        if (value == previous) {
                            date0 = D(record + 0x4eu);
                            date1 = D(record + 0x52u);
                            start = record + 0x4eu;
                            D(local) = date0;
                            year = W(local + 2u);
                            D(local + 4u) = date1;
                            if (year == W(record + 0x78u) &&
                                (u16)date0 == W(record + 0x76u)) {
                                cm_fn_00417c10(record + 0x7eu, local, 0x00ae2c90);
                                goto long_interval;
                            }
                        }
                        date1 = D(record + 0x52u);
                        date0 = D(record + 0x4eu);
                        start = record + 0x4eu;
                        elapsed = cm_fn_0054cdb0(0x00ae2c90, date0, date1);
                        date0 = D(record + 0x76u);
                        date1 = D(record + 0x7au);
                        D(local) = (u32)elapsed;
                        elapsed = cm_fn_0054cdb0(record + 0x7eu, date0, date1);
                        value = D(local);
                        if ((s32)value < elapsed) {
                            value = D(record + 0x4au);
                            D(record + 0x72u) = value;
                            cm_fn_00417c10(record + 0x76u, local, start);
                            cm_fn_00417c10(record + 0x7eu, local, 0x00ae2c90);
                        }
                        goto long_interval;
                    }
                }
                value = D(record + 0x4au);
                start = record + 0x4eu;
                D(record + 0x72u) = value;
                cm_fn_00417c10(record + 0x76u, local, start);
                cm_fn_00417c10(record + 0x7eu, local, 0x00ae2c90);
long_interval:
                value = D(record + 0x5eu);
                if (value != 0xffffffffu && D(record + 0x4au) != 0xffffffffu) {
                    date1 = D(start + 4u);
                    date0 = D(start);
                    elapsed = cm_fn_0054cdb0(0x00ae2c90, date0, date1);
                    date0 = D(record + 0x62u);
                    date1 = D(record + 0x66u);
                    D(local) = (u32)elapsed;
                    elapsed = cm_fn_0054cdb0(record + 0x6au, date0, date1);
                    value = D(local);
                    if ((s32)value > elapsed) {
                        value = D(record + 0x4au);
                        D(record + 0x5eu) = value;
                        cm_fn_00417c10(record + 0x62u, local, start);
                        cm_fn_00417c10(record + 0x6au, local, record + 0x56u);
                    }
                } else {
                    value = D(record + 0x4au);
                    D(record + 0x5eu) = value;
                    cm_fn_00417c10(record + 0x62u, local, start);
                    cm_fn_00417c10(record + 0x6au, local, 0x00ae2c90);
                }
                self = D(local + 8u);
            }
            D(record + 0x4au) = 0xffffffffu;
            if ((u8)mode == 1) {
                value = D(club);
                D(record + 0x4au) = value;
                cm_fn_00417c10(record + 0x4eu, local, 0x00ae2c90);
                date = cm_fn_00549f70(local, 13, 11, 0x7b7, 0xffffffffu);
                cm_fn_00417c10(record + 0x56u, local + 8u, date);
                B(record + 0xb4u) = (u8)(B(record + 0xb4u) + 1u);
            }
        }
    } else if ((u8)mode == 1) {
        record = cm_fn_0086f5b0(self, player, 1);
        if (record != 0) B(record + 0xb5u) = (u8)(B(record + 0xb5u) + 1u);
    }
    cm_fn_0054a110(local + 0x7cu);
    frame.state = 0;
    cm_fn_00417c10(local + 0x7cu, local, 0x00ae2c90);
    attribute = (u32)(s32)(s8)(u8)mode;
    id = D(club);
    B(local + 0x88u) = 0x96;
    D(local + 0x84u) = id;
    D(local + 0x89u) = attribute;
    if (local + 0x7cu != 0) {
        id = D(player);
        base = D(self + 0xcu);
        row = base + id * 8u;
        receiver = D(self);
        cm_fn_0058e340(receiver, row, row + 4u, local + 0x7cu);
    } else {
        cm_fn_00944cff(0x00a83b78, 0, 0, local + 0xb4u, local + 0x1b4u);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       local + 0xb4u, local + 0x1b4u, 0x1010);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    }
    pointer = D(club + 0xbfu);
    if (pointer != 0) {
        cm_fn_0054a110(local + 0x10u);
        *(volatile u8 *)&frame.state = 1;
        cm_fn_00417c10(local + 0x10u, local, 0x00ae2c90);
        id = D(club);
        value = D(player);
        pointer = D(player + 4u);
        B(local + 0x1cu) = 0x97;
        D(local + 0x18u) = id;
        D(local + 0x1du) = attribute;
        D(local + 0x21u) = value;
        value = D(pointer + 0x33u);
        pointer = D(player + 8u);
        D(local + 0x25u) = value;
        base = D(player + 0xcu);
        value = D(pointer + 0x33u);
        D(local + 0x29u) = value;
        value = D(base + 0x33u);
        D(local + 0x2du) = value;
        pointer = D(club + 0xbfu);
        if (pointer != 0 && local + 0x10u != 0) {
            id = D(pointer);
            base = D(self + 0xcu);
            row = base + id * 8u;
            receiver = D(self);
            cm_fn_0058e340(receiver, row, row + 4u, local + 0x10u);
        } else {
            cm_fn_00944cff(0x00a83b78, 0, 0, local + 0x1b4u, local + 0xb4u);
            text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                           local + 0x1b4u, local + 0xb4u, 0x1010);
            cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
            D(0x00b67a34) = 0;
        }
        *(volatile u8 *)&frame.state = 0;
    }
    cm_fn_0054a110(local + 0x48u);
    *(volatile u8 *)&frame.state = 2;
    cm_fn_00417c10(local + 0x48u, local, 0x00ae2c90);
    id = D(player);
    pointer = D(player + 4u);
    B(local + 0x50u) = 0x96;
    D(local + 0x51u) = id;
    value = D(pointer + 0x33u);
    pointer = D(player + 8u);
    D(local + 0x55u) = value;
    base = D(player + 0xcu);
    value = D(pointer + 0x33u);
    D(local + 0x59u) = value;
    value = D(base + 0x33u);
    D(local + 0x61u) = attribute;
    D(local + 0x5du) = value;
    if (local + 0x48u != 0) {
        id = D(club);
        base = D(self + 0x10u);
        row = base + id * 8u;
        receiver = D(self + 4u);
        result = cm_fn_0058e340(receiver, row, row + 4u, local + 0x48u);
    } else {
        first = local + 0x1b4u;
        second = local + 0xb4u;
        line = 0x102c;
final_error:
        cm_fn_00944cff(0x00a83b78, 0, 0, first, second);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, first, second, line);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    }
    CM_FS(u32, 0) = frame.prev;
    return result;
}
