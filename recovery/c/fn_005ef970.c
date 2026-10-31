/* Preferred-image VA 005ef970, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_005ef200(u32 self, u32 source);
extern void CM_CDECL cm_fn_00945333(u32 base, u32 count, u32 size, u32 comparator);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_005ef970(u32 self, s8 category, u32 source)
{
    u8 work[0x200];
    u32 base, row, index, word, old_value, new_value, score, version;
    if (category < 0 || category > 27 || source == 0) {
        cm_fn_00944cff(0x009ce408u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x18fu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    base = self + (u32)(s32)category * 12000u;
    row = base;
    for (index = 0; index < 99; ++index) {
        for (word = 0; word < 30; ++word) {
            /* Native CMPSD faults on EDI before ESI (cases9/31). */
            new_value = D(source + word * 4u);
            old_value = D(row + word * 4u);
            if (old_value != new_value)
                break;
        }
        if (word == 30)
            return 0;
        row += 120;
    }
    score = D(source + 0x68u);
    if (score == 0)
        return 0;
    old_value = D(base + 0x2e68u + 0x68u);
    if (score <= old_value)
        return 0;
    cm_fn_005ef200(base + 0x2e68u, source);
    cm_fn_00945333(base, 100, 120, 0x005f0620u);
    return 1;
}
