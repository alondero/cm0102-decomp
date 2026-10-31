/* Preferred-image VA 005ca3e0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole direct caller discards EAX; the following initializer overwrites it. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern void CM_CDECL cm_fn_00945333(u32 base, u32 count, u32 size, u32 comparator);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

void CM_THISCALL cm_fn_005ca3e0(u32 self)
{
    u8 work[0x200];
    u32 count, allocation, nation, index, selected, table, text;
    D(self + 0x34feu) = 0;
    count = D(0x00ae2350u);
    allocation = (u32)cm_fn_00944e46(count * 4u);
    D(self + 0x34fau) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009cd0a0u, 0, 0, work + 0x100, work);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x100, work, 0x15afu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return;
    }
    count = D(0x00ae2350u);
    nation = D(0x00ae23a8u);
    index = 0;
    if ((s32)count > 0) {
        do {
            if (D(nation + 0x71u) != 0) {
                selected = D(self + 0x34feu);
                table = D(self + 0x34fau);
                D(table + selected * 4u) = nation;
                selected = D(self + 0x34feu);
                D(self + 0x34feu) = selected + 1u;
            }
            count = D(0x00ae2350u);
            index += 1u;
            nation += 0x122u;
        } while ((s32)index < (s32)count);
    }
    count = D(self + 0x34feu);
    table = D(self + 0x34fau);
    cm_fn_00945333(table, count, 4, 0x005cc5f0u);
}
