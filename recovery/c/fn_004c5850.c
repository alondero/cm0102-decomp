/* Preferred-image VA 004c5850, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_STDCALL cm_fn_004c6890(u32 *club);
extern void CM_CDECL cm_fn_004afb40(u32 clubs, s16 count);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_004c5850(u32 self)
{
    u8 work[0x200];
    u32 allocation, countries, total, club, index, selected = 0, table, result, text;
    s16 count;
    D(self + 0xf2u) = 20;
    allocation = (u32)cm_fn_00944e46(0x50u);
    D(self + 0xeeu) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009a7e5cu, 0, 0, work + 0x100, work);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x100, work, 0x2c6u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    countries = D(0x00ae2350u);
    total = D(0x00ae235cu);
    club = D(0x00ae23b4u);
    index = 0;
    if ((s32)(total - countries * 2u) > 0) {
        do {
            (void)D(0x00ae1000u);
            result = cm_fn_004c6890((u32 *)club);
            if (result != 0) {
                table = D(self + 0xeeu);
                count = (s16)selected;
                selected += 1u;
                D(table + (u32)(s32)count * 4u) = club;
            }
            countries = D(0x00ae2350u);
            total = D(0x00ae235cu);
            index += 1u;
            club += 0x245u;
        } while ((s32)index < (s32)(total - countries * 2u));
    }
    count = (s16)W(self + 0xf2u);
    table = D(self + 0xeeu);
    cm_fn_004afb40(table, count);
    return 1;
}
