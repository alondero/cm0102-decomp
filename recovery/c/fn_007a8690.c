/* Preferred-image VA 007a8690, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00946464(u32 key, u32 base, u32 count,
                                  u32 size, u32 comparator);
extern u32 CM_THISCALL cm_fn_007a8780(u32 destination, u32 source);

u32 CM_THISCALL cm_fn_007a8690(u32 self, u32 key)
{
    u32 count, records, found, index, offset;
    count = D(self + 8u);
    records = D(self + 4u);
    found = cm_fn_00946464(key, records, count, 0x1au, 0x007ae2a0u);
    if (found == 0) return 0;
    records = D(self + 4u);
    index = (found - records) / 26u;
    count = D(self + 8u);
    if ((s32)index < (s32)(count - 1u)) {
        offset = index * 26u;
        do {
            records = D(self + 4u);
            cm_fn_007a8780(records + offset, records + offset + 26u);
            count = D(self + 8u);
            ++index;
            offset += 26u;
        } while ((s32)index < (s32)(count - 1u));
    }
    count = D(self + 8u);
    records = D(self + 4u);
    D(records + count * 26u - 26u) = 0xffffffffu;
    count = D(self + 8u);
    records = D(self + 4u);
    B(records + count * 26u - 2u) = 0;
    count = D(self + 8u);
    records = D(self + 4u);
    W(records + count * 26u - 0x12u) = 0;
    count = D(self + 8u);
    records = D(self + 4u);
    W(records + count * 26u - 0x10u) = 0;
    count = D(self + 8u);
    records = D(self + 4u);
    W(records + count * 26u - 0xau) = 0;
    count = D(self + 8u);
    records = D(self + 4u);
    W(records + count * 26u - 8u) = 0;
    count = D(self + 8u) - 1u;
    D(self + 8u) = count;
    return count;
}
