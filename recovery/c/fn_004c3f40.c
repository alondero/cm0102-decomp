/* Preferred-image VA 004c3f40, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern void CM_CDECL cm_fn_004afb40(u32 entries, u32 raw_count);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_004c3f40(u32 self)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 allocation, version, line, total, club, pointer, value, expected, entries;
    u32 selected = 0, scanned = 0, raw_count;
    D(self + 0xf2u) = 32;
    allocation = (u32)cm_fn_00944e46(128);
    D(self + 0xeeu) = allocation;
    if (allocation == 0) {
        name = work + 0x100;
        extension = work;
        line = 0x285u;
        goto error;
    }
    total = D(0x00ae235cu);
    club = D(0x00ae23b4u);
    if ((s32)total > 0) {
        do {
            pointer = D(self + 4u);
            value = D(club + 0x1dbu);
            expected = D(pointer);
            if (value == expected && (s32)selected < (s32)D(self + 0xf2u)) {
                entries = D(self + 0xeeu);
                ++selected;
                D(entries + selected * 4u - 4u) = club;
            }
            total = D(0x00ae235cu);
            ++scanned;
            club += 0x245u;
        } while ((s32)(s16)scanned < (s32)total);
    }
    if (selected == D(self + 0xf2u)) {
        raw_count = (scanned & 0xffff0000u) | W(self + 0xf2u);
        entries = D(self + 0xeeu);
        cm_fn_004afb40(entries, raw_count);
        return 1;
    }
    name = work;
    extension = work + 0x100;
    line = 0x297u;
error:
    cm_fn_00944cff(0x009a7de4u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
