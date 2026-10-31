/* Preferred-image VA 005cc2d0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole direct caller overwrites EAX immediately after this call. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

void CM_THISCALL cm_fn_005cc2d0(u32 self)
{
    u8 work[0x200];
    u32 allocation, table, value, text;
    B(self + 0x3506u) = 4;
    allocation = (u32)cm_fn_00944e46(0x20u);
    D(self + 0x3502u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009cd0a0u, 0, 0, work + 0x100, work);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x100, work, 0x1800u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        B(self + 0x3506u) = 0;
        return;
    }
    value = D(0x009cf354u);
    D(allocation) = value;
    table = D(self + 0x3502u);
    value = D(0x009cf4f8u);
    D(table + 4u) = value;
    table = D(self + 0x3502u);
    value = D(0x009cf358u);
    D(table + 8u) = value;
    table = D(self + 0x3502u);
    D(table + 0xcu) = 0xffffffffu;
    table = D(self + 0x3502u);
    value = D(0x009cf508u);
    D(table + 0x10u) = value;
    table = D(self + 0x3502u);
    value = D(0x009cf4f8u);
    D(table + 0x14u) = value;
    table = D(self + 0x3502u);
    value = D(0x009cf404u);
    D(table + 0x18u) = value;
    table = D(self + 0x3502u);
    value = D(0x009cf484u);
    D(table + 0x1cu) = value;
}
