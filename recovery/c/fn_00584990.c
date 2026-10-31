/* Preferred-image VA 00584990, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00584990(u32 self)
{
    u8 work[0x200];
    u32 allocation, version, i = 0, offset = 0, rows, entries, index, value;
    u16 count;
    u8 tag;
    W(self + 0x3eu) = 55;
    allocation = (u32)cm_fn_00944e46(0x14au);
    D(self + 0xa7u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009c4900u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x2f6u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0xffffffffu;
    }
    if ((s16)W(self + 0x3eu) > 0) {
        do {
            count = W(self + 0x36u);
            if (count != 0) {
                rows = D(self + 0x14u);
                entries = D(self + 0xa7u);
                index = (u32)(s32)(s16)count - i - 1u;
                value = D(rows + index * 6u);
                D(entries + offset) = value;
            } else {
                entries = D(self + 0xa7u);
                D(entries + offset) = 0;
            }
            count = W(self + 0x36u);
            rows = D(self + 0x14u);
            index = (u32)(s32)(s16)count - i;
            tag = B(rows + index * 6u - 2u);
            entries = D(self + 0xa7u);
            B(entries + offset + 4u) = tag == 8 ? 2 : tag == 13 ? 1 : 0;
            entries = D(self + 0xa7u);
            ++i;
            B(entries + offset + 5u) = 0;
            offset += 6u;
        } while ((s32)i < (s32)(s16)W(self + 0x3eu));
    }
    return 1;
}
