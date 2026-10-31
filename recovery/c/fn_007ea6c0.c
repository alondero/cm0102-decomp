/* Preferred-image VA 007ea6c0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern void CM_CDECL cm_fn_004afbd0(void *table, s16 count, s16 wanted);
extern u32 CM_THISCALL cm_fn_00687430(u32 self, u32 index, u32 club, u32 flag, u32 base);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007ea6c0(u32 self)
{
    u8 work[0x248];
    u8 selected = 0;
    u32 allocation, total, club, expected, pointer, id, scanned = 0;
    u32 version, i, current, nation, base;
    s16 count, wanted;
    W(self + 0x3eu) = 18;
    allocation = (u32)cm_fn_00944e46(0x426u);
    D(self + 0xb1u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x00a7ac8cu, 0, 0, work + 0x148, work + 0x48);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x148, work + 0x48, 0xf0u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    total = D(0x00ae235cu);
    club = D(0x00ae23b4u);
    if ((s32)total > 0) {
        expected = D(0x009cf92cu);
        do {
            pointer = D(club + 0x57u);
            if (pointer != 0) {
                id = D(pointer);
                if (id == expected && (s8)selected < 18) {
                    u32 slot = (u32)(s32)(s8)selected;
                    selected = (u8)(selected + 1u);
                    D((u32)work + slot * 4u) = club;
                }
            }
            ++scanned;
            club += 581u;
        } while ((s32)(s16)scanned < (s32)total);
    }
    wanted = (s16)W(self + 0x3eu);
    count = (s16)(s8)selected;
    if (count < wanted)
        return 0;
    cm_fn_004afbd0(work, count, wanted);
    i = 0;
    if (count > 0) {
        do {
            if ((s16)i < (s16)W(self + 0x3eu)) {
                current = D((u32)work + i * 4u);
                cm_fn_00687430(self, i, current, 0, 0);
            } else {
                nation = D(0x009cf91cu);
                current = D((u32)work + i * 4u);
                base = D(0x00ae23d0u);
                D(current + 0x57u) = base + nation * 107u;
            }
            ++i;
        } while ((s16)i < count);
    }
    return 1;
}
