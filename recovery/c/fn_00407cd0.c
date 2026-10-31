/* Preferred-image VA 00407cd0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_THISCALL cm_fn_00687430(u32 self, u8 index, u32 club, u8 flag, u32 base);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00407cd0(u32 self)
{
    u8 work[0x200];
    u32 allocation, scanned = 0, total, club, nation, pointer, id, selected, holder, competition;
    u32 version;
    u8 count = 0, index;
    W(self + 0x3eu) = 25;
    allocation = (u32)cm_fn_00944e46(0x5c3u);
    D(self + 0xb1u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009874ecu, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x247u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    total = D(0x00ae235cu);
    club = D(0x00ae23b4u);
    if ((s32)total > 0) {
        do {
            nation = D(club + 0x57u);
            if (nation != 0) {
                pointer = D(club + 0x60u);
                if (pointer != 0) {
                    id = D(nation);
                    selected = D(0x009cf658u);
                    if (id == selected) {
                        index = count;
                        count = (u8)(count + 1u);
                        cm_fn_00687430(self, index, club, 0, 0);
                        if ((u16)(s16)(s8)count == W(self + 0x3eu))
                            break;
                    }
                }
            }
            total = D(0x00ae235cu);
            scanned += 1u;
            club += 0x245u;
        } while ((s32)(s16)scanned < (s32)total);
        if (count == 24) {
            id = D(0x009d020cu);
            holder = D(0x00ae23b4u);
            pointer = holder + id * 581u + 0x57u;
            competition = D(self + 4u);
            if (D(pointer) != competition) {
                D(pointer) = competition;
                id = D(0x009d020cu);
                holder = D(0x00ae23b4u);
                club = holder + id * 581u;
                cm_fn_00687430(self, 24, club, 0, 0);
                count = 25;
            }
        }
    }
    if ((u16)(s16)(s8)count != W(self + 0x3eu))
        return 0;
    D(self + 0x28u) = 0;
    B(self + 0xe1u) = 2;
    return 1;
}
