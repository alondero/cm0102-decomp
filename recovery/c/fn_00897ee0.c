/* Preferred-image VA 00897ee0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_008982b0(u32 self, u32 record, u32 option);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00897ee0(u32 self, u32 record, s8 slot,
                              volatile u32 replacement, volatile u32 option)
{
    u8 work[0x200];
    u32 index, pointer, base, offset, value, version;
    if (record == 0) {
        cm_fn_00944cff(0x00a8b908u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x675u);
        goto diagnostic;
    }
    if (slot < 0 || slot > 1) {
        cm_fn_00944cff(0x00a8b908u, 0, 0, work, work + 0x100);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work, work + 0x100, 0x67du);
        goto diagnostic;
    }
    index = cm_fn_008982b0(self, record, option);
    if (index == 0xffffffffu)
        return 0;
    pointer = replacement;
    if (pointer != 0) {
        base = D(self + 0x601u);
        base += (u32)(s32)slot * 4u;
        offset = index * 145u;
        value = D(pointer);
        D(base + offset + 0x56u) = value;
        return 1;
    }
    base = D(self + 0x601u);
    offset = index * 145u;
    D(base + (u32)(s32)slot * 4u + offset + 0x56u) = 0xffffffffu;
    if (slot == 0) {
        base = D(self + 0x601u);
        base += offset;
        value = D(base + 0x5au);
        D(base + 0x56u) = value;
        base = D(self + 0x601u);
        D(base + offset + 0x5au) = 0xffffffffu;
    }
    return 1;
diagnostic:
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
