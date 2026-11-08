/* Preferred-image VA 004afbd0, image base 00400000; CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL3 with genuine incoming EAX/EDX prefix, RET0. Full incidental EAX
   survives early exits. All callees remain original boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
typedef unsigned long long wide_4afbd0;
extern wide_4afbd0 CM_CDECL __attribute__((regparm(2))) cm_fn_00945333(
    u32 eax_in, u32 edx_in, u32 base, u32 count, u32 width, u32 compare);
extern wide_4afbd0 CM_CDECL __attribute__((regparm(2))) cm_fn_0090cfc0(
    u32 eax_in, u32 edx_in, u32 bound);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL __attribute__((regparm(2))) cm_fn_004afbd0(
    u32 entry_eax, u32 entry_edx, u32 base, u32 count, u32 wanted)
{
    u8 scratch[0x200];
    u32 eax = entry_eax, edx = entry_edx, selected, remaining;
    u32 first_result, first_address, second_address, second_value;
    wide_4afbd0 result;
    if ((s16)count < 0 || (s16)count < (s16)wanted) {
        cm_fn_00944cff(0x0099ec20u, 0, 0, scratch + 0x100, scratch);
        eax = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, eax,
                      (u32)(scratch + 0x100), (u32)scratch, 0xf1u);
        /* Original 005e8290 does not consume incoming EDX: see pinned
           assert-edx-contract/CONTRACT.txt (005e5071 kills it before use). */
        eax = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return eax;
    }
    if ((s16)wanted < 1) return eax;
    eax = (u32)(s32)(s16)count;
    result = cm_fn_00945333(eax, edx, base, eax, 4, 0x004b07d0u);
    edx = (u32)(result >> 32);
    eax = wanted * 3u;
    if ((s16)eax > (s16)count) eax = count;
    if ((s16)eax <= 0) return eax;
    selected = (u32)(s32)(s16)eax;
    remaining = selected;
    do {
        result = cm_fn_0090cfc0(eax, edx, selected);
        first_result = (u32)result;
        edx = (u32)(result >> 32);
        result = cm_fn_0090cfc0(first_result, edx, selected);
        eax = (u32)(s32)(s16)(u32)result;
        second_value = D(base + eax * 4u);
        edx = D(base + (u32)(s32)(s16)first_result * 4u);
        second_address = base + eax * 4u;
        first_address = base + (u32)(s32)(s16)first_result * 4u;
        --remaining;
        D(first_address) = second_value;
        D(second_address) = edx;
        eax = second_address;
    } while (remaining != 0);
    return eax;
}
