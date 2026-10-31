/* Preferred-image VA 00469da0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_007ff010(u32 self, u32 key);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 key, u32 value, u32 owned);
extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *pointer);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);

u32 CM_CDECL cm_fn_00469da0(void)
{
    u32 second, third, thirteenth, twenty_first, result;
    second = cm_fn_007ff010(0x00b74450u, 2);
    third = cm_fn_007ff010(0x00b74450u, 3);
    thirteenth = cm_fn_007ff010(0x00b74450u, 0xd);
    twenty_first = cm_fn_007ff010(0x00b74450u, 0x15);
    result = twenty_first;
    if (second) {
        cm_fn_00944c94((void *)second);
        result = cm_fn_007ff270(0x00b74450u, 2, 0, 0);
    }
    if (third) {
        cm_fn_00944c94((void *)third);
        result = cm_fn_007ff270(0x00b74450u, 3, 0, 0);
    }
    if (thirteenth) {
        cm_fn_0068d040(thirteenth);
        cm_fn_00944c94((void *)thirteenth);
        result = cm_fn_007ff270(0x00b74450u, 0xd, 0, 0);
    }
    if (twenty_first) {
        cm_fn_009452ca(twenty_first);
        result = cm_fn_007ff270(0x00b74450u, 0x15, 0, 0);
        result = cm_fn_007ff270(0x00b74450u, 0x16, 0, 0);
    }
    return result;
}
