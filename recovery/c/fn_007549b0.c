/* Preferred-image VA 0x007549b0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_CDECL __attribute__((regparm(2))) cm_fn_00945333(
    u32 eax_in, u32 edx_in, u32 base, u32 count, u32 width, u32 compare);
u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_007549b0(
    u32 incoming_eax, u32 incoming_edx, u32 self, u32 key)
{
    u32 count, index = 0, offset = 0, base, source, destination, i, entry, wanted;
    (void)incoming_eax;
    (void)incoming_edx;
    count = *(volatile u32 *)(self + 0x14u);
    if ((s32)count > 0) {
        do {
            base = *(volatile u32 *)(self + 0xcu);
            destination = base + offset;
            entry = *(volatile u32 *)destination;
            wanted = *(volatile u32 *)key;
            if (entry == wanted) {
                source = base + count * 25u - 25u;
                for (i = 0; i < 6; ++i)
                    *(volatile u32 *)(destination + 4u * i) = *(volatile u32 *)(source + 4u * i);
                *(volatile u8 *)(destination + 24u) = *(volatile u8 *)(source + 24u);
                count = *(volatile u32 *)(self + 0x14u) - 1u;
                *(volatile u32 *)(self + 0x14u) = count;
                --index;
                offset -= 25u;
            }
            count = *(volatile u32 *)(self + 0x14u);
            ++index;
            offset += 25u;
        } while ((s32)index < (s32)count);
    }
    count = *(volatile u32 *)(self + 0x14u);
    if (!count) return self;
    base = *(volatile u32 *)(self + 0xcu);
    return (u32)cm_fn_00945333(base, offset, base, count, 25u, 0x00757ea0u);
}
