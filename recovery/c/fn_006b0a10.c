#include "cmre.h"
/* Preferred-image VA 0x006b0a10, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_CDECL cm_fn_006b0a10(void)
{
    u32 eax, index, person, club, nation, value;
    eax = cm_fn_007fe600(0x00b74450, 0x006b0b10, 0x006b1790, 0, 0x006b1980, 0);
    if (!eax) return eax;
    index = (u32)(s32)(s16)W(0x00b77c7a);
    person = D(0x00b74c26 + index * 768u);
    value = 0xffffffffu;
    if (person) {
        club = D(person + 0x39);
        if (club) {
            nation = D(club + 0x53);
            value = D(nation);
        } else {
            nation = D(person + 0x1a);
            if (B(nation + 0x11c) & 2) value = D(nation);
            else {
                club = D(person + 0x24);
                if (club) {
                    nation = D(club + 0x53);
                    if (B(nation + 0x11c) & 2) value = D(nation);
                }
            }
        }
    }
    cm_fn_007ff270(0x00b74450, 0, value, 0);
    cm_fn_007ff270(0x00b74450, 1, 0, 0);
    cm_fn_007ff270(0x00b74450, 2, 0, 0);
    cm_fn_007ff270(0x00b74450, 3, 1, 0);
    cm_fn_007ff270(0x00b74450, 4, 1, 0);
    cm_fn_007ff270(0x00b74450, 5, 0, 0);
    cm_fn_007ff270(0x00b74450, 6, 0, 0);
    return (u32)cm_fn_007ff270(0x00b74450, 7, 1, 0);
}
