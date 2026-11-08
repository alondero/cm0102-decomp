/* Preferred-image VA 0x004cbb60, base 0x00400000; CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical EAX/unused EDX/ECX prefix, two native stack DWORDs, RET8.
   The first native argument is initialized and passed by address to 540a50;
   object retains its original value while record receives direction output. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0053b620(u32 arg);
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern u32 CM_CDECL cm_fn_00542740(u32 club);

__attribute__((regparm(3)))
u32 CM_STDCALL cm_fn_004cbb60(u32 incoming_eax, u32 unused_edx,
                             u32 self, volatile u32 record, u32 club)
{
    u32 object = record;
    u32 eax = incoming_eax;
    u32 partner = 0;
    u32 candidate, limit, table, index, base;
    u8 flag;
    (void)unused_edx;
    eax = (eax & 0xffffff00u) | *(volatile u8 *)(object + 0x3du);
    if ((u8)eax == 12u) goto zero_al;
    eax = cm_fn_0053b620(eax);
    if (eax == 0u) goto zero_al;
    eax = *(volatile u32 *)(object + 0x39u);
    if (eax != 0u && eax == club) {
        eax = (eax & 0xffffff00u) | *(volatile u8 *)(object + 0x3du);
        eax = cm_fn_0053b620(eax);
        if (eax != 0u) {
            eax = cm_fn_00542740(club);
            if (eax != 0u) {
                flag = *(volatile u8 *)(object + 0x5eu);
                return ((eax & 0xffffff00u) | ((flag & 1u) != 0u)) + 1u;
            }
        }
    }
    eax = *(volatile u32 *)(object + 0x39u);
    if (eax != 0u) {
        eax = cm_fn_00540a50(eax, (u32)&record, 1u);
        partner = eax;
        if (partner != 0u) {
            eax = record;
            if (eax != 0u) {
                if (club == *(volatile u32 *)(object + 0x39u))
                    return (eax & 0xffffff00u) | 4u;
            } else if (club == partner) {
                return (eax & 0xffffff00u) | 3u;
            }
        }
    }
    limit = CM_GLOBAL(volatile u32, 0x00ae2364);
    table = CM_GLOBAL(volatile u32, 0x00ae2d0c);
    eax = 0u;
    if (object != 0u) {
        eax = *(volatile u32 *)object;
        if ((s32)eax >= (s32)limit) eax = 0u;
        else {
            index = eax * 79u;
            eax = *(volatile u32 *)(index + table);
            if ((s32)eax < 0) eax = 0u;
            else {
                base = CM_GLOBAL(volatile u32, 0x00ae1050);
                eax = eax * 80u;
                eax += *(volatile u32 *)base;
            }
        }
    }
    CM_GLOBAL(volatile u32, 0x00ae1080) = eax;
    if (eax == 0u) goto zero_al;
    eax = *(volatile u32 *)(eax + 4u);
    base = CM_GLOBAL(volatile u32, 0x00ae23b4);
    candidate = base + eax * 581u;
    if (candidate == 0u) goto zero_al;
    if (candidate == *(volatile u32 *)(object + 0x39u)) goto zero_al;
    if (candidate == partner) goto zero_al;
    eax = 0u;
    if (object != 0u) {
        eax = *(volatile u32 *)object;
        if ((s32)eax >= (s32)limit) eax = 0u;
        else {
            index = eax * 79u;
            eax = *(volatile u32 *)(index + table);
            if ((s32)eax < 0) eax = 0u;
            else {
                eax = eax * 80u;
                eax += *(volatile u32 *)self;
            }
        }
    }
    flag = *(volatile u8 *)(eax + 0x35u);
    /* SHL BL,1; SAR BL,7 selects the original bit 6. */
    flag = (flag & 0x40u) != 0u;
    if (candidate == club) {
        eax = cm_fn_00542740(club);
        if (eax != 0u)
            return ((eax & 0xffffff00u) | (flag == 0u)) + 1u;
    }
    eax = cm_fn_00540a50(candidate, (u32)&record, 1u);
    if (eax == 0u) goto zero_al;
    if (eax != club && candidate != club) goto zero_al;
    return ((eax & 0xffffff00u) | (flag == 0u)) + 3u;
zero_al:
    return eax & 0xffffff00u;
}
