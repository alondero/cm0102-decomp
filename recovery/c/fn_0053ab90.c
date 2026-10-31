#include "cmre.h"
/* Preferred-image VA 0x0053ab90, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

u32 CM_CDECL cm_fn_0053ab90(u32 object, u32 date)
{
    u8 scratch[512];
    u32 first_day, second_day, difference, low_first, low_second, eax, edx;
    char *version;
    if (object == 0) {
        cm_fn_00944cff(0x009b5ac8, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                      version, scratch + 256, scratch, 0x312e);
        eax = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return eax | 0xffu;
    }
    first_day = (u32)(s32)*(volatile s16 *)(object + 0x10u);
    second_day = (u32)(s32)*(volatile s16 *)date;
    difference = first_day - second_day;
    low_first = *(volatile u16 *)(object + 0x12u);
    low_second = *(volatile u16 *)(date + 2u);
    /* MOV DX and MOV AX retain their register's previous high half. */
    edx = (difference & 0xffff0000u) | low_first;
    eax = (object & 0xffff0000u) | low_second;
    eax -= edx;
    if ((s32)difference > 0)
        --eax;
    return eax;
}
