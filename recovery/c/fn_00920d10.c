/* Preferred-image VA 00920d10, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   005fa730's year argument is WORD-width; upper DX-source bits are dead. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u8 CM_THISCALL cm_fn_005fa730(u32, u32, s16, u32 *, u32 *, u32);
extern u32 CM_CDECL cm_fn_004afd80(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00920d10(u32 self)
{
    u8 scratch[0x208];
    B(self + 0xee) = 0;
    B(self + 0xef) = 0;
    u32 manager = D(0x00b5e134);
    D((u32)scratch) = 0xffffffffu;
    D((u32)scratch + 4) = 0xffffffffu;
    if (!manager) {
        cm_fn_00944cff(0x00ad4ce0, 0, 0, scratch + 0x108, scratch + 8);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x108, scratch + 8, 0x1ab);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    } else {
        u16 year = W(self + 0x40);
        u32 competition = D(0x009cf79c);
        s8 total = (s8)cm_fn_005fa730(manager, competition, (s16)(u16)(year + 2),
                                      (u32 *)scratch, (u32 *)(scratch + 4), 1);
        u8 index = 0;
        if (total > 0) {
            do {
                u32 id = D((u32)scratch + (u32)(s32)(s8)index * 4u);
                if (id == 0xffffffffu) break;
                u32 nations = D(0x00ae23a8);
                u32 region = D(nations + id * 290u + 0x71);
                if (region) {
                    u32 number = D(region);
                    if (number == D(0x009cfa1c)) {
                        B(self + 0xee) += 1;
                    } else {
                        number = D(region);
                        u32 target = D(0x009cfa18);
                        if (number == target) B(self + 0xef) += 1;
                    }
                }
                index = (u8)(index + 1);
            } while ((s8)index < total);
        }
    }
    u32 competition = D(0x009cf79c);
    u32 base = D(0x00ae23d0);
    u32 result = cm_fn_004afd80(base + competition * 107u, 0);
    if (result) {
        u32 club = result;
        u32 nation = D(club + 0x53);
        u32 second = D((u32)scratch + 4);
        u32 first = D((u32)scratch);
        if (nation) {
            u32 region = D(nation + 0x71);
            if (region) {
                u32 number = D(region);
                if (number == D(0x009cfa1c)) {
                    u32 id = D(nation);
                    if (id != first && id != second) B(self + 0xee) += 1;
                }
            }
        }
        result = D(club + 0x53);
        if (result) {
            u32 region = D(result + 0x71);
            if (region) {
                u32 number = D(region);
                u32 target = D(0x009cfa18);
                if (number == target) {
                    result = D(result);
                    if (result != first && result != second) B(self + 0xef) += 1;
                }
            }
        }
    }
    return result;
}
