/* Preferred-image VA 0066bba0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_THISCALL cm_fn_00687430(u32 self, u8 index, u32 club, u8 flag, u32 base);
extern s32 CM_THISCALL cm_fn_00682f70(u32 self, s32 value);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0066bba0(u32 self)
{
    u8 work[0x200];
    u32 allocation, club, index, nation, target, count, text, value;
    u8 selected = 0, argument;
    W(self + 0x3eu) = 10;
    allocation = (u32)cm_fn_00944e46(0x24eu);
    D(self + 0xb1u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009eb120u, 0, 0, work + 0x100, work);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x100, work, 0x1a1u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    count = D(0x00ae235cu);
    club = D(0x00ae23b4u);
    index = 0;
    if ((s32)count > 0) {
        do {
            nation = D(club + 0x57u);
            if (nation != 0) {
                target = D(self + 4u);
                if (nation == target) {
                    argument = selected;
                    selected = (u8)(selected + 1u);
                    cm_fn_00687430(self, argument, club, 0, 0);
                    if ((s16)(s8)selected == (s16)W(self + 0x3eu))
                        break;
                }
            }
            count = D(0x00ae235cu);
            index += 1u;
            club += 0x245u;
        } while ((s32)index < (s32)count);
    }
    if ((s16)(s8)selected != (s16)W(self + 0x3eu))
        return 0;
    value = (u32)cm_fn_00682f70(self, 10000);
    D(self + 0x28u) = value;
    B(self + 0xe1u) = 1;
    return 1;
}
