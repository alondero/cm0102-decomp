#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (self):
   0xf582 dword, compared with the current row pointer when the fourth
   argument is nonzero. Rows are 0x1be bytes apart, 20 rows per call, starting
   at self + 0x4796 + (signed second argument) * 0x22d8.
   Row offsets: 0x19 signed byte (a row counts only when it is >= 0),
   0x103 signed byte compared with the first argument. */

u8 CM_THISCALL cm_fn_006cb700(u32 self, u8 arg_a, u8 arg_b, u8 arg_c)
{
    u8 count;
    s32 s;
    u32 row;
    u32 remaining;

    count = 0;
    s = (s32)(s8)arg_b;
    row = self + 0x4796u + (u32)s * 0x22d8u;
    remaining = 0x14;
    do {
        if (*(volatile s8 *)(row + 0x19) >= 0) {
            if (arg_c == 0 || row != *(volatile u32 *)(self + 0xf582)) {
                if (arg_b == 1) {
                    if (*(volatile s8 *)(row + 0x103) < (s8)arg_a) {
                        count = (u8)(count + 1);
                    }
                } else if (arg_b == 0) {
                    if (*(volatile s8 *)(row + 0x103) > (s8)arg_a) {
                        count = (u8)(count + 1);
                    }
                }
            }
        }
        row = row + 0x1be;
        remaining = remaining - 1;
    } while (remaining != 0);
    return count;
}
