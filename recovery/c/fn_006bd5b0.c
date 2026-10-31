#include "cmre.h"
/* Preferred-image VA 0x006bd5b0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The two status calls remain separate, and the candidate test compares AX to
   one rather than testing a bit. The observed direct callers consume AL. */
extern u8 CM_THISCALL cm_fn_00714d30(u32);
extern u32 CM_FASTCALL cm_fn_00717410(u32);
#define B(a) (*(volatile u8 *)(u32)(a))

u8 CM_THISCALL cm_fn_006bd5b0(u32 self, u32 record, u8 ignore)
{
    u8 first, second, side;
    s8 row, column, current, candidate;
    u32 pointer, i, offset;
    first = cm_fn_00714d30(record);
    second = cm_fn_00714d30(record);
    side = second == 0;
    if (!ignore) {
        row = (s8)B(record + 0x102);
        column = (s8)B(record + 0x103);
        offset = (u32)((s32)row * 12 + (s32)column) * 90u;
        if (B(self + side + offset + 0x21b6)) return 0;
    }
    column = (s8)B(record + 0x103);
    row = (s8)B(record + 0x102);
    pointer = self + (u32)side * 0x22d8u + 0x4796;
    for (i = 0; i < 20; ++i) {
        if ((s8)B(pointer + 0x19) >= 0 && (u16)cm_fn_00717410(pointer) != 1) {
            if (first == 1) {
                current = (s8)B(pointer + 0x103);
                if (column <= current) goto next;
            } else {
                if (first != 0) goto next;
                current = (s8)B(pointer + 0x103);
                if (column >= current) goto next;
            }
            candidate = (s8)B(pointer + 0x102);
            if ((s32)row + 1 <= candidate) goto next;
            if ((s32)row - 1 < candidate) return 0;
        }
next:
        pointer += 0x1be;
    }
    return 1;
}
