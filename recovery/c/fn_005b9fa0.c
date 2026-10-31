/* Preferred-image VA 005b9fa0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_THISCALL cm_fn_00687430(u32 self, u8 index, u32 club, u8 flag, u32 base);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_005b9fa0(u32 self, u32 clubs)
{
    u8 scratch[0x200];
    u32 allocation, row, club, pointer, id, special, text;
    u8 index = 0, flag, argument;
    W(self + 0x3eu) = 18;
    allocation = (u32)cm_fn_00944e46(0x426u);
    D(self + 0xb1u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009ccaf0u, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, scratch + 0x100, scratch, 0x185u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    if ((s16)W(self + 0x3eu) > 0) {
        row = 0;
        do {
            club = D(clubs + (u32)(s32)(s16)row * 4u);
            pointer = D(club + 0x57u);
            id = D(pointer);
            special = D(0x009cf5ccu);
            if (id == special)
                flag = 1;
            else
                flag = id == D(0x009cf5d0u);
            argument = index;
            index = (u8)(index + 1u);
            cm_fn_00687430(self, argument, club, flag, 0);
            row += 1u;
        } while ((s16)row < (s16)W(self + 0x3eu));
    }
    return (s16)(s8)index == (s16)W(self + 0x3eu);
}
