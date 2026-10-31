/* Preferred-image VA 0057c320, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00945501(u32 count, u32 size);
extern u32 CM_CDECL cm_fn_0090cfc0(u32 limit);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0057c320(u32 self)
{
    u8 work[0x20c];
    u32 allocation, value, source, destination, index, remaining, random, count_address, version;
    s32 quotient;
    u8 maximum, count;
    B((u32)work) = 1;
    B((u32)work + 1u) = 1;
    B((u32)work + 2u) = 1;
    B((u32)work + 3u) = 1;
    allocation = cm_fn_00945501(16, 5);
    if (allocation == 0) {
        cm_fn_00944cff(0x009c4638u, 0, 0, work + 0x10c, work + 12);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x10c, work + 12, 0x3beu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    value = D(self + 0xeeu);
    B(allocation + 4u) = 1;
    D(allocation) = value;
    value = D(self + 0xf3u);
    D(allocation + 5u) = value;
    B(allocation + 9u) = 2;
    value = D(self + 0xf8u);
    B(allocation + 14u) = 3;
    D(allocation + 10u) = value;
    value = D(self + 0xfdu);
    D(allocation + 15u) = value;
    B(allocation + 19u) = 4;
    D((u32)work + 4u) = 4;
    destination = allocation + 20u;
    source = self + 0x102u;
    D((u32)work + 8u) = 12;
    do {
        index = D((u32)work + 4u);
        quotient = (s32)index / 4;
        maximum = (u8)((u32)quotient + 1u);
        do {
            random = cm_fn_0090cfc0(4);
            count_address = (u32)work + (u32)(s32)(s16)random;
            count = B(count_address);
        } while ((s8)count >= (s8)maximum);
        value = D(source);
        D(destination) = value;
        index = D((u32)work + 4u);
        source += 5u;
        B(destination + 4u) = (u8)(random + 1u);
        destination += 5u;
        count = B(count_address);
        index += 1u;
        B(count_address) = (u8)(count + 1u);
        remaining = D((u32)work + 8u) - 1u;
        D((u32)work + 4u) = index;
        D((u32)work + 8u) = remaining;
    } while (remaining != 0);
    return allocation;
}
