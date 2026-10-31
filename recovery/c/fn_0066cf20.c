/* Preferred-image VA 0066cf20, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_00945501(u32 first, u32 second);
extern void CM_CDECL cm_fn_00945333(u32 base, u32 count, u32 size, u32 comparator);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0066cf20(u32 self)
{
    u8 work[0x200];
    u32 allocation, total, club, record, nation, selected, table, count = 0, scanned = 0, version;
    D(self + 0xf2u) = 10;
    allocation = cm_fn_00945501(4, 10);
    D(self + 0xeeu) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009eb15cu, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x2abu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    total = D(0x00ae235cu);
    club = D(0x00ae23b4u);
    if ((s32)total > 0) {
        do {
            record = D(club + 0x57u);
            if (record != 0) {
                nation = D(record);
                selected = D(0x009cfa00u);
                if (nation == selected) {
                    table = D(self + 0xeeu);
                    count += 1u;
                    D(table + count * 4u - 4u) = club;
                }
            }
            total = D(0x00ae235cu);
            scanned += 1u;
            club += 0x245u;
        } while ((s32)(s16)scanned < (s32)total);
    }
    table = D(self + 0xeeu);
    cm_fn_00945333(table, count, 4, 0x004b0780u);
    total = D(self + 0xf2u);
    if ((s32)count < (s32)total) {
        cm_fn_00944cff(0x009eb15cu, 0, 0, work, work + 0x100);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work, work + 0x100, 0x2bfu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
    }
    return 1;
}
